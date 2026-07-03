#ifndef _Game_Dialog
#define _Game_Dialog

#pragma once
#include <QDialog>
#include <QVBoxLayout>

class GameDialog : public QDialog {
    Q_OBJECT
protected:
    QVBoxLayout *mainLayout; // Expose the layout so child windows can add items to it

public:
    GameDialog(QWidget *parent = nullptr);
};

#endif