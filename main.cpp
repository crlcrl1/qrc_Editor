#include "mainwindow.h"

#include <QApplication>
#include <QString>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
#ifdef _WIN32
    a.setStyle("fusion");
#endif
    if (argc > 1) {
        const QString qrcFile = argv[1];
        MainWindow w(nullptr, qrcFile);
        w.show();
        return QApplication::exec();
    }
    MainWindow w;
    w.show();
    return QApplication::exec();
}
