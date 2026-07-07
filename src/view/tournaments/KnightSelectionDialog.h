// Tournaments/KnightSelectionDialog.h
#ifndef KNIGHTSELECTIONDIALOG_H
#define KNIGHTSELECTIONDIALOG_H

#pragma once

#include <QObject>
#include <QCheckBox>
#include <QMessageBox>

#include "GameDialog.h"
#include "model/tournaments/Tournament.h"

class KnightSelectionDialog : public GameDialog
{
    Q_OBJECT

public:
    explicit KnightSelectionDialog(const Tournament &tournament, QWidget *parent = nullptr);

    std::vector<Knight> getSelectedKnights() const { return m_selectedKnights; }

private:
    int m_requiredTeammates;
    std::vector<std::pair<Knight *, QCheckBox *>> checkboxRegistry;
    std::vector<Knight> m_selectedKnights;
};

#endif