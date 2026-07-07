#include "controller/knights/KnightRosterTabController.h"

KnightRosterTabController::KnightRosterTabController(QWidget *parent)
//: m_knightRosterTabHandler(new KnightRosterTabHandler())
{
    m_knightRosterView = new KnightRosterView(parent);
}

void KnightRosterTabController::startDay()
{
    m_knightRosterView->populateRoster();
}