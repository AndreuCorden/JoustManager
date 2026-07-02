#include "KnightDetailDialog.h"

KnightDetailDialog::KnightDetailDialog(Knight &knight, QWidget *parent) : QDialog(parent) {
    // Set up window properties
    setWindowTitle(QString("%1's Profile").arg(QString::fromStdString(knight.getName())));
    setMinimumSize(350, 450);
    setModal(true); // Prevents interacting with the main menu until closed

    QVBoxLayout *layout = new QVBoxLayout(this);

    // Header
    QLabel *titleLabel = new QLabel(QString::fromStdString(knight.getName()), this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #D4AF37;");
    layout->addWidget(titleLabel);

    // Display Stats (Example)
    QLabel *statsLabel = new QLabel("Stats & Equipment placeholder...", this);
    statsLabel->setStyleSheet("font-size: 14px; color: #4A5568;");
    layout->addWidget(statsLabel);

    // Add spacing
    layout->addStretch();

    // Close Button
    QPushButton *closeButton = new QPushButton("Back to Roster", this);
    closeButton->setStyleSheet("padding: 10px; font-weight: bold;");
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(closeButton);

    setLayout(layout);
}