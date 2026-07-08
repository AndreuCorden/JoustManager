#ifndef KNIGHTDETAILDIALOG_H
#define KNIGHTDETAILDIALOG_H

#pragma once
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include "model/knights/Knight.h"
#include "view/knights/KnightRosterView.h"
#include "view/knights/KnightRecruitmentView.h"
#include "GameDialog.h"


class KnightDetailDialog : public GameDialog {
    Q_OBJECT

public:
    // Pass the knight by reference so modifications (like changing gear) save back to the roster
    KnightDetailDialog(Knight &knight, KnightRosterView *parent = nullptr);

    KnightDetailDialog(const Knight &knight, KnightRecruitmentView *parent = nullptr);

private:
    QWidget *createEquipmentSlot(Knight &knight, Item::ItemType type);
};

#endif