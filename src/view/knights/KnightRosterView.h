#ifndef KNIGHTROSTERVIEW_H
#define KNIGHTROSTERVIEW_H

#pragma once

#include <QWidget>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class KnightRosterView : public QWidget
{
Q_OBJECT // Don't forget to include the macro if you intend to add buttons/signals later!
public: 
    explicit KnightRosterView(QWidget *parent = nullptr);

    void populateRoster();

private:
    QVBoxLayout *rosterListLayout;
};

#endif