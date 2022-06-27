#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QSplashScreen>
#include <QThread>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "CookiesGames_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    // loading splashscreen (2 secondes)
    QPixmap pixmap(":/images/images/loading_splash_screen.png");
    QSplashScreen splashscreen(pixmap);
    splashscreen.show();
    QThread::sleep(2);

    MainWindow w;
    w.show();
    splashscreen.finish(&w);

    return a.exec();
}
