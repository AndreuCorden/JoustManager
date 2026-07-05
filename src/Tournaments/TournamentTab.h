#ifndef _Tournament_Tab
#define _Tournament_Tab

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class TournamentTab : public QWidget
{
    Q_OBJECT
public:
    explicit TournamentTab(QWidget *parent = nullptr);

    void populateRoster();

private:
    QVBoxLayout *tournamentListLayout;
};

#endif