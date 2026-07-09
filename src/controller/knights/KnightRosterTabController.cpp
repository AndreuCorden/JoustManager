#include "controller/knights/KnightRosterTabController.h"

KnightRosterTabController::KnightRosterTabController(Player &player, QWidget *parent)
//: m_knightRosterTabHandler(new KnightRosterTabHandler())
{
    m_knightRosterView = new KnightRosterView(player, parent);
}

void KnightRosterTabController::startDay()
{
    m_knightRosterView->populateRoster();
}