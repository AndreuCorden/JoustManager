#ifndef _Knight_Detail_Dialog
#define _Knight_Detail_Dialog

#pragma once
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include "Knight.h" // Replace with your actual Knight class header
#include "GameDialog.h"


class KnightDetailDialog : public GameDialog {
    Q_OBJECT

public:
    // Pass the knight by reference so modifications (like changing gear) save back to the roster
    KnightDetailDialog(Knight &knight, QWidget *parent = nullptr);
};

#endif