#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>
#include <QStandardItemModel>

#include "qrc_parser.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, const QString &qrcFile = "");
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QString qrcFile;
    QStandardItemModel *qrcModel;
    QrcParser *qrcParser;
    QList<QrcPrefix> qrcPrefixes;

    bool saved;

    void loadQrcFile(const QString &fileName);

public slots:
    void do_actionOpen_triggered();
    void do_actionSave_triggered();
    void do_actionNew_triggered();
    void do_actionExit_triggered();

    void do_qrcModel_dataChanged();
    void do_qrcTreeView_menu(const QPoint &pos);

    void do_removePrefixAction_triggered();
    void do_addPrefixAction_triggered();
    void do_addFileAction_triggered();
    void do_removeFileAction_triggered();

    void do_actionPreview_triggered();

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
