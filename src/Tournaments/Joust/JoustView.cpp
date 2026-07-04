#include "JoustView.h"
#include <QRandomGenerator>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDialog>
#include <QVBoxLayout>

const int SCENE_WIDTH = 1200;
const int SCENE_HEIGHT = 400;
const int CENTER_X = SCENE_WIDTH / 2;

JoustView::JoustView(const std::vector<Knight> &playerSquad, const std::vector<Knight> &enemySquad, QWidget *parent)
    : QGraphicsView(parent),
      currentState(JoustState::WaitingToStart), currentCharge(1),
      qteActive(false), baseSpeed(6.0), playerSpeedBoost(0.0),
      spriteFrameCounter(0), currentFrameIndex(0),
      playerHorseColorIdx(0), // 0 = BrownHorse
      enemyHorseColorIdx(3)   // 3 = GreyHorse
{
    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    scene = new QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT, this);
    setScene(scene);
    setBackgroundBrush(QBrush(QColor("#4A5568")));
    setFixedSize(SCENE_WIDTH + 5, SCENE_HEIGHT + 5);

    QPixmap bgPixmap(":/assets/Backgroundjoust.png");
    if (!bgPixmap.isNull())
    {
        // Scales your pixel art to 800x400 using smooth filtering
        QPixmap scaledBg = bgPixmap.scaled(SCENE_WIDTH, SCENE_HEIGHT, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        setBackgroundBrush(QBrush(scaledBg));
    }
    else
    {
        qWarning("Failed to load background image, falling back to flat gray color.");
        setBackgroundBrush(QBrush(QColor("#4A5568"))); // Gray safety fallback
    }

    announcerText = scene->addText("READY TO JOUST!\nClick Screen or Press SPACE to Charge", QFont("Arial", 16, QFont::Bold));
    announcerText->setDefaultTextColor(Qt::white);
    announcerText->setPos(CENTER_X - 180, 40);

    // Load each color file into our array cleanly
    horseSheets[0].load(":/assets/BrownHorse_Run.png");
    horseSheets[1].load(":/assets/WhiteHorse_Run.png");
    horseSheets[2].load(":/assets/BlackHorse_Run.png");
    horseSheets[3].load(":/assets/GreyHorse_Run.png");
    knightPixmap.load(":/assets/Knight2.png"); 

    qteDisplayItem = scene->addText("", QFont("Arial", 36, QFont::Bold));
    qteDisplayItem->setDefaultTextColor(QColor("#E53E3E"));
    qteDisplayItem->setPos(CENTER_X - 25, 100);

    playerSprite = scene->addPixmap(QPixmap());
    enemySprite = scene->addPixmap(QPixmap());

    playerKnightSprite = scene->addPixmap(QPixmap());
    enemyKnightSprite = scene->addPixmap(QPixmap());

    playerKnightSprite->setParentItem(playerSprite);
    enemyKnightSprite->setParentItem(enemySprite);

    tiltBarrier = scene->addPixmap(QPixmap());

    QPixmap fence(":/assets/Fence.png");
    if (!fence.isNull())
    {
        QPixmap scaledFence = fence.scaled(SCENE_WIDTH, 43, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        tiltBarrier->setPixmap(scaledFence);
    }
    tiltBarrier->setPos(0, 222);

    announcerText->setZValue(10);
    qteDisplayItem->setZValue(10);

    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &JoustView::gameTick);
    gameTimer->start(16);

    qteTimer = new QTimer(this);
    connect(qteTimer, &QTimer::timeout, this, &JoustView::triggerQTE);

    resetRound();
}

JoustView::~JoustView()
{
    // Turn off all background processing loops the split second this widget is removed
    if (gameTimer)
    {
        gameTimer->stop();
    }
    if (qteTimer)
    {
        qteTimer->stop();
    }
}

QPixmap JoustView::getHorseFrame(int colorIdx, ViewDirection direction, int frameNum)
{
    // Safety fallback
    if (colorIdx < 0 || colorIdx > 3 || horseSheets[colorIdx].isNull())
    {
        return QPixmap(84, 46);
    }

    const int FRAME_WIDTH = 60; // 360 total width / 6 frames
    const int FRAME_HEIGHT = 33;

    // Slice out the exact single frame out of the 6 running frames
    QPixmap frame = horseSheets[colorIdx].copy(frameNum * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT);

    qreal scaleX = (direction == ViewDirection::Right) ? -1.7 : 1.7;
    qreal scaleY = 1.7;

    frame = frame.transformed(QTransform().scale(scaleX, scaleY), Qt::SmoothTransformation);

    return frame;
}

