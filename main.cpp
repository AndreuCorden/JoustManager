#include <QApplication>
#include "GameWindow.h"

int main(int argc, char *argv[]) {
    // QApplication initializes operating system display loops and hooks
    QApplication app(argc, argv);

    GameWindow window;
    window.show();

    // Hands control over to the background rendering pipeline execution
    return app.exec();
}