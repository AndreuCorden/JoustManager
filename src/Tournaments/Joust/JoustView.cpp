#include "JoustView.h"
#include <QRandomGenerator>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QDialog>
#include <QVBoxLayout>

const int SCENE_WIDTH = 800;
const int SCENE_HEIGHT = 400;
const int CENTER_X = SCENE_WIDTH / 2;

JoustView::JoustView(const std::vector<Knight> &playerSquad, const std::vector<Knight> &enemySquad, QWidget *parent)
    : QGraphicsView(parent),
      currentState(JoustState::WaitingToStart), currentCharge(1),
      qteActive(false), baseSpeed(4.0), playerSpeedBoost(0.0),
      spriteFrameCounter(0), currentFrameIndex(0),
      playerHorseColorIdx(0), // 👈 Assign breed color index (0 to 7)
      enemyHorseColorIdx(3)   // 👈 Give enemy a different breed color index (0 to 7)
{
    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    scene = new QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT, this);
    setScene(scene);
    setBackgroundBrush(QBrush(QColor("#4A5568")));
    setFixedSize(SCENE_WIDTH + 5, SCENE_HEIGHT + 5);

    // Load the sprite sheet asset image file
    if (!spriteSheet.load(":/assets/horses.png")) {
        qWarning("Failed to load sprite sheet from embedded resources!");
    }

    announcerText = scene->addText("READY TO JOUST!\nClick Screen or Press SPACE to Charge", QFont("Arial", 16, QFont::Bold));
    announcerText->setDefaultTextColor(Qt::white);
    announcerText->setPos(CENTER_X - 180, 40);

    qteDisplayItem = scene->addText("", QFont("Arial", 36, QFont::Bold));
    qteDisplayItem->setDefaultTextColor(QColor("#E53E3E"));
    qteDisplayItem->setPos(CENTER_X - 25, 100);

    // Instantiate scene layout pixmap containers
    playerSprite = scene->addPixmap(QPixmap());
    enemySprite = scene->addPixmap(QPixmap());

    tiltBarrier = scene->addLine(0, 220, SCENE_WIDTH, 220, QPen(QColor("#D4AF37"), 6));

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

QPixmap JoustView::getHorseFrame(int breedIndex, ViewDirection direction, int frameNum) {
    if (spriteSheet.isNull()) return QPixmap(64, 64);

    const int FRAME_SIZE = 70;

    // 1. Determine if the breed is in the Top Block (0-3) or Bottom Block (4-7)
    int sheetRowBlock = breedIndex / 4;      // 0 for breeds 0-3, 1 for breeds 4-7
    int breedColumnGroup = breedIndex % 4;   // Horizontal index inside that block (0 to 3)

    // 2. Calculate the base Y position
    // Top block rows start at Y=0. Bottom block rows start after 4 direction rows (4 * 64 = 256 pixels down)
    int y = (sheetRowBlock * 4 * FRAME_SIZE) + (static_cast<int>(direction) * FRAME_SIZE);

    // 3. Calculate the base X position
    // Each breed group owns exactly 3 columns of animation frames side-by-side
    int x = (breedColumnGroup * 3 * FRAME_SIZE) + (frameNum * FRAME_SIZE);

    // 4. Safely crop the isolated frame out of the sheet texture maps
    return spriteSheet.copy(x, y, FRAME_SIZE, FRAME_SIZE);
}

void JoustView::updateVisualSprites()
{
    // Frames alternate through a 3-stage loop: 0 -> 1 -> 2 -> 1 -> repeat
    int animFrame = currentFrameIndex;
    if (animFrame == 3)
        animFrame = 1; // Maps 4 frame counters smoothly into a 3-frame looping chain

    playerSprite->setPixmap(getHorseFrame(playerHorseColorIdx, playerFacing, animFrame));
    enemySprite->setPixmap(getHorseFrame(enemyHorseColorIdx, enemyFacing, animFrame));
}


// To draw knight on top.
/*void JoustView::updateVisualSprites() {
    int animFrame = currentFrameIndex;
    if (animFrame == 3) animFrame = 1;

    // 1. Extract base horse crop
    QPixmap horsePixmap = getHorseFrame(playerHorseColorIdx, playerFacing, animFrame);

    // 2. Create an editable painter overlay layer
    QPainter painter(&horsePixmap);
    
    // 3. Load your knight asset file (e.g., a knight sitting pose with transparent background)
    QPixmap knightPixmap("knight_overlay.png"); 
    
    // 4. Overlay the knight directly on top of the horse centered coordinates
    painter.drawPixmap(0, -10, knightPixmap); // Adjust -10 up/down to seat him perfectly in the saddle
    painter.end();

    // 5. Send complete composite image to viewport
    playerSprite->setPixmap(horsePixmap);
}*/

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

    // Alternate initial spawn locations and directions depending on pass count sequence
    if (currentCharge % 2 != 0)
    {
        // Pass 1 & 3: Player starts Left (Charges Right), Enemy starts Right (Charges Left)
        playerX = 50;
        enemyX = SCENE_WIDTH - 150;

        playerFacing = ViewDirection::Right;
        enemyFacing = ViewDirection::Left;
    }
    else
    {
        // Pass 2: Player starts Right (Charges Left), Enemy starts Left (Charges Right)
        playerX = SCENE_WIDTH - 150;
        enemyX = 50;

        playerFacing = ViewDirection::Left;
        enemyFacing = ViewDirection::Right;
    }

    // OVERRIDE FOR BEFORE THE START: Show horses facing away/forward at rest before charge!
    if (currentState == JoustState::WaitingToStart)
    {
        playerFacing = ViewDirection::Down; // Facing the audience/screen forward
        enemyFacing = ViewDirection::Up;    // Facing away towards the background tents
    }

    if (playerSprite)
        playerSprite->setPos(playerX, 140);
    if (enemySprite)
    {
        enemySprite->setPos(enemyX, 230);
        enemySprite->setRotation(0);
    }

    updateVisualSprites();
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

void JoustView::updateSpriteFrames()
{
    spriteFrameCounter++;
    if (spriteFrameCounter >= 6)
    { // Advance gait speed loop pace every 6 frames
        spriteFrameCounter = 0;
        currentFrameIndex = (currentFrameIndex + 1) % 4; // Ping-pong loops (0,1,2,3)
        updateVisualSprites();
    }
}