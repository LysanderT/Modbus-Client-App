#ifndef DELEGATE_HPP
#define DELEGATE_HPP

#include <QComboBox>
#include <QStyledItemDelegate>
#include <QPushButton>
#include <QApplication>

class DeleteDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    DeleteDelegate(QObject *parent = 0): QStyledItemDelegate(parent) { };
    QWidget *createEditor(QWidget*parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override
    {
        //        QPushButton *editor = new QPushButton(parent);
        //        editor->setText("1");
        //        return editor;
        return nullptr;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const override
    {
        return;
    }
    void paint( QPainter* pPainter, const QStyleOptionViewItem& qOption, const QModelIndex& qIndex) const override
    {
        QStyleOptionButton button;
        button.rect = qOption.rect;
        button.text = "-";
        button.state = QStyle::State_On;
        //button.state = QStyle::State_Enabled;

        QApplication::style()->drawControl(QStyle::CE_PushButton, &button, pPainter);
    }
};

class FunctionCodeDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    FunctionCodeDelegate(QObject *parent = 0): QStyledItemDelegate(parent) { };
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
    {
        QComboBox *editor = new QComboBox(parent);
        editor->addItems({"01","02","03","04","05","06"});
        return editor;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        QString text = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        int tindex = comboBox->findText(text);
        comboBox->setCurrentIndex(tindex);
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QString text = comboBox->currentText();
        model->setData(index, text, Qt::EditRole);
    }

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        editor->setGeometry(option.rect);
    }
};


class DataTypeDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    DataTypeDelegate(QObject *parent = 0): QStyledItemDelegate(parent) { };
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
    {
        QComboBox *editor = new QComboBox(parent);
        editor->addItems({"int","bool","double"});
        return editor;
    }

    void setEditorData(QWidget *editor, const QModelIndex &index) const
    {
        QString text = index.model()->data(index, Qt::EditRole).toString();
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        int tindex = comboBox->findText(text);
        comboBox->setCurrentIndex(tindex);
    }

    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const
    {
        QComboBox *comboBox = static_cast<QComboBox*>(editor);
        QString text = comboBox->currentText();
        model->setData(index, text, Qt::EditRole);
    }

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        editor->setGeometry(option.rect);
    }
};





#endif // DELEGATE_HPP
