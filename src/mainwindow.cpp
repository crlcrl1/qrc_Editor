#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "comboboxDelegate.h"
#include "previewdialog.h"

#include <QCloseEvent>
#include <QFileDialog>
#include <QInputDialog>
#include <qmessagebox.h>

MainWindow::MainWindow(QWidget *parent, const QString &qrcFile)
    : QMainWindow(parent), ui(new Ui::MainWindow), qrcFile(qrcFile) {
    ui->setupUi(this);
    setWindowTitle("QRC Editor");
    setMinimumSize(600, 400);
    qrcModel = new QStandardItemModel(this);
    ui->qrcTreeView->setModel(qrcModel);
    ui->qrcTreeView->setAnimated(true);
    ui->qrcTreeView->setSelectionMode(QTreeView::ExtendedSelection);
    ui->qrcTreeView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
    ui->qrcTreeView->setItemDelegateForColumn(2, new ComboBoxDelegate(this));
    ui->statusbar->setFont(QFont("Microsoft YaHei UI", 11));
    if (!qrcFile.isEmpty()) {
        loadQrcFile(qrcFile);
    } else {
        qrcParser = nullptr;
    }
    saved = true;

    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::do_actionNew_triggered);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::do_actionOpen_triggered);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::do_actionSave_triggered);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::do_actionExit_triggered);
    connect(ui->actionAdd_prefix, &QAction::triggered, this,
            &MainWindow::do_addPrefixAction_triggered);
    connect(ui->actionRemove_prefix, &QAction::triggered, this,
            &MainWindow::do_removePrefixAction_triggered);

    connect(qrcModel, &QStandardItemModel::dataChanged, this, &MainWindow::do_qrcModel_dataChanged);
    connect(ui->qrcTreeView, &QTreeView::customContextMenuRequested, this,
            &MainWindow::do_qrcTreeView_menu);

    if (qrcFile.isEmpty() || !qrcParser->isReady()) {
        ui->actionAdd_prefix->setEnabled(false);
        ui->actionRemove_prefix->setEnabled(false);
        ui->actionSave->setEnabled(false);
    }
}

MainWindow::~MainWindow() {
    delete ui;
    delete qrcParser;
}

void MainWindow::do_actionOpen_triggered() {
    QString fileName =
        QFileDialog::getOpenFileName(this, tr("Open QRC File"), "", tr("QRC Files (*.qrc)"));
    if (fileName.isEmpty()) {
        return;
    }
    delete qrcParser;
    loadQrcFile(fileName);
}

void MainWindow::do_actionSave_triggered() {
    if (qrcFile.isEmpty()) {
        return;
    }
    QList<QrcPrefix> newPrefixes;
    int rowCount = qrcModel->rowCount();
    for (int i = 0; i < rowCount; i++) {
        QString prefix            = qrcModel->item(i, 0)->text();
        QStandardItem *prefixItem = qrcModel->item(i, 0);
        newPrefixes.append(QrcPrefix(prefix));
        QrcPrefix *newPrefix = &newPrefixes.last();
        int fileCount        = prefixItem->rowCount();
        for (int j = 0; j < fileCount; j++) {
            QStandardItem *fileItem  = prefixItem->child(j, 0);
            QStandardItem *aliasItem = prefixItem->child(j, 1);
            QStandardItem *emptyItem = prefixItem->child(j, 2);
            QString filePath         = fileItem->text();
            QString alias            = aliasItem->text();
            bool empty               = emptyItem->text() == "true";
            newPrefix->addFile(filePath, alias, empty);
        }
    }
    qrcParser->setPrefixes(newPrefixes);
    if (!qrcParser->checkFilesExist()) {
        const auto reply =
            QMessageBox::question(this, "QRC Editor", "Some files do not exist. Save anyway?",
                                  QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }
    }
    qrcParser->writeToFile();
    setWindowTitle(windowTitle().remove('*'));
    saved           = true;
    size_t fileSize = QFileInfo(qrcFile).size();
    ui->statusbar->showMessage(tr("File %1 written [%2B]").arg(qrcFile).arg(fileSize), 2000);
}

