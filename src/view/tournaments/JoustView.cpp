#include "view/tournaments/JoustView.h"
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsView>

const int SCENE_WIDTH = 1200;
const int SCENE_HEIGHT = 400;

JoustView::JoustView(QWidget *parent) : QWidget(parent)
{
    setFocusPolicy(Qt::StrongFocus);
    setFixedWidth(SCENE_WIDTH + 5);

    // Main vertical stacking layout for our component
    QVBoxLayout *windowLayout = new QVBoxLayout(this);
    windowLayout->setContentsMargins(0, 0, 0, 0);
    windowLayout->setSpacing(0);

    // ==========================================
    // 1. TOP DASHBOARD: TEXT BOX & STRATEGY PANEL
    // ==========================================
    QWidget *topDashboard = new QWidget(this);
    topDashboard->setStyleSheet("background-color: #111827; border-bottom: 3px solid #D4AF37;");
    QHBoxLayout *dashboardLayout = new QHBoxLayout(topDashboard);
    dashboardLayout->setContentsMargins(20, 15, 20, 15);

    // Left Column: Intelligence Text Feed
    scoutingTextLabel = new QLabel("Reviewing opposing layout...", topDashboard);
    scoutingTextLabel->setStyleSheet("font-size: 15px; color: #E2E8F0; font-family: 'Arial'; line-height: 130%;");
    scoutingTextLabel->setWordWrap(true);
    dashboardLayout->addWidget(scoutingTextLabel, 3);

    // Right Column: Tactical Stance Chooser Panel
    strategyContainer = new QWidget(topDashboard);
    QHBoxLayout *strategyLayout = new QHBoxLayout(strategyContainer);
    strategyLayout->setContentsMargins(0, 0, 0, 0);
    strategyLayout->setSpacing(10);

    QPushButton *defensiveBtn = new QPushButton("🛡️ Defensive Stance", strategyContainer);
    QPushButton *balancedBtn = new QPushButton("⚖️ Balanced Match", strategyContainer);
    QPushButton *aggressiveBtn = new QPushButton("⚔️ Aggressive Charge", strategyContainer);

    // 🌟 THE VISUAL FIX: Localized Stance Highlighting Function
    auto updateButtonVisuals = [defensiveBtn, balancedBtn, aggressiveBtn](int selectedStance)
    {
        QString baseStyle = "padding: 10px 15px; font-weight: bold; border-radius: 4px; color: white; font-size: 13px;";

        // 1. Reset all buttons to default dark/inactive states
        defensiveBtn->setStyleSheet(baseStyle + "background-color: #1E3A8A; border: 2px solid transparent;");
        balancedBtn->setStyleSheet(baseStyle + "background-color: #374151; border: 2px solid transparent;");
        aggressiveBtn->setStyleSheet(baseStyle + "background-color: #991B1B; border: 2px solid transparent;");

        // 2. Brighten the active stance and apply a royal gold frame
        if (selectedStance == 0)
        {
            defensiveBtn->setStyleSheet(baseStyle + "background-color: #2563EB; border: 2px solid #D4AF37;");
        }
        else if (selectedStance == 1)
        {
            balancedBtn->setStyleSheet(baseStyle + "background-color: #4B5563; border: 2px solid #D4AF37;");
        }
        else if (selectedStance == 2)
        {
            aggressiveBtn->setStyleSheet(baseStyle + "background-color: #DC2626; border: 2px solid #D4AF37;");
        }
    };

    // Initialize with "Balanced Match" selected by default on startup
    updateButtonVisuals(1);

    // Wire up button actions to update visuals alongside emitting signals
    connect(defensiveBtn, &QPushButton::clicked, this, [this, updateButtonVisuals]()
            { 
        updateButtonVisuals(0);
        emit strategySelected(0); });

    connect(balancedBtn, &QPushButton::clicked, this, [this, updateButtonVisuals]()
            { 
        updateButtonVisuals(1);
        emit strategySelected(1); });

    connect(aggressiveBtn, &QPushButton::clicked, this, [this, updateButtonVisuals]()
            { 
        updateButtonVisuals(2);
        emit strategySelected(2); });

    strategyLayout->addWidget(defensiveBtn);
    strategyLayout->addWidget(balancedBtn);
    strategyLayout->addWidget(aggressiveBtn);

    dashboardLayout->addWidget(strategyContainer, 2);
    windowLayout->addWidget(topDashboard);

    // ==========================================
    // 2. BOTTOM CANVAS: THE SCENE DRAWER
    // ==========================================
    graphicsView = new QGraphicsView(this);
    graphicsView->setFixedSize(SCENE_WIDTH + 5, SCENE_HEIGHT + 5);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scene = new QGraphicsScene(0, 0, SCENE_WIDTH, SCENE_HEIGHT, this);
    scene->setSceneRect(0, 0, SCENE_WIDTH, SCENE_HEIGHT);
    graphicsView->setScene(scene);
    windowLayout->addWidget(graphicsView);

    // --- SPRITES & BACKGROUND INITIALIZATION ---
    QPixmap bgPixmap(":/assets/Backgroundjoust.png");
    graphicsView->setBackgroundBrush(bgPixmap.isNull() ? QBrush(QColor("#4A5568")) : QBrush(bgPixmap.scaled(SCENE_WIDTH, SCENE_HEIGHT)));

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
    if (!fence.isNull())
        tiltBarrier->setPixmap(fence.scaled(SCENE_WIDTH, 43));
    tiltBarrier->setPos(0, 222);
    tiltBarrier->setZValue(2);
}

