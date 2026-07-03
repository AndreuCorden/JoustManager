// Tournaments/KnightSelectionDialog.h
#pragma once
#include "GameDialog.h"
#include "Tournament.h"
#include <QCheckBox>
#include <QMessageBox>

class KnightSelectionDialog : public GameDialog {
    Q_OBJECT
private:
    Tournament &currentTournament;
    std::vector<std::pair<Knight*, QCheckBox*>> checkboxRegistry;

public:
    KnightSelectionDialog(Tournament &tournament, QWidget *parent = nullptr);
};