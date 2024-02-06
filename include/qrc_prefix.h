#ifndef QRC_PREFIX_H
#define QRC_PREFIX_H

#include <QList>
#include <QString>
#include <QStringList>

#include "qrc_file.h"

class QrcPrefix {
public:
    QrcPrefix();
    QrcPrefix(const QString &prefix);
    virtual ~QrcPrefix();
    QString getPrefix() const;
    void setPrefix(const QString &value);
    const QList<QrcFile> &getFiles() const;
    void setFiles(const QList<QrcFile> &value);
    void addFile(const QrcFile &file);
    void addFile(const QString &filePath, const QString &alias = "", bool empty = false);

private:
    QString prefix;
    QList<QrcFile> files;
};

#endif // QRC_PREFIX_H
