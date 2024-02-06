#include "comboboxDelegate.h"

#include <QComboBox>

ComboBoxDelegate::ComboBoxDelegate(QObject *parent) : QStyledItemDelegate(parent) {
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const {
    if (index.column() == 2) {
        QComboBox *editor = new QComboBox(parent);
        editor->addItems(QStringList() << "true"
                                       << "false");
        return editor;
    }
    return NULL;
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    if (QComboBox *comboBox = qobject_cast<QComboBox *>(editor)) {
        QString value = index.model()->data(index, Qt::EditRole).toString();
        int i         = comboBox->findText(value);
        if (i >= 0) {
            comboBox->setCurrentIndex(i);
        }
    }
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const {
    if (QComboBox *comboBox = qobject_cast<QComboBox *>(editor)) {
        model->setData(index, comboBox->currentText(), Qt::EditRole);
    }
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                            const QModelIndex &index) const {
    editor->setGeometry(option.rect);
}
