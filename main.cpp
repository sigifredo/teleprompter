// Own
#include "MainWindow.hpp"

// Qt
#include <QApplication>
#include <QLocale>
#include <QScreen>
#include <QTranslator>

QRect centerGeometry();

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();

    for (const QString &locale : uiLanguages)
    {
        const QString baseName = "teleprompter_" + QLocale(locale).name();

        if (translator.load(":/i18n/" + baseName))
        {
            app.installTranslator(&translator);
            break;
        }
    }

    MainWindow mw;
    mw.setGeometry(centerGeometry());
    mw.show();

    return app.exec();
}

QRect centerGeometry()
{
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();

    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    int windowWidth = screenWidth * 0.3;
    int windowHeight = screenHeight / 2;

    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    return QRect(posX, posY, windowWidth, windowHeight);
}
