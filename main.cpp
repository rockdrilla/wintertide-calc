#include <QApplication>

#include "mainwindow.h"

#include "appinfo.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QCoreApplication::setApplicationName(APP_TITLE);
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QCoreApplication::setOrganizationName(APP_AUTHOR);

    MainWindow w;
    w.show();

    return a.exec();
}
