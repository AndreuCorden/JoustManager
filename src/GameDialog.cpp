#include "GameDialog.h"

GameDialog::GameDialog(QWidget *parent) : QDialog(parent) {
    // 1. Force the frameless window hint on EVERY dialog that uses this base
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setModal(true);

    // 2. Apply a uniform base layout with margins
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(25, 25, 25, 25);
    mainLayout->setSpacing(15);

    // 3. Apply the global master dialog style
    this->setStyleSheet(
        "QDialog {"
        "   background-color: #1A202C;"
        "   border: 3px solid #D4AF37;"
        "   border-radius: 8px;"
        "}"
    );
}