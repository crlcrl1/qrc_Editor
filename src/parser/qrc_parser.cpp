#include "qrc_parser.h"

#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

QrcParser::QrcParser(const QString &qrcFilePath) {
    // Prepare the qrc file
    this->qrcFilePath = qrcFilePath;
    this->qrcFile.setFileName(qrcFilePath);
    this->ready = false;
    if (!QFileInfo::exists(qrcFilePath)) {
        QDomElement root = this->qrcDoc.createElement("RCC");
        this->qrcDoc.appendChild(root);
        this->ready = true;
        return;
    }

    auto qrcFileInfo = QFileInfo(qrcFilePath);
    if (!this->qrcFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Error", "Could not open " + qrcFilePath + " for reading.");
        return;
    }

    if (qrcFileInfo.size() != 0 && !this->qrcDoc.setContent(&this->qrcFile)) {
        QMessageBox::critical(nullptr, "Error",
                              "Your file " + qrcFilePath + " is not a valid qrc file.");
        this->qrcFile.close();
        return;
    }

    // If the file is empty, create a new qrc file
    if (this->qrcDoc.isNull()) {
        QDomElement root = this->qrcDoc.createElement("RCC");
        this->qrcDoc.appendChild(root);
        this->ready = true;
        if (qrcFileInfo.size() == 0) {
            this->qrcFile.close();
        }
        return;
    }

    // If the file is not a qrc file, just return
    if (this->qrcDoc.documentElement().nodeName() != "RCC") {
        QMessageBox::critical(nullptr, "Error",
                              "Your file " + qrcFilePath + " is not a valid qrc file.");
        this->qrcFile.close();
        return;
    }
    this->qrcFile.close();
    this->ready = true;
}

QrcParser::QrcParser() : ready(false) {
    this->qrcFilePath = "";
    this->qrcFile.setFileName(this->qrcFilePath);
    QDomElement root = this->qrcDoc.createElement("RCC");
    this->qrcDoc.appendChild(root);
}

QrcParser::~QrcParser() {
    if (this->qrcFile.isOpen()) {
        this->qrcFile.close();
    }
}

void QrcParser::addFile(const QString &prefix, const QString &filePath, const QString &alias,
                        bool empty) {
    QDomElement root          = this->qrcDoc.documentElement();
    QDomElement prefixElement = this->findOrCreatePrefix(root, prefix);
    QDomElement fileElement   = this->qrcDoc.createElement("file");
    if (!alias.isEmpty()) {
        fileElement.setAttribute("alias", alias);
    }
    if (empty) {
        fileElement.setAttribute("empty", "true");
    }
    QString relativePath = calculateRelativePath(this->qrcFilePath, filePath);
    fileElement.appendChild(this->qrcDoc.createTextNode(relativePath));
    prefixElement.appendChild(fileElement);
}

QDomElement QrcParser::findOrCreatePrefix(QDomElement &root, const QString &prefix) {
    QDomNodeList children = root.childNodes();
    int nodeCount         = children.count();
    for (int i = 0; i < nodeCount; i++) {
        QDomElement child = children.at(i).toElement();
        if (child.tagName() == "file") {
            continue;
        }
        if (child.tagName() == "qresource" && child.attribute("prefix") == prefix) {
            return child;
        }
    }
    QDomElement newPrefix = this->qrcDoc.createElement("qresource");
    newPrefix.setAttribute("prefix", prefix);
    root.appendChild(newPrefix);
    return newPrefix;
}

