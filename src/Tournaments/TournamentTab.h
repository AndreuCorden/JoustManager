#ifndef TOURNAMENTTAB_H
#define TOURNAMENTTAB_H

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "GameTimelineController.h"

class TournamentTab : public QWidget
{
    Q_OBJECT
public:
    explicit TournamentTab(GameTimelineController &gameTimelineController, QWidget *parent = nullptr);

    void populateRoster();

private:
    GameTimelineController &m_gameTimelineController;

    QVBoxLayout *m_tournamentListLayout;
};

#endif