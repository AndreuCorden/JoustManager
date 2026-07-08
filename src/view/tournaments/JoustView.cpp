#include "view/tournaments/JoustView.h"
#include <QKeyEvent>

const int SCENE_WIDTH = 1200;
const int SCENE_HEIGHT = 400;

JoustView::JoustView(QWidget *parent) : QGraphicsView(parent) {
    setFocusPolicy(Qt::StrongFocus);
    setFixedSize(SCENE_WIDTH + 5, SCENE_HEIGHT + 5);

    scene = new QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT, this);
    setScene(scene);

    QPixmap bgPixmap(":/assets/Backgroundjoust.png");
    setBackgroundBrush(bgPixmap.isNull() ? QBrush(QColor("#4A5568")) : QBrush(bgPixmap.scaled(SCENE_WIDTH, SCENE_HEIGHT)));

    announcerText = scene->addText("", QFont("Arial", 16, QFont::Bold));
    announcerText->setDefaultTextColor(Qt::white);
    announcerText->setPos(SCENE_WIDTH / 2 - 180, 40);
    announcerText->setZValue(10);

    qteDisplayItem = scene->addText("", QFont("Arial", 36, QFont::Bold));
    qteDisplayItem->setDefaultTextColor(QColor("#E53E3E"));
    qteDisplayItem->setPos(SCENE_WIDTH / 2 - 25, 100);
    qteDisplayItem->setZValue(10);

    horseSheets[0].load(":/assets/BrownHorse_Run.png");
    horseSheets[1].load(":/assets/WhiteHorse_Run.png");
    horseSheets[2].load(":/assets/BlackHorse_Run.png");
    horseSheets[3].load(":/assets/GreyHorse_Run.png");
    knightPixmap.load(":/assets/Knight2.png");

    playerSprite = scene->addPixmap(QPixmap());
    enemySprite = scene->addPixmap(QPixmap());
    playerKnightSprite = scene->addPixmap(QPixmap());
    enemyKnightSprite = scene->addPixmap(QPixmap());

    playerKnightSprite->setParentItem(playerSprite);
    enemyKnightSprite->setParentItem(enemySprite);
    playerKnightSprite->setPos(35, -24);
    enemyKnightSprite->setPos(40, -24);

    tiltBarrier = scene->addPixmap(QPixmap());
    QPixmap fence(":/assets/Fence.png");
    if (!fence.isNull()) tiltBarrier->setPixmap(fence.scaled(SCENE_WIDTH, 43));
    tiltBarrier->setPos(0, 222);
    tiltBarrier->setZValue(2);
}

void JoustView::updateRenderState(double pX, double eX, int charge, int frameIndex, 
                                  JoustState state, const QString& announcer, const QString& qte) 
{
    announcerText->setPlainText(announcer);
    qteDisplayItem->setPlainText(qte);

    ViewDirection pFacing = (charge % 2 != 0) ? ViewDirection::Right : ViewDirection::Left;
    ViewDirection eFacing = (pFacing == ViewDirection::Right) ? ViewDirection::Left : ViewDirection::Right;

    int animFrame = (state == JoustState::WaitingToStart) ? 1 : frameIndex;

    playerSprite->setPixmap(getHorseFrame(0, pFacing, animFrame));
    enemySprite->setPixmap(getHorseFrame(3, eFacing, animFrame));
    playerKnightSprite->setPixmap(getKnightPixmap(pFacing, animFrame));
    enemyKnightSprite->setPixmap(getKnightPixmap(eFacing, animFrame));

    playerSprite->setPos(pX, (charge % 2 != 0) ? 225 : 190);
    enemySprite->setPos(eX, (charge % 2 != 0) ? 190 : 225);
    
    playerSprite->setZValue((charge % 2 != 0) ? 4 : 1);
    enemySprite->setZValue((charge % 2 != 0) ? 1 : 4);

    if (state == JoustState::Passing && charge == 3) {
        enemySprite->setRotation(90);
    } else {
        enemySprite->setRotation(0);
    }
}

void JoustView::mousePressEvent(QMouseEvent*) {
    setFocus();
    emit actionTriggered();
}

void JoustView::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Space) {
        emit actionTriggered();
    } else {
        emit inputKeyPressed(event->key());
    }
}

void JoustView::closeEvent(QCloseEvent*) {
    emit viewClosed();
}

QPixmap JoustView::getHorseFrame(int colorIdx, ViewDirection direction, int frameNum)
{
    // Safety fallback if textures fail to load
    if (colorIdx < 0 || colorIdx > 3 || horseSheets[colorIdx].isNull())
    {
        return QPixmap(84, 46);
    }

    const int FRAME_WIDTH = 60; // 360 total width / 6 frames
    const int FRAME_HEIGHT = 33;

    // Slice the specific target animation frame from the spreadsheet strip
    QPixmap frame = horseSheets[colorIdx].copy(frameNum * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT);

    // Flip horizontally if facing right, otherwise keep default scaling direction
    qreal scaleX = (direction == ViewDirection::Right) ? -1.7 : 1.7;
    qreal scaleY = 1.7;

    return frame.transformed(QTransform().scale(scaleX, scaleY), Qt::SmoothTransformation);
}

QPixmap JoustView::getKnightPixmap(ViewDirection direction, int frameIndex)
{
    if (knightPixmap.isNull())
    {
        return QPixmap(60, 133); 
    }

    const int KNIGHT_FRAME_WIDTH = 350;
    const int KNIGHT_FRAME_HEIGHT = 833;

    // 🌟 MVC FIX: Use the active frameIndex passed from the model/controller loop
    int atlasFrameNum = (frameIndex / 3) % 2; 

    // Slice out the running frame sequence from the knight texture atlas
    QPixmap frame = knightPixmap.copy(atlasFrameNum * KNIGHT_FRAME_WIDTH, 0, 
                                       KNIGHT_FRAME_WIDTH, KNIGHT_FRAME_HEIGHT);

    qreal downScaleY = 0.08;
    qreal scaleX = (direction == ViewDirection::Left) ? -0.08 : 0.08;

    return frame.transformed(QTransform().scale(scaleX, downScaleY), 
                             Qt::SmoothTransformation);
}