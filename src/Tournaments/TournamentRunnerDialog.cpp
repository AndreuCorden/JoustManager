#include "TournamentRunnerDialog.h"
#include "Tournaments/Joust/JoustView.h"
#include "Player.h"

TournamentRunnerDialog::TournamentRunnerDialog(Tournament &tournament, QWidget *parent)
    : GameDialog(parent), activeTournament(tournament)
{
    this->setMinimumSize(800, 600);
    this->resize(800, 600);

    if (this->mainLayout)
    {
        this->mainLayout->setAlignment(Qt::AlignCenter);
        this->mainLayout->setSpacing(20);
    }

    // Load Stage 1: The Announcement Screen safely
    setupIntroScreen();
}

void TournamentRunnerDialog::setupIntroScreen()
{
    clearCurrentLayout();

    // Ensure mainLayout is valid before attaching widgets
    if (!mainLayout)
        return;

    titleLabel = new QLabel(QString::fromStdString(activeTournament.getName()), this);
    titleLabel->setStyleSheet("font-size: 28px; font-weight: bold; color: #F6AD55; background: transparent;");
    titleLabel->setAlignment(Qt::AlignCenter);

    infoLabel = new QLabel(QString("Hosted by: %1\n\nPrepare your knights for battle!").arg(QString::fromStdString(activeTournament.getHost())), this);
    infoLabel->setStyleSheet("font-size: 16px; color: #E2E8F0; font-style: italic; background: transparent;");
    infoLabel->setAlignment(Qt::AlignCenter);

    actionButton = new QPushButton("Enter the Arena", this);
    actionButton->setStyleSheet("background-color: #48BB78; color: white; font-size: 16px; font-weight: bold; padding: 12px 30px; border-radius: 6px;");
    actionButton->setCursor(Qt::PointingHandCursor);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(infoLabel);
    mainLayout->addWidget(actionButton);

    connect(actionButton, &QPushButton::clicked, this, &TournamentRunnerDialog::startNextRoundLifecycle);
}

void TournamentRunnerDialog::startNextRoundLifecycle()
{
    // 🌟 BASE CASE: Check if matches exist first. If none, wrap up cleanly.
    std::vector<MatchUp> currentMatches = activeTournament.generateCurrentRoundMatches();
    if (currentMatches.empty())
    {
        this->accept();
        return;
    }

    std::vector<std::vector<Knight>> roundWinners;
    bool playerMatchFound = false;
    MatchUp playerMatch;

    for (const MatchUp &match : currentMatches)
    {
        if (match.involvesPlayer && !playerMatchFound)
        {
            playerMatch = match;
            playerMatchFound = true;
        }
        else
        {
            roundWinners.push_back(match.teamA); // AI auto-advancements
        }
    }

    // 🎥 STATE A: Player is fighting!
    if (playerMatchFound)
    {

        // 🌟 STEP 1: Close the intro dialog completely to get it out of the way
        this->accept();

        // 🌟 STEP 2: Spawn JoustView with NO parent (nullptr).
        // This guarantees it runs completely independently at its own 1205x405 size.
        JoustView *joustArena = new JoustView(playerMatch.teamA, playerMatch.teamB, nullptr);

        // We set the WindowModal flag so it still blocks background inputs nicely
        joustArena->setWindowFlags(Qt::FramelessWindowHint);
        joustArena->setWindowModality(Qt::ApplicationModal);

        QScreen *primaryScreen = QGuiApplication::primaryScreen();
        if (primaryScreen)
        {
            QRect screenGeometry = primaryScreen->geometry();

            // Calculate the exact center position
            int x = (screenGeometry.width() - joustArena->width()) / 2;
            int y = (screenGeometry.height() - joustArena->height()) / 2;

            // Explicitly lock the coordinates onto the window frame before showing it
            joustArena->move(x, y);
        }
        joustArena->show();
        joustArena->raise();
        joustArena->activateWindow();

        // 🌟 STEP 3: Connect to the arena lifecycle from your global controller scope
        // When the player clicks to close the match, proceed to the next logic loop step.
        // (Note: Since your JoustView calls parentWidget() -> accept(), we handle destruction cleanly)
        QObject::connect(joustArena, &QWidget::destroyed, [roundWinners, playerMatch, this]() mutable
                         {
            // Assume player won for logic progression
            roundWinners.push_back(playerMatch.teamA); 
            this->activeTournament.advanceTournamentRound(roundWinners); });
    }
    // 💤 STATE B: Player isn't in this round. Auto-simulate.
    else
    {
        activeTournament.advanceTournamentRound(roundWinners);
        startNextRoundLifecycle(); // Check next round
    }
}

void TournamentRunnerDialog::clearCurrentLayout()
{
    // 1. Explicitly clean up your known text elements and buttons
    if (titleLabel)
    {
        titleLabel->deleteLater();
        titleLabel = nullptr;
    }
    if (infoLabel)
    {
        infoLabel->deleteLater();
        infoLabel = nullptr;
    }
    if (actionButton)
    {
        actionButton->deleteLater();
        actionButton = nullptr;
    }

    // 🌟 FIX: Safely scan and delete ONLY child JoustView widgets without wiping out layout matrices
    // This looks for any JoustView child instance currently inside this window block.
    JoustView *oldArena = this->findChild<JoustView *>();
    if (oldArena)
    {
        oldArena->deleteLater();
    }
}