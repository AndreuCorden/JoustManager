#include "controller/knights/KnightRecruitmentTabController.h"

KnightRecruitmentTabController::KnightRecruitmentTabController(QWidget *parent)
: m_KnightRecruitmentTabHandler(new KnightRecruitmentTabHandler())
{
    m_KnightRecruitmentView = new KnightRecruitmentView(parent);
    m_KnightRecruitmentView->populateList(m_KnightRecruitmentTabHandler->getBuyableKnight());
    
    connect(m_KnightRecruitmentView, &KnightRecruitmentView::knightRecruited, this, &KnightRecruitmentTabController::handleKnightRecruitment);
}

void KnightRecruitmentTabController::handleKnightRecruitment(const Knight &knight)
{
    // 1. Remove the item from your backend handler state
    m_KnightRecruitmentTabHandler->sellKnight(knight);

    // 2. Force the view to repaint and sync with the clean backend data list
    m_KnightRecruitmentView->populateList(m_KnightRecruitmentTabHandler->getBuyableKnight());
}

void KnightRecruitmentTabController::startDay()
{
    m_KnightRecruitmentTabHandler->startDay();
    m_KnightRecruitmentView->populateList(m_KnightRecruitmentTabHandler->getBuyableKnight());
}