void JoustView::setScoutingReport(const QString &opponentName, const QString &traitAdvice)
{
    QString formattedText = QString("<b style='color:#D4AF37; font-size:16px;'>OPPONENT: %1</b><br/>"
                                    "<span style='color:#CBD5E1;'>Scouting Intelligence: %2</span>")
                                .arg(opponentName, traitAdvice);
    scoutingTextLabel->setText(formattedText);
}

void JoustView::updateRenderState(double pX, double eX, int charge, int frameIndex,
                                  JoustState state, const QString &announcer, const QString &qte)
{
    // Toggle interactive UI state vs cinematic presentation state
    if (state == JoustState::WaitingToStart)
    {
        strategyContainer->show();
    }
    else
    {
        strategyContainer->hide(); // Lock commands once horses begin running down the list lanes

        // Update dashboard text dynamically to act as a running feed during execution
        if (!qte.isEmpty())
        {
            scoutingTextLabel->setText(QString("<b style='color:#EF4444;'>BRACE FOR IMPACT!</b> Press Key matching: <b>%1</b>").arg(qte));
        }
        else if (!announcer.isEmpty())
        {
            scoutingTextLabel->setText(QString("<b style='color:#34D399;'>HERALD:</b> <i>%1</i>").arg(announcer));
        }
    }

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

    if (state == JoustState::Passing && charge == 3)
    {
        enemySprite->setRotation(90);
    }
    else
    {
        enemySprite->setRotation(0);
    }
}

// Redirect view focusing clicks downwards internally
void JoustView::mousePressEvent(QMouseEvent *)
{
    setFocus();
    emit actionTriggered();
}

void JoustView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space)
    {
        emit actionTriggered();
    }
    else
    {
        emit inputKeyPressed(event->key());
    }
}

void JoustView::closeEvent(QCloseEvent *)
{
    emit viewClosed();
}

QPixmap JoustView::getHorseFrame(int colorIdx, ViewDirection direction, int frameNum)
{
    if (colorIdx < 0 || colorIdx > 3 || horseSheets[colorIdx].isNull())
        return QPixmap(84, 46);
    const int FRAME_WIDTH = 60;
    const int FRAME_HEIGHT = 33;
    QPixmap frame = horseSheets[colorIdx].copy(frameNum * FRAME_WIDTH, 0, FRAME_WIDTH, FRAME_HEIGHT);
    qreal scaleX = (direction == ViewDirection::Right) ? -1.7 : 1.7;
    qreal scaleY = 1.7;
    return frame.transformed(QTransform().scale(scaleX, scaleY), Qt::SmoothTransformation);
}

QPixmap JoustView::getKnightPixmap(ViewDirection direction, int frameIndex)
{
    if (knightPixmap.isNull())
        return QPixmap(60, 133);
    const int KNIGHT_FRAME_WIDTH = 350;
    const int KNIGHT_FRAME_HEIGHT = 833;
    int atlasFrameNum = (frameIndex / 3) % 2;
    QPixmap frame = knightPixmap.copy(atlasFrameNum * KNIGHT_FRAME_WIDTH, 0, KNIGHT_FRAME_WIDTH, KNIGHT_FRAME_HEIGHT);
    qreal downScaleY = 0.08;
    qreal scaleX = (direction == ViewDirection::Left) ? -0.08 : 0.08;
    return frame.transformed(QTransform().scale(scaleX, downScaleY), Qt::SmoothTransformation);
}