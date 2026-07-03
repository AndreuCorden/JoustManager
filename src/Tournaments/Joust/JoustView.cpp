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
      playerHorseColorIdx(0), // 0 = BrownHorse
      enemyHorseColorIdx(3)   // 3 = GreyHorse
{
    setFocusPolicy(Qt::StrongFocus);
    setFocus();

    scene = new QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT, this);
    setScene(scene);
    setBackgroundBrush(QBrush(QColor("#4A5568")));
    setFixedSize(SCENE_WIDTH + 5, SCENE_HEIGHT + 5);

    // Load each color file into our array cleanly
    horseSheets[0].load(":/assets/BrownHorse_Run.png");
    horseSheets[1].load(":/assets/WhiteHorse_Run.png");
    horseSheets[2].load(":/assets/BlackHorse_Run.png");
    horseSheets[3].load(":/assets/GreyHorse_Run.png");

    announcerText = scene->addText("READY TO JOUST!\nClick Screen or Press SPACE to Charge", QFont("Arial", 16, QFont::Bold));
    announcerText->setDefaultTextColor(Qt::white);
    announcerText->setPos(CENTER_X - 180, 40);

    qteDisplayItem = scene->addText("", QFont("Arial", 36, QFont::Bold));
    qteDisplayItem->setDefaultTextColor(QColor("#E53E3E"));
    qteDisplayItem->setPos(CENTER_X - 25, 100);

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

QPixmap JoustView::getHorseFrame(int colorIdx, ViewDirection direction, int frameNum) {
    // Safety fallback
    if (colorIdx < 0 || colorIdx > 3 || horseSheets[colorIdx].isNull()) {
        return QPixmap(60, 33);
    }

    const int FRAME_WIDTH = 60;   // 360 total width / 6 frames
    const int FRAME_HEIGHT = 33;

    // Slice out the exact single frame out of the 6 running frames
    QPixmap frame = horseSheets[colorIdx].copy(frameNum * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT);

    // If the horse needs to move left, mirror the image horizontally
    if (direction == ViewDirection::Right) {
        frame = frame.transformed(QTransform().scale(-1, 1));
    }

    return frame;
}

void JoustView::updateVisualSprites() {
    int pFrame = currentFrameIndex;
    int eFrame = currentFrameIndex;

    // Use frame index 1 (the 2nd image) for standing still
    if (currentState == JoustState::WaitingToStart) {
        pFrame = 1;
        eFrame = 1;
    }

    playerSprite->setPixmap(getHorseFrame(playerHorseColorIdx, playerFacing, pFrame));
    enemySprite->setPixmap(getHorseFrame(enemyHorseColorIdx, enemyFacing, eFrame));
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
    if (qteTimer) qteTimer->stop();

    qteActive = false;
    if (qteDisplayItem) qteDisplayItem->setPlainText("");
    playerSpeedBoost = 0.0;

    if (currentCharge % 2 != 0) {
        playerX = 50;
        enemyX = SCENE_WIDTH - 150;
        playerFacing = ViewDirection::Right;
        enemyFacing = ViewDirection::Left;
    } else {
        playerX = SCENE_WIDTH - 150;
        enemyX = 50;
        playerFacing = ViewDirection::Left;
        enemyFacing = ViewDirection::Right;
    }

    // Adjusting Y position so the horses sit beautifully on either side of the barrier
    // The line is at Y=220, thickness is 6px (ranges 220 to 226)
    if (playerSprite) playerSprite->setPos(playerX, 230); // 186 + 33 = 219 (rests on top of line)
    if (enemySprite) {
        enemySprite->setPos(enemyX, 175);                 // 228 (rests just beneath line thickness)
        enemySprite->setRotation(0);
    }

    updateVisualSprites();
}

// 5. Run a 6-frame loop engine pass
void JoustView::updateSpriteFrames()
{
    spriteFrameCounter++;
    if (spriteFrameCounter >= 5) { // Slightly speed up animation cycles for extra energy
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