#include "qrc_file.h"

QrcFile::QrcFile(const QString &filePath, const QString &alias, bool empty) {
    this->filePath = filePath;
    this->alias    = alias;
    this->empty    = empty;
}

QrcFile::~QrcFile() = default;

QString QrcFile::getFilePath() const {
    return this->filePath;
}

QString QrcFile::getAlias() const {
    return this->alias;
}

void QrcFile::setAlias(const QString &alias) {
    this->alias = alias;
}

bool QrcFile::isEmpty() const {
    return this->empty;
}

void QrcFile::setEmpty(bool empty) {
    this->empty = empty;
}
