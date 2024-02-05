#include "qrc_prefix.h"

QrcPrefix::QrcPrefix() {
}

QrcPrefix::QrcPrefix(const QString &prefix) {
    this->prefix = prefix;
}

QrcPrefix::~QrcPrefix() {
}

QString QrcPrefix::getPrefix() const {
    return this->prefix;
}

void QrcPrefix::setPrefix(const QString &value) {
    this->prefix = value;
}

const QList<QrcFile> &QrcPrefix::getFiles() const {
    return this->files;
}

void QrcPrefix::setFiles(const QList<QrcFile> &value) {
    this->files = value;
}

void QrcPrefix::addFile(const QrcFile &file) {
    this->files.append(file);
}

void QrcPrefix::addFile(const QString &filePath, const QString &alias, bool empty) {
    QrcFile file(filePath, alias, empty);
    this->files.append(file);
}
