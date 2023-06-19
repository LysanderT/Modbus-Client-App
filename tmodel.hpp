#ifndef TMODEL_HPP
#define TMODEL_HPP

#include <QAbstractTableModel>
#include <QTableView>
#include <QComboBox>
#include <QPushButton>
#include <QDebug>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

class TModel : public QAbstractTableModel
{
    Q_OBJECT

    struct Row{
        QString slave = "00";
        QString function_code = "00";
        QString address = "0000";
        QString length = "1";
        QString data_type = "int";
        double ratio = 1;
        QString var = "";
        QString unit = "";
        QString cur_val = "";
        QString real_val = "";
    };

    QVector<QString> header = {"", "从站地址(0x)", "功能码", "地址(0x)", "长度(0d)", "数据类型", "变比", "变量", "单位", "当前值", "实际值"};

public:
    explicit TModel(QTableView* parent = Q_NULLPTR) : QAbstractTableModel(parent){}
    ~TModel(){}

    virtual int rowCount(const QModelIndex &/*parent*/ = QModelIndex()) const override{
        return rows.size();
    }
    virtual int columnCount(const QModelIndex &/*parent*/ = QModelIndex()) const override{
        return header.size();
    }
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override{
        if(!index.isValid() || index.row()>=rowCount() || index.column()>=columnCount()) return QVariant();
        switch(role){

        case Qt::DisplayRole:
            switch (index.column()) {
            case 0:
                return "-";
                break;
            case 1:
                return rows[index.row()].slave;
                break;
            case 2:
                return rows[index.row()].function_code;
                break;
            case 3:
                return rows[index.row()].address;
                break;
            case 4:
                return rows[index.row()].length;
                break;
            case 5:
                return rows[index.row()].data_type;
                break;
            case 6:
                return rows[index.row()].ratio;
                break;
            case 7:
                return rows[index.row()].var;
                break;
            case 8:
                return rows[index.row()].unit;
                break;
            case 9:
                return rows[index.row()].cur_val;
                break;
            case 10:
                return rows[index.row()].real_val;
                break;
            default:
                return QVariant();
                break;
            }
            break;

        case Qt::TextAlignmentRole:
            return Qt::AlignCenter;
            break;


        default:
            return QVariant();
            break;
        }
    }
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override{
        bool ok = false;
        if(value == "" || role != Qt::EditRole || !index.isValid() || index.row()>=rowCount() || index.column()>=columnCount()) return false;
        switch (index.column()) {
        case 0:
            return false;
            break;
        case 1:
            rows[index.row()].slave = value.toString();
            break;
        case 2:
            rows[index.row()].function_code = value.toString();
            break;
        case 3:
            rows[index.row()].address = value.toString();
            break;
        case 4:
            rows[index.row()].length = value.toString();
            break;
        case 5:
            rows[index.row()].data_type = value.toString();
            break;
        case 6:
            value.toInt(&ok);
            if(ok){rows[index.row()].ratio = value.toInt(&ok);break;}
            value.toDouble(&ok);
            if(ok){rows[index.row()].ratio = value.toDouble(&ok);break;}
            return false;
        case 7:
            rows[index.row()].var = value.toString();
            break;
        case 8:
            rows[index.row()].unit = value.toString();
            break;
        case 9:
            rows[index.row()].cur_val = value.toString();
            break;
        case 10:
            rows[index.row()].real_val = value.toString();
            break;
        default:
            return false;
            break;
        }
        emit dataChanged(index, index);
        return true;
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override
    {
        switch(role)
        {
        case Qt::DisplayRole:
            if(orientation==Qt::Horizontal && section>=0 && section<=columnCount())
                return header.at(section);
            break;


        default:
            break;
        }


        return QAbstractItemModel::headerData(section, orientation, role);
    }


    virtual Qt::ItemFlags flags(const QModelIndex &index) const override{
        return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
    }


private:
    virtual bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override{
        if(row > rowCount()) row = rowCount();
        if(row < 0) row = 0;

        beginInsertRows(parent, row, row+count-1);

        for(int i = 0; i < count; ++i){
            rows.insert(rows.begin()+row+i,Row());
            if(row+i-1>=0){
                rows[row+i].slave = rows[row+i-1].slave;
                rows[row+i].function_code = rows[row+i-1].function_code;

                QString str = rows[row+i-1].length;
                rows[row+i].length = str;
                int y = str.toInt();
                str = QString::number(y,16);
                y = str.toInt(nullptr,16);
                int x = rows[row+i-1].address.toInt(nullptr,16);
                str =  QString("%1").arg(x+y,4,16,QLatin1Char('0'));
                rows[row+i].address = str;

                rows[row+i].data_type = rows[row+i-1].data_type;
                rows[row+i].ratio = rows[row+i-1].ratio;
                rows[row+i].var = rows[row+i-1].var;
                rows[row+i].unit = rows[row+i-1].unit;

            }
        }

        endInsertRows();

        emit dataChanged(createIndex(row, 0), createIndex(row+count-1, columnCount()-1));
        return true;
    }

    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override
    {
        if(row < 0 || row >= rowCount() || row + count > rowCount()) return false;

        beginRemoveRows(parent, row, row+count-1);

        for(int i = 0; i < count; ++i)
        {
            rows.remove(row);
        }

        endRemoveRows();

        return true;
    }
public:
    //    virtual bool insertRow(int row, const QModelIndex &parent = QModelIndex());
    void appendRow()
    {
        insertRow(rowCount());
    }
public slots:
    void remove_when_click(QModelIndex index)
    {
        if(index.column()!=0){return;}
        if(index.row()>=rowCount()){return;}
        removeRow(index.row());
    }
private:
    QVector<Row> rows = {};
};


#endif // TMODEL_HPP
