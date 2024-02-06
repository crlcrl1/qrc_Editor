#ifndef PREVIEW_DIALOG_H
#define PREVIEW_DIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui {
class PreviewDialog;
}
QT_END_NAMESPACE

class PreviewDialog : public QDialog {
    Q_OBJECT

public:
    PreviewDialog(QWidget *parent = nullptr, QString file = "");
    ~PreviewDialog();

private:
    Ui::PreviewDialog *ui;

    QImage *image;

    bool isTextFile(QString filePath);
    bool isImageFile(QString filePath);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
};
#endif // PREVIEW_DIALOG_H
