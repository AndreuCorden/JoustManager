#ifndef _Knight_Recruitment_Tab
#define _Knight_Recruitment_Tab

#pragma once

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include <list>
#include "Knights/Knight.h"

class KnightRecruitmentTab : public QWidget
{
    Q_OBJECT // Don't forget to include the macro if you intend to add buttons/signals later!
public:
    explicit KnightRecruitmentTab(QWidget *parent = nullptr);
private:
    std::list<Knight> recruitableKnights;
};

#endif