QString QrcParser::calculateRelativePath(const QString &baseFile, const QString &targetFile) {
    QFileInfo baseFileInfo(baseFile);
    QFileInfo targetFileInfo(targetFile);
    QString baseDir        = baseFileInfo.absolutePath();
    QString targetDir      = targetFileInfo.absolutePath();
    QString targetFileName = targetFileInfo.fileName();
    baseDir                = baseDir.replace("\\", "/");
    targetDir              = targetDir.replace("\\", "/");
    QStringList sepBase    = baseDir.split("/");
    QStringList sepTarget  = targetDir.split("/");
    QString relativePath   = "";

    // On windows, the first element of the split is the drive letter
#ifdef Q_OS_WIN
    if (sepBase.at(0) != sepTarget.at(0)) {
        return targetFile;
    }
#endif

    // Find the first directory that differs
    int index        = 0;
    size_t baseLen   = sepBase.size();
    size_t targetLen = sepTarget.size();
    for (; index < baseLen && index < targetLen; index++) {
        if (sepBase.at(index) != sepTarget.at(index)) {
            break;
        }
    }

    // Add the relative path
    relativePath += QString("../").repeated(baseLen - index);
    for (; index < targetLen; ++index) {
        relativePath += sepTarget.at(index) + '/';
    }
    relativePath += targetFileName;
    return relativePath;
}

void QrcParser::writeToFile() {
    if (qrcFilePath.isEmpty()) {
        return;
    }
    if (!this->qrcFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::information(nullptr, "Error",
                                 "Could not open " + this->qrcFilePath + " for writing.");
        return;
    }
    QTextStream out(&this->qrcFile);
    this->qrcDoc.save(out, 4);
    this->qrcFile.close();
}

bool QrcParser::isReady() const {
    return this->ready;
}

QList<QrcPrefix> QrcParser::getPrefixes() const {
    QDomElement root      = this->qrcDoc.documentElement();
    QDomNodeList prefixes = root.childNodes();
    int prefixesCount     = prefixes.count();
    QList<QrcPrefix> res(prefixesCount);
    for (int i = 0; i < prefixesCount; ++i) {
        QDomElement prefix = prefixes.at(i).toElement();
        res[i].setPrefix(prefix.attribute("prefix"));
        QDomNodeList files = prefix.childNodes();
        int filesCount     = files.count();
        for (int j = 0; j < filesCount; ++j) {
            QDomElement file = files.at(j).toElement();
            res[i].addFile(file.text(), file.attribute("alias", ""),
                           file.attribute("empty", "false") == "true");
        }
    }
    return res;
}

void QrcParser::setPrefixes(const QList<QrcPrefix> &prefixes) {
    qrcDoc.clear();
    QDomElement root = this->qrcDoc.createElement("RCC");
    this->qrcDoc.appendChild(root);

    // Add the new prefixes
    for (const auto &prefix : prefixes) {
        QDomElement prefixElement = this->qrcDoc.createElement("qresource");
        prefixElement.setAttribute("prefix", prefix.getPrefix());
        size_t filesCount = prefix.getFiles().size();
        for (int j = 0; j < filesCount; ++j) {
            QrcFile file            = prefix.getFiles().at(j);
            QDomElement fileElement = this->qrcDoc.createElement("file");
            fileElement.appendChild(this->qrcDoc.createTextNode(file.getFilePath()));
            if (!file.getAlias().isEmpty()) {
                fileElement.setAttribute("alias", file.getAlias());
            }
            if (file.isEmpty()) {
                fileElement.setAttribute("empty", "true");
            }
            prefixElement.appendChild(fileElement);
        }
        root.appendChild(prefixElement);
    }
}

QString QrcParser::getFileName() const {
    return this->qrcFilePath;
}

void QrcParser::setFileName(const QString &qrcFilePath) {
    this->qrcFilePath = qrcFilePath;
    this->qrcFile.setFileName(qrcFilePath);
}

bool QrcParser::checkFilesExist() const {
    QList<QrcPrefix> prefixes = this->getPrefixes();
    size_t prefixesCount      = prefixes.size();
    for (int i = 0; i < prefixesCount; ++i) {
        const QrcPrefix &prefix = prefixes.at(i);
        size_t filesCount       = prefix.getFiles().size();
        for (int j = 0; j < filesCount; ++j) {
            QrcFile file         = prefix.getFiles().at(j);
            QString filePath     = file.getFilePath();
            QString absolutePath = QFileInfo(filePath).absoluteFilePath();
            if (!QFileInfo::exists(absolutePath)) {
                QMessageBox::critical(nullptr, "Error",
                                      "The file " + QDir::cleanPath(absolutePath) +
                                          " does not exist.");
                return false;
            }
        }
    }
    return true;
}
