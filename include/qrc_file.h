#ifndef QRC_FILE_H
#define QRC_FILE_H

#include <QString>

class QrcFile {
private:
    QString filePath;
    QString alias;
    bool empty;

public:
    QrcFile(QString filePath, QString alias = "", bool empty = false);
    virtual ~QrcFile();
    QString getFilePath() const;
    QString getAlias() const;
    bool isEmpty() const;
    void setEmpty(bool empty);
    void setAlias(QString alias);
};

#endif /* QRC_FILE_H */
