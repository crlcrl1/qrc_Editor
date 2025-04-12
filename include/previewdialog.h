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
    explicit PreviewDialog(QWidget *parent = nullptr, const QString &file = "");
    ~PreviewDialog() override;

private:
    Ui::PreviewDialog *ui;

    QImage *image;

    static bool isTextFile(const QString &filePath);
    static bool isImageFile(const QString &filePath);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
};
#endif // PREVIEW_DIALOG_H
