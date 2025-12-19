#include <QApplication>
#include "ui/MainWindow.h"
#include "utils/RandomGenerator.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    RandomGenerator::initializeSeed();

    MainWindow window;
    window.show();

    return app.exec();
}
