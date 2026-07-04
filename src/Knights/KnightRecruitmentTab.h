#ifndef _Knight_Recruitment_Tab
#define _Knight_Recruitment_Tab

#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <list>
#include "Knight.h"

class KnightRecruitmentTab : public QWidget
{
    Q_OBJECT

public:
    KnightRecruitmentTab(QWidget *parent = nullptr);

    void populateList();

private:

    std::list<Knight> recruitableKnights;
    QVBoxLayout *listLayout;
};

#endif