#ifndef KNIGHTROSTERTABCONTROLLER_H
#define KNIGHTROSTERTABCONTROLLER_H

#pragma once

#include <QObject>
#include <QWidget>

#include "model/knights/KnightRosterTabHandler.h"
#include "view/knights/KnightRosterView.h"

class KnightRosterTabController : public QObject
{
    Q_OBJECT
public:
    KnightRosterTabController(QWidget *parent = nullptr);

    KnightRosterView* getTab() { return m_knightRosterView; }

    void startDay();

private:
    KnightRosterView *m_knightRosterView;
    // KnightRosterTabHandler *m_knightRosterTabHandler;
};

#endif