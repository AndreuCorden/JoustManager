#ifndef _Knight_Roster_Tab
#define _Knight_Roster_Tab

#pragma once

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>


class KnightRosterTab : public QWidget
{
    Q_OBJECT // Don't forget to include the macro if you intend to add buttons/signals later!
public:
    explicit KnightRosterTab(QWidget *parent = nullptr);
private:
};

#endif