QPixmap JoustView::getKnightPixmap(ViewDirection direction)
{
    if (knightPixmap.isNull())
    {
        // Safe fallback dimensions matching previous 35x78 scaled size
        return QPixmap(60, 133); 
    }

    // 1. 🌟 Define dimensions matching your new giant asset frames
    const int KNIGHT_FRAME_WIDTH = 350;
    const int KNIGHT_FRAME_HEIGHT = 833;

    // We alternate which frame to use based on the horse frame loop
    int atlasFrameNum = (currentFrameIndex/3) % 2; 

    // 2. 🌟 Slice the exact frame we need out of the atlas
    QPixmap frame = knightPixmap.copy(atlasFrameNum * KNIGHT_FRAME_WIDTH, 0, 
                                       KNIGHT_FRAME_WIDTH, KNIGHT_FRAME_HEIGHT);

    // 3. 🌟 Calculate downscaling to keep him seated perfectly
    // Our target vertical size remains 133px (matching previous 78 * 1.7 scale)
    // 133 / 794 giant source height = ~0.1675 downscale factor.
    qreal downScaleY = 0.08;
    
    // Natively faces right; if left, flip horizontally (-0.1675)
    qreal scaleX = (direction == ViewDirection::Left) ? -0.08 : 0.08;

    // 4. 🌟 Transform! Downscale with anti-aliasing to make it clean
    return frame.transformed(QTransform().scale(scaleX, downScaleY), 
                             Qt::SmoothTransformation);
}

void JoustView::updateVisualSprites()
{
    int pFrame = currentFrameIndex;
    int eFrame = currentFrameIndex;

    // Use frame index 1 (the 2nd image) for standing still
    if (currentState == JoustState::WaitingToStart)
    {
        pFrame = 1;
        eFrame = 1;
    }

    playerSprite->setPixmap(getHorseFrame(playerHorseColorIdx, playerFacing, pFrame));
    enemySprite->setPixmap(getHorseFrame(enemyHorseColorIdx, enemyFacing, eFrame));

    if (playerKnightSprite) playerKnightSprite->setPixmap(getKnightPixmap(playerFacing));
    if (enemyKnightSprite)   enemyKnightSprite->setPixmap(getKnightPixmap(enemyFacing));

    int localX = 35;
    int localY = -24;

    if (playerKnightSprite) playerKnightSprite->setPos(localX, localY);
    if (enemyKnightSprite)   enemyKnightSprite->setPos(localX + 5, localY);
}

