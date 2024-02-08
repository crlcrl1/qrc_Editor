#ifndef QRC_PARSER_H
#define QRC_PARSER_H

#include <QDomDocument>
#include <QFile>
#include <QString>

#include "qrc_prefix.h"

class QrcParser {
public:
    QrcParser(QString qrcFilePath);
    QrcParser();
    virtual ~QrcParser();

    /**
     * @brief Add a file resource to the qrc file.
     *
     * @param prefix The prefix to use for the file.
     * @param filePath The absolute path to the file.
     * @param alias The alias to use for the file, if "" then no alias is used.
     * @param empty If true, the file will be added as an empty file.
     */
    void addFile(QString prefix, QString filePath, QString alias = "", bool empty = false);

    /**
     * @brief A static method to calculate the relative path between two files.
     *
     * @param baseFile The absolute path to the base file.
     * @param targetFile The absolute path to the target file.
     * @return The relative path from the base file to the target file.
     */
    static QString calculateRelativePath(QString baseFile, QString targetFile);

    /**
     * @brief Write the changes to the qrc file.
     */
    void writeToFile();

    /**
     * @brief Check if the parser is ready to be used.
     *
     * @return True if the parser is ready, false otherwise.
     */
    bool isReady() const;

    /**
     * @brief Get the prefixes in the qrc file.
     *
     * @return A list of prefixes in the qrc file.
     */
    QList<QrcPrefix> getPrefixes() const;

    /**
     * @brief Set the prefixes in the qrc file.
     *
     * @param prefixes The prefixes to set in the qrc file.
     */
    void setPrefixes(const QList<QrcPrefix> &prefixes);

    QString getFileName() const;

    void setFileName(QString qrcFilePath);

    bool checkFilesExist() const;

private:
    QString qrcFilePath;
    QFile qrcFile;
    QDomDocument qrcDoc;
    bool ready;

    QDomElement findOrCreatePrefix(QDomElement root, QString prefix);
};

#endif // QRC_PARSER_H
