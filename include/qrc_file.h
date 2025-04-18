#ifndef QRC_FILE_H
#define QRC_FILE_H

#include <QString>

class QrcFile {
private:
    QString filePath;
    QString alias;
    bool empty;

public:
    explicit QrcFile(const QString &filePath, const QString &alias = "", bool empty = false);
    virtual ~QrcFile();
    QString getFilePath() const;
    QString getAlias() const;
    bool isEmpty() const;
    void setEmpty(bool empty);
    void setAlias(const QString &alias);
};

#endif /* QRC_FILE_H */
