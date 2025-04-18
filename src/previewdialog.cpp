#include "previewdialog.h"
#include "./ui_previewdialog.h"

#include <QFileInfo>
#include <QImage>
#include <QImageReader>
#include <QMessageBox>
#include <QTextStream>
#include <qnamespace.h>

PreviewDialog::PreviewDialog(QWidget *parent, const QString &file)
    : QDialog(parent), ui(new Ui::PreviewDialog) {
    ui->setupUi(this);
    setWindowTitle(tr("Preview - %1").arg(file));
    setAttribute(Qt::WA_DeleteOnClose);
    if (!QFileInfo::exists(file)) {
        close();
    }

    if (isImageFile(file)) {
        ui->stackedWidget->setCurrentIndex(0);
        image = new QImage();
        image->load(file);
    } else if (isTextFile(file)) {
        image = nullptr;
        ui->stackedWidget->setCurrentIndex(1);
        QFile fileObj(file);
        if (fileObj.open(QIODevice::ReadOnly)) {
            QTextStream in(&fileObj);
            in.setAutoDetectUnicode(true);
            ui->textEdit->setPlainText(in.readAll());
            ui->textEdit->setReadOnly(true);
            fileObj.close();
        }
    } else {
        image = nullptr;
        QMessageBox::warning(this, tr("Preview"), tr("The file is not supported"));
        this->close();
    }
    setMinimumSize(400, 300);
}

PreviewDialog::~PreviewDialog() {
    delete ui;
    delete image;
}

bool PreviewDialog::isTextFile(const QString &filePath) {
    if (isImageFile(filePath)) {
        return false;
    }
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream in(&file);
        in.setAutoDetectUnicode(true);
        int lineCount = 0;
        while (!in.atEnd()) {
            QString line = in.readLine();
            lineCount++;
            if (lineCount > 50) {
                break;
            }
            for (const QChar &ch : line) {
                if (!ch.isPrint() && !ch.isSpace() && !ch.isPunct()) {
                    return false;
                }
            }
        }
        file.close();
        return true;
    }
    return false;
}

bool PreviewDialog::isImageFile(const QString &filePath) {
    return QImageReader::supportedImageFormats().contains(
        QFileInfo(filePath).suffix().toLower().toLatin1());
}

void PreviewDialog::resizeEvent(QResizeEvent *event) {
    QDialog::resizeEvent(event);
    if (image != nullptr) {
        QImage scaledImage = image->scaled(ui->imageLabel->size(), Qt::KeepAspectRatio);
        ui->imageLabel->setPixmap(QPixmap::fromImage(scaledImage));
    }
}

void PreviewDialog::showEvent(QShowEvent *event) {
    QDialog::showEvent(event);
    if (image != nullptr) {
        QImage scaledImage = image->scaled(ui->imageLabel->size(), Qt::KeepAspectRatio);
        ui->imageLabel->setPixmap(QPixmap::fromImage(scaledImage));
    }
}
