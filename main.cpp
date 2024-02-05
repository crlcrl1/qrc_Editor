#include "mainwindow.h"

#include <QApplication>
#include <QString>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setStyle("fusion");
    if (argc > 1) {
        QString qrcFile = argv[1];
        MainWindow w(nullptr, qrcFile);
        w.show();
        return a.exec();
    }
    MainWindow w;
    w.show();
    return a.exec();
}
