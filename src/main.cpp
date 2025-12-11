#include "QApplication"
#include "ui/MainWindow.h"
#include "utils/RandomGenerator.h"
#include "utils/TypeWriter.h"
#include "utils/TextGenerator.h"
#include "ui/RiddleDialog.h"
#include "ui/NotesDialog.h"          
#include <QLocale>
#include <QCoreApplication>
#include <QFontDatabase>
#include <QDebug>

int main(int argc, char *argv[]) {
    QLocale::setDefault(QLocale(QLocale::Russian, QLocale::Russia));
    QApplication app(argc, argv);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#endif
    RandomGenerator::initializeSeed();

    // Load pixel font
    int fontId = QFontDatabase::addApplicationFont(":/assets/fonts/PressStart2P-Regular.ttf");
    if (fontId == -1) {
        qWarning() << "Failed to load font PressStart2P-Regular.ttf";
    } else {
        QString fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
        QFont font(fontFamily, 12);
        font.setStyleStrategy(QFont::PreferAntialias);
        app.setFont(font);
        qDebug() << "Successfully loaded font:" << fontFamily;
    }

    QLocale::setDefault(QLocale(QLocale::Russian, QLocale::Russia));
    QCoreApplication::setAttribute(Qt::AA_Use96Dpi, true); // опционально

    MainWindow window;
    window.show();

    return app.exec();
}
