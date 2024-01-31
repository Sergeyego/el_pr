#include "modelnakl.h"

ModelNakl::ModelNakl(QObject *parent) : DbTableModel("parti_nakl",parent)
{
    addColumn("id",tr("id"));
    addColumn("num",tr("Номер"));
    addColumn("dat",tr("Дата"));
    addColumn("tip",tr("Тип"));
    setSort(name()+".dat, "+name()+".num");
}

void ModelNakl::refresh(int id_type, QDate begDate, QDate endDate)
{
    QString filter=name()+".dat between '"+begDate.toString("yyyy-MM-dd")+"' and '"+endDate.toString("yyyy-MM-dd")+"' and "+name()+".tip = "+QString::number(id_type);
    setFilter(filter);
    setDefaultValue(3,id_type);
    setDefaultValue(1,"1");
    select();
}

bool ModelNakl::insertRow(int row, const QModelIndex &parent)
{
    select();
    if (rowCount()>0 && !isAdd()) {
        int old_num=this->data(this->index(rowCount()-1,1),Qt::EditRole).toInt();
        QVariant old_date=this->data(this->index(rowCount()-1,2),Qt::EditRole);
        setDefaultValue(1,old_num+1);
        setDefaultValue(2,!old_date.isNull()? old_date.toDate().addDays(1) : QDate::currentDate());
    }
    return DbTableModel::insertRow(row,parent);
}