void MainWindow::do_actionNew_triggered() {
    delete qrcParser;
    qrcFile = QFileDialog::getSaveFileName(this, tr("New QRC File"), "", tr("QRC Files (*.qrc)"));
    if (qrcFile.isEmpty()) {
        return;
    }
    qrcParser = new QrcParser(qrcFile);
    qrcModel->clear();
    this->setWindowTitle(tr("QRC Editor - %1*").arg(qrcFile));
    saved = false;
    qrcModel->setHorizontalHeaderLabels(QStringList() << "File"
                                                      << "Alias"
                                                      << "Empty");
    ui->actionAdd_prefix->setEnabled(true);
    ui->actionRemove_prefix->setEnabled(true);
    ui->actionSave->setEnabled(true);
    ui->qrcTreeView->expandAll();
    ui->qrcTreeView->setColumnWidth(0, 300);
    ui->qrcTreeView->setColumnWidth(1, 300);
}

void MainWindow::do_actionExit_triggered() {
    close();
}

void MainWindow::do_qrcModel_dataChanged() {
    QString title = windowTitle();
    if (!title.endsWith('*')) {
        setWindowTitle(title + '*');
    }
    saved = false;
}

void MainWindow::do_qrcTreeView_menu(const QPoint &pos) {
    if (qrcFile.isEmpty() || !qrcParser->isReady()) {
        return;
    }
    QMenu menu(this);
    menu.setFont(QFont("Microsoft YaHei UI", 11));
    QModelIndex index       = ui->qrcTreeView->indexAt(pos);
    QModelIndex parentIndex = index.parent();
    if (index.isValid()) {
        if (parentIndex.isValid()) {
            auto *removeFileAction = new QAction("Remove File", &menu);
            auto *addFileAction    = new QAction("Add File", &menu);
            auto *previewAction    = new QAction("Preview", &menu);
            connect(previewAction, &QAction::triggered, this,
                    &MainWindow::do_actionPreview_triggered);
            connect(addFileAction, &QAction::triggered, this,
                    &MainWindow::do_addFileAction_triggered);
            connect(removeFileAction, &QAction::triggered, this,
                    &MainWindow::do_removeFileAction_triggered);
            menu.addAction(addFileAction);
            menu.addAction(removeFileAction);
            menu.addAction(previewAction);
        } else {
            auto *removePrefixAction = new QAction("Remove Prefix", &menu);
            auto *addPrefixAction    = new QAction("Add Prefix", &menu);
            auto *addFileAction      = new QAction("Add File", &menu);
            connect(addFileAction, &QAction::triggered, this,
                    &MainWindow::do_addFileAction_triggered);
            connect(removePrefixAction, &QAction::triggered, this,
                    &MainWindow::do_removePrefixAction_triggered);
            connect(addPrefixAction, &QAction::triggered, this,
                    &MainWindow::do_addPrefixAction_triggered);
            menu.addAction(addPrefixAction);
            menu.addAction(addFileAction);
            menu.addAction(removePrefixAction);
        }
    } else {
        auto *addPrefixAction = new QAction("Add Prefix", &menu);
        connect(addPrefixAction, &QAction::triggered, this,
                &MainWindow::do_addPrefixAction_triggered);
        menu.addAction(addPrefixAction);
    }
    menu.exec(QCursor::pos());
}

void MainWindow::do_removePrefixAction_triggered() {
    if (const QModelIndex index = ui->qrcTreeView->currentIndex(); index.isValid()) {
        qrcModel->removeRow(index.row());
        do_qrcModel_dataChanged();
    }
}

void MainWindow::do_addPrefixAction_triggered() {
    QString prefix = QInputDialog::getText(this, tr("Add Prefix"), tr("Prefix:"));
    if (prefix.isEmpty()) {
        return;
    }
    if (!prefix.startsWith('/')) {
        prefix = "/" + prefix;
    }
    int rowCount = qrcModel->rowCount();
    for (int i = 0; i < rowCount; i++) {
        if (qrcModel->item(i, 0)->text() == prefix) {
            QMessageBox::critical(this, tr("Error"), tr("Prefix already exists"));
            return;
        }
    }

    auto *item = new QStandardItem(prefix);
    qrcModel->appendRow(item);
    do_qrcModel_dataChanged();
}

