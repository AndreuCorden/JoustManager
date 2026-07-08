#ifndef TOURNAMENTTABCONTROLLER_H
#define TOURNAMENTTABCONTROLLER_H

#include <QObject>

#include "model/tournaments/TournamentTabHandler.h"
#include "view/tournaments/TournamentView.h"

#include <vector>

class TournamentTabController : public QObject
{
    Q_OBJECT
public:
    explicit TournamentTabController(QWidget *parent, std::vector<Tournament> todaysTournaments);
    TournamentView *getTab() { return m_tournamentView; };

    void setDailyTournaments(std::vector<Tournament> todaysTournaments);

    void runTournaments() { runNextRegisteredTournament(0); }

signals:
    void endOfJousting();

private slots:
    void handleRegistration(size_t index, const std::vector<Knight> &knights);
    void handleCancellation(size_t index);

private:
    void runNextRegisteredTournament(size_t currentIndex);

    TournamentTabHandler *m_tournamentTabHandler;
    TournamentView *m_tournamentView;
};

#endif