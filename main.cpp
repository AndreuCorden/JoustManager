#include <QApplication>
#include "GameWindow.h"

int main(int argc, char *argv[])
{
    // QApplication initializes operating system display loops and hooks
    QApplication app(argc, argv);

    app.setStyleSheet(
        // Main Windows
        "QMainWindow {"
        "   background-color: #1A202C;"
        "   border: 4px solid #D4AF37;"
        "}"

        // --- Core UI Tabs ---
        "QTabWidget::pane { border: 2px solid #4A5568; background-color: #1A202C; top: -2px; }"
        "QTabBar::tab {"
        "   background: #2D3748; color: #A0AEC0; padding: 12px 24px;"
        "   font-family: 'Georgia', serif; font-weight: bold;"
        "   border: 1px solid #4A5568; border-bottom: none;"
        "   border-top-left-radius: 4px; border-top-right-radius: 4px;"
        "}"
        "QTabBar::tab:selected { background: #1A202C; color: #D4AF37; border-bottom: 2px solid #1A202C; }"

        // --- Global Popups & Inputs ---
        "QInputDialog { background-color: #1A202C; color: white; }"
        "QComboBox { background-color: #2D3748; color: white; padding: 5px; border: 1px solid #4A5568; }"
        "QAbstractItemView { background-color: #2D3748; color: white; selection-background-color: #D4AF37; }"

        // --- NEW: Universal Game Dialog Style ---
        // This targets GameDialog and any class inheriting from it (like KnightDetailDialog)
        "GameDialog {"
        "   background-color: #1A202C;"
        "   border: 3px solid #D4AF37;"
        "   border-radius: 8px;"
        "}"

        "QInputDialog, QMessageBox {"
        "   background-color: #1A202C;"
        "   border: 2px solid #D4AF37;"
        "}"
        
        /*"QPushButton {"
        "   background-color: #48BB78; color: white; font-size: 16px; font-weight: bold;"
        "   padding: 12px 30px; border-radius: 6px; border: none;"
        "}"
        "QPushButton:hover { background-color: #38A169; }"*/
    );

    GameWindow window;
    window.show();

    // Hands control over to the background rendering pipeline execution
    return app.exec();
}