void JoustView::mousePressEvent(QMouseEvent *event)
{
    setFocus();

    if (currentState == JoustState::WaitingToStart)
    {
        startMatch();
    }
    // 👍 FIXED: If the match has ended, a click will close out the arena window cleanly
    else if (currentState == JoustState::MatchOver)
    {
        QWidget *p = this->parentWidget();
        while (p)
        {
            QDialog *dialog = qobject_cast<QDialog *>(p);
            if (dialog)
            {
                dialog->accept();
                return;
            }
            p = p->parentWidget();
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void JoustView::resetRound()
{
    if (qteTimer)
        qteTimer->stop();

    qteActive = false;
    if (qteDisplayItem)
        qteDisplayItem->setPlainText("");
    playerSpeedBoost = 0.0;

    // The dimensions of our upscaled horses (~46px high)
    const int TOP_LANE_Y = 190;    // Higher up, slightly tucked behind the taller fence
    const int BOTTOM_LANE_Y = 225; // Lower down, completely in the foreground

    if (currentCharge % 2 != 0)
    {
        // 🏇 Pass 1 & 3: Player is on Bottom Track (Right), Enemy is on Top Track (Left)
        playerX = 50;
        enemyX = SCENE_WIDTH - 150;
        playerFacing = ViewDirection::Right;
        enemyFacing = ViewDirection::Left;

        if (playerSprite)
        {
            playerSprite->setPos(playerX, BOTTOM_LANE_Y);
            playerSprite->setZValue(4); // Foreground layer
        }
        if (enemySprite)
        {
            enemySprite->setPos(enemyX, TOP_LANE_Y);
            enemySprite->setZValue(1); // Background layer (behind fence)
            enemySprite->setRotation(0);
        }
    }
    else
    {
        // 🏇 Pass 2: Player is on Top Track (Left), Enemy is on Bottom Track (Right)
        playerX = SCENE_WIDTH - 150;
        enemyX = 50;
        playerFacing = ViewDirection::Left;
        enemyFacing = ViewDirection::Right;

        if (playerSprite)
        {
            playerSprite->setPos(playerX, TOP_LANE_Y);
            playerSprite->setZValue(1); // Background layer (behind fence)
        }
        if (enemySprite)
        {
            enemySprite->setPos(enemyX, BOTTOM_LANE_Y);
            enemySprite->setZValue(4); // Foreground layer
            enemySprite->setRotation(0);
        }
    }

    // Keep the fence securely locked between them at middle layer depth
    if (tiltBarrier)
    {
        tiltBarrier->setZValue(2);
    }

    updateVisualSprites();
}

// 5. Run a 6-frame loop engine pass
void JoustView::updateSpriteFrames()
{
    spriteFrameCounter++;
    if (spriteFrameCounter >= 5)
    { // Slightly speed up animation cycles for extra energy
        spriteFrameCounter = 0;
        currentFrameIndex = (currentFrameIndex + 1) % 6; // 👍 Loop cleanly through all 6 running frames
        updateVisualSprites();
    }
}

void JoustView::startMatch()
{
    if (currentState == JoustState::WaitingToStart)
    {
        currentState = JoustState::Charging;
        announcerText->setPlainText(QString("PASS %1! CHARGE!").arg(currentCharge));
        qteTimer->start(800);

        // Update facing vectors to match the side track runs
        if (currentCharge % 2 != 0)
        {
            playerFacing = ViewDirection::Right;
            enemyFacing = ViewDirection::Left;
        }
        else
        {
            playerFacing = ViewDirection::Left;
            enemyFacing = ViewDirection::Right;
        }
        updateVisualSprites();
    }
}

void JoustView::triggerQTE()
{
    if (currentState != JoustState::Charging)
        return;

    // Pick a random controller input key to prompt on screen
    int randIdx = QRandomGenerator::global()->bounded(4);
    const char keys[] = {'W', 'A', 'S', 'D'};
    const int qtKeys[] = {Qt::Key_W, Qt::Key_A, Qt::Key_S, Qt::Key_D};

    targetQteKey = qtKeys[randIdx];
    qteDisplayItem->setPlainText(QString(keys[randIdx]));
    qteActive = true;
}

void JoustView::keyPressEvent(QKeyEvent *event)
{
    if (currentState == JoustState::WaitingToStart && event->key() == Qt::Key_Space)
    {
        startMatch();
    }
    else if (qteActive)
    {
        checkQTE(event->key());
    }
}

void JoustView::checkQTE(int pressedKey)
{
    if (pressedKey == targetQteKey)
    {
        playerSpeedBoost += 2.5; // Success! Reward player with explicit velocity gain
        qteDisplayItem->setPlainText("BOOST!");
    }
    else
    {
        qteDisplayItem->setPlainText("MISS!");
    }
    qteActive = false;
}

void JoustView::gameTick()
{
    if (currentState != JoustState::Charging && currentState != JoustState::Passing)
    {
        return;
    }

    // Determine travel direction multiplier (+1 = moving right, -1 = moving left)
    qreal pDirection = (currentCharge % 2 != 0) ? 1.0 : -1.0;
    qreal eDirection = -pDirection;

    // Update positions using base speeds modified by active boost states
    qreal pSpeed = (baseSpeed + playerSpeedBoost) * pDirection;
    qreal eSpeed = baseSpeed * eDirection;

    playerX += pSpeed;
    enemyX += eSpeed;

    playerSprite->setPos(playerX, playerSprite->y());
    enemySprite->setPos(enemyX, enemySprite->y());

    updateSpriteFrames();

    // STATE TRANSITION CHECK: The Impact Clash
    if (currentState == JoustState::Charging)
    {
        // If they have crossed paths or met at center bounds
        if ((pDirection > 0 && playerX >= enemyX) || (pDirection < 0 && playerX <= enemyX))
        {
            currentState = JoustState::Passing;
            qteTimer->stop();
            qteDisplayItem->setPlainText("💥 SMASH!");

            if (currentCharge == 3)
            {
                // Finale pass! Spin or unseat opponent
                enemySprite->setRotation(90); // Knocked down visual representation
            }
        }
    }

    // STATE TRANSITION CHECK: Reached the opposite side boundaries
    bool playerFinished = (pDirection > 0) ? (playerX > SCENE_WIDTH) : (playerX < -50);
    if (playerFinished)
    {
        if (currentCharge < 3)
        {
            currentCharge++;
            currentState = JoustState::WaitingToStart;
            announcerText->setPlainText(QString("Pass %1 Completed.\nPress SPACE for next Pass!").arg(currentCharge - 1));
            resetRound();
        }
        else
        {
            // 👍 FIXED: Turn off engine processing loops entirely upon completion
            gameTimer->stop();
            currentState = JoustState::MatchOver;
            announcerText->setPos(CENTER_X - 150, 50);
            announcerText->setPlainText("VICTORY!\n\nClick Screen to Complete Match.");
            qteDisplayItem->setPlainText("");
        }
    }
}