void MainWindow::do_addFileAction_triggered() {
    QModelIndex index = ui->qrcTreeView->currentIndex();
    if (index.isValid()) {
        QModelIndex parentIndex = index.parent();
        if (parentIndex.isValid()) {
            QStandardItem *item = qrcModel->itemFromIndex(parentIndex);
            QString filePath    = QFileDialog::getOpenFileName(this, tr("Add file"), "", "");
            if (filePath.isEmpty()) {
                return;
            }
            filePath = QrcParser::calculateRelativePath(qrcFile, filePath);
            item->appendRow(QList<QStandardItem *>()
                            << new QStandardItem(filePath) << new QStandardItem("")
                            << new QStandardItem("false"));
            ui->qrcTreeView->expand(parentIndex);
        } else {
            QStandardItem *item = qrcModel->itemFromIndex(index);
            QString filePath    = QFileDialog::getOpenFileName(this, tr("Add file"), "", "");
            if (filePath.isEmpty()) {
                return;
            }
            filePath = QrcParser::calculateRelativePath(qrcFile, filePath);
            if (!filePath.isEmpty()) {
                auto *fileItem  = new QStandardItem(filePath);
                auto *aliasItem = new QStandardItem("");
                auto *emptyItem = new QStandardItem("false");
                item->appendRow(QList<QStandardItem *>() << fileItem << aliasItem << emptyItem);
            }
            ui->qrcTreeView->expand(index);
        }
        do_qrcModel_dataChanged();
    }
}

void MainWindow::do_removeFileAction_triggered() {
    QModelIndex index = ui->qrcTreeView->currentIndex();
    if (index.isValid()) {
        qrcModel->removeRow(index.row(), index.parent());
    }
    do_qrcModel_dataChanged();
}

void MainWindow::do_actionPreview_triggered() {
    const QModelIndex index = ui->qrcTreeView->currentIndex();
    if (index.isValid()) {
        QModelIndex parentIndex = index.parent();
        if (parentIndex.isValid()) {
            QString filePath = qrcModel->itemFromIndex(parentIndex)->child(index.row(), 0)->text();
            QDir fileDir     = QFileInfo(qrcFile).dir();
            QString absoluteFilePath = QDir::cleanPath(fileDir.absoluteFilePath(filePath));
            if (!QFileInfo::exists(absoluteFilePath)) {
                QMessageBox::critical(this, tr("Error"),
                                      tr("File not found: %1").arg(absoluteFilePath));
                return;
            }
            auto *previewDialog = new PreviewDialog(this, absoluteFilePath);
            previewDialog->show();
        }
    }
}

void MainWindow::loadQrcFile(const QString &fileName) {
    qrcParser = new QrcParser(fileName);
    qrcFile   = fileName;
    qrcModel->clear();
    if (qrcParser->isReady()) {
        qrcModel->setHorizontalHeaderLabels(QStringList() << "File"
                                                          << "Alias"
                                                          << "Empty");
        qrcPrefixes = qrcParser->getPrefixes();
        for (int i = 0; i < qrcPrefixes.size(); i++) {
            QrcPrefix prefix = qrcPrefixes.at(i);
            auto *item       = new QStandardItem(prefix.getPrefix());
            qrcModel->setItem(i, 0, item);
            auto *temp = new QStandardItem("");
            temp->setEditable(false);
            qrcModel->setItem(i, 1, temp);
            qrcModel->setItem(i, 2, temp->clone());
            size_t rowCount = prefix.getFiles().size();
            for (int j = 0; j < rowCount; j++) {
                QrcFile file    = prefix.getFiles().at(j);
                auto *fileItem  = new QStandardItem(file.getFilePath());
                auto *aliasItem = new QStandardItem(file.getAlias());
                auto *emptyItem = new QStandardItem(file.isEmpty() ? "true" : "false");
                item->appendRow(QList<QStandardItem *>() << fileItem << aliasItem << emptyItem);
            }
        }
        ui->qrcTreeView->expandAll();
        ui->qrcTreeView->setColumnWidth(0, 400);
        ui->qrcTreeView->setColumnWidth(1, 300);
        this->setWindowTitle(tr("QRC Editor - %1").arg(fileName));
        ui->actionAdd_prefix->setEnabled(true);
        ui->actionRemove_prefix->setEnabled(true);
        ui->actionSave->setEnabled(true);
        saved = true;
        ui->statusbar->showMessage(tr("File %1 loaded").arg(fileName), 2000);
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (!saved) {
        const auto reply =
            QMessageBox::question(this, "QRC Editor", "Do you want to save changes?",
                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (reply == QMessageBox::Yes) {
            do_actionSave_triggered();
        } else if (reply == QMessageBox::Cancel) {
            event->ignore();
            return;
        }
    }
    event->accept();
}
