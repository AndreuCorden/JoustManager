#include "controller/tournaments/TournamentTabController.h"
#include "view/tournaments/TournamentRunnerDialog.h"
#include "Player.h"

TournamentTabController::TournamentTabController(QWidget *parent, std::vector<Tournament> todaysTournaments)
    : m_tournamentTabHandler(new TournamentTabHandler(todaysTournaments))
{
    m_tournamentView = new TournamentView(parent);

    // 🌟 Establish our MVC communication pipes via QObject namespaces
    QObject::connect(m_tournamentView, &TournamentView::registrationRequested, 
                     this, &TournamentTabController::handleRegistration);
                     
    QObject::connect(m_tournamentView, &TournamentView::cancellationRequested, 
                     this, &TournamentTabController::handleCancellation);

    // Initial load setup
    // Call GameTimelineController to recieve first day tournaments;
    //m_tournamentTabHandler->generateTournamentsForDay();
    m_tournamentView->populateRoster(m_tournamentTabHandler->getAvailableTournaments());
}

void TournamentTabController::handleRegistration(size_t index, const std::vector<Knight>& knights)
{
    // 1. Process mutations exclusively through Model bounds
    m_tournamentTabHandler->registerSquadForTournament(index, knights);

    // 2. Refresh the display layout with clean backend data references
    m_tournamentView->populateRoster(m_tournamentTabHandler->getAvailableTournaments());
}

void TournamentTabController::handleCancellation(size_t index)
{
    m_tournamentTabHandler->cancelRegistration(index);
    m_tournamentView->populateRoster(m_tournamentTabHandler->getAvailableTournaments());
}

void TournamentTabController::setDailyTournaments(std::vector<Tournament> todaysTournaments)
{
    m_tournamentView->populateRoster(todaysTournaments);
    m_tournamentTabHandler->setTodaysTournaments(std::move(todaysTournaments));
}

void TournamentTabController::runNextRegisteredTournament(size_t currentIndex)
{
    // 1. Ask the model for the next valid tournament index the player signed up for
    int nextIndex = m_tournamentTabHandler->getNextRegisteredTournamentIndex(currentIndex);

    // BASE CASE: No more active tourneys to process
    if (nextIndex == -1)
    {
        // Flush daily tracks and clear visual states
        m_tournamentTabHandler->getAvailableTournaments().clear();
        m_tournamentView->populateRoster(m_tournamentTabHandler->getAvailableTournaments());
        return;
    }

    // 2. Fetch data references safely from our model boundaries
    std::vector<Tournament>& tourneys = m_tournamentTabHandler->getAvailableTournaments();
    Tournament& activeMatch = tourneys[static_cast<size_t>(nextIndex)];

    // 3. Controller constructs the View component
    TournamentRunnerDialog *runner = new TournamentRunnerDialog(activeMatch, nullptr);
    
    // 4. Handle closure logic hooks
    QObject::connect(runner, &QDialog::accepted, this, [this, nextIndex, runner]() {
        
        // Apply reward states using backend controllers/singletons
        std::vector<Tournament>& liveTourneys = m_tournamentTabHandler->getAvailableTournaments();
        int rewardAmount = liveTourneys[static_cast<size_t>(nextIndex)].getReward();
        Player::getInstance().modifyGold(rewardAmount); 
        
        runner->deleteLater();

        // RECURSION STEP: Look for the next registered match starting at the next index position
        this->runNextRegisteredTournament(static_cast<size_t>(nextIndex + 1)); 
    });

    // Execute the window block modally
    runner->exec(); 
}