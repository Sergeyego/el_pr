#include "models.h"

ModelChem::ModelChem(QString tname, QObject *parent) : DbTableModel(tname,parent), tablename(tname)
{
    colIdPart=0;
    colIdChem=1;
    colVal=2;
}

QVariant ModelChem::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    if (role==Qt::BackgroundColorRole){
        return getColor(index);
    }
    if (role==Qt::ToolTipRole){
        return getToolTip(index);
    }
    return DbTableModel::data(index,role);
}

ModelChem::~ModelChem()
{

}

void ModelChem::refresh(int id_part, QString dopFlt)
{
    map.clear();
    QSqlQuery query;
    QString qu=tuQuery;
    qu=qu.replace(":id",QString::number(id_part));
    query.prepare(qu);
    if (query.exec()){
        while (query.next()){
            range r;
            r.min=query.value(1);
            r.max=query.value(2);
            map.insert(query.value(0).toInt(),r);
        }
    } else {
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    }
    QString strFlt;
    if (!dopFlt.isEmpty()){
        strFlt=dopFlt+" and "+flt+" = "+QString::number(id_part);
    } else {
        strFlt=flt+" = "+QString::number(id_part);
    }
    setFilter(strFlt);
    setDefaultValue(colIdPart,id_part);
    select();
}

QColor ModelChem::getColor(const QModelIndex &index) const
{
    if (!index.isValid()){
        return QColor();
    }
    int id_chem=data(this->index(index.row(),colIdChem),Qt::EditRole).toInt();
    double val=data(this->index(index.row(),colVal),Qt::EditRole).toDouble();
    range r=map.value(id_chem);
    QColor color=Qt::white;
    if (!r.min.isNull() || !r.max.isNull()){
        if (!r.max.isNull()){
            if ((val>r.min.toDouble()) && (val<r.max.toDouble())){
                color=QColor(170,255,170);
            } else if ((val<r.min.toDouble()) || (val>r.max.toDouble())){
                color=QColor(255,170,170);
            } else {
                color=Qt::yellow;
            }
        } else {
            if (val<r.min.toDouble()){
                color=QColor(255,170,170);
            } else if (val>r.min.toDouble()){
                color=QColor(170,255,170);
            } else {
                color=Qt::yellow;
            }
        }
    }
    return color;
}

QString ModelChem::getToolTip(const QModelIndex &index) const
{
    if (!index.isValid()){
        return QString();
    }
    int id_chem=data(this->index(index.row(),colIdChem),Qt::EditRole).toInt();
    QString el=data(this->index(index.row(),colIdChem),Qt::DisplayRole).toString();
    range r=map.value(id_chem);
    QString s;
    if (!r.min.isNull() || !r.max.isNull()){
        s+=el;
        if (r.max.isNull() && !r.min.isNull()){
            s+=QString(": min %1").arg(r.min.toDouble());
        } else if (!r.max.isNull() && r.min.isNull()){
            s+=QString(": max %1").arg(r.max.toDouble());
        } else {
            s+=QString(": min %1 max %2").arg(r.min.toDouble()).arg(r.max.toDouble());
        }
    }
    return s;
}

ModelChemSrc::ModelChemSrc(QObject *parent) : ModelChem("parti_chem",parent)
{
    addColumn("id","id");
    addColumn("id_part","id_part");
    addColumn("id_chem",QString::fromUtf8("Элем."),Rels::instance()->relChem);
    addColumn("kvo",QString::fromUtf8("Сод., %"));
    addColumn("id_dev",QString::fromUtf8("Прибор"),Rels::instance()->relChemDev);
    addColumn("dt_cre",QString::fromUtf8("Дата"));
    setValidator(3,new QDoubleValidator(0,100,3,this));
    setSort(tablename+".id_chem, "+tablename+".dt_cre");
    flt=tableName+".id_part";
    setColumnFlags(5,Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    colIdPart=1;
    colIdChem=2;
    colVal=3;
    tuQuery="select c.id_chem, c.min, c.max from chem_tu as c where "
            "c.id_el = (select p.id_el from parti as p where p.id = :id ) "
            "and c.id_var = (select p.id_var from parti as p where p.id = :id )";
}

int ModelChemSrc::addChem(int id_chem, double val, int id_dev)
{
    int id_part=defaultValue(colIdPart).toInt();
    int id=-1;
    QSqlQuery query;
    query.prepare("insert into parti_chem (id_part, id_chem, kvo, id_dev) values (:id_part, :id_chem, :kvo, :id_dev) returning id");
    query.bindValue(":id_part",id_part);
    query.bindValue(":id_chem",id_chem);
    query.bindValue(":kvo",val);
    query.bindValue(":id_dev",id_dev);
    if (query.exec()){
        while (query.next()){
            id=query.value(0).toInt();
        }
    } else {
        QMessageBox::critical(NULL,"Error",query.lastError().text(),QMessageBox::Cancel);
    }
    return id;
}

QList <int> ModelChemSrc::ids()
{
    return map.keys();
}

ModelMechSrc::ModelMechSrc(QObject *parent) : ModelChem("parti_mech",parent)
{
    addColumn("id_part","id_part");
    addColumn("id_mech",QString::fromUtf8("Параметр"),Rels::instance()->relMech);
    addColumn("kvo",QString::fromUtf8("Значение"));
    setSort("parti_mech.id_mech");
    setDecimals(2,2);
    flt=tableName+".id_part";
    colIdPart=0;
    colIdChem=1;
    colVal=2;
    tuQuery="select m.id_mech, m.min, m.max from mech_tu as m where "
            "m.id_el = (select p.id_el from parti as p where p.id = :id )"
            "and m.id_var = (select p.id_var from parti as p where p.id = :id )";
}

ModelConsStatData::ModelConsStatData(QObject *parent) :ModelRo(parent)
{
    dec=1;
    connect(this,SIGNAL(newQuery()),this,SLOT(refreshInPar()));
}

void ModelConsStatData::refresh(int id_load)
{
    current_id_load=id_load;
    QSqlQuery query;
    query.prepare("(select d.id_load, d.proc, m.nam, l.part_lump, l.modul, NULL "
                  "from glass_korr_load_data as d "
                  "inner join glass_sump_load as l on l.id=d.id_sump_load "
                  "inner join matr as m on m.id=l.id_matr "
                  "inner join glass_sump as s on s.id=l.id_sump "
                  "where d.id_load = (select id_korr_load from glass_cons_load where id = :id_load1 ) order by proc) "
                  "union "
                  "(select NULL, 100.0, mm.nam, ll.part_lump, ll.modul, ll.id "
                  "from glass_sump_load as ll "
                  "inner join matr as mm on mm.id=ll.id_matr "
                  "inner join glass_sump as ss on ss.id=ll.id_sump "
                  "where ll.id = (select id_sump_load from glass_cons_load where id = :id_load2 ))");
    query.bindValue(":id_load1",id_load);
    query.bindValue(":id_load2",id_load);
    if (execQuery(query)){
        setHeaderData(1,Qt::Horizontal,QString::fromUtf8("%"));
        setHeaderData(2,Qt::Horizontal,QString::fromUtf8("Стекло"));
        setHeaderData(3,Qt::Horizontal,QString::fromUtf8("Парт.гл."));
        setHeaderData(4,Qt::Horizontal,QString::fromUtf8("Модуль"));
    }
}

QVariant ModelConsStatData::data(const QModelIndex &item, int role) const
{
    if (role==Qt::ToolTipRole){
        QVariant id=ModelRo::data(index(item.row(),0),Qt::EditRole);
        if (!id.isNull()){
            QString str;
            QList<QString> l=inPar.values(id.toInt());
            foreach (QString s, l) {
                if (!str.isEmpty()){
                    str+="\n";
                }
                str+=s;
            }
            if (!str.isEmpty()){
                str.prepend(QString::fromUtf8("Параметры корректора ")+QString::fromUtf8(":\n"));
            }
            return str;
        }
    }
    return ModelRo::data(item,role);
}

void ModelConsStatData::refreshInPar()
{
    QSqlQuery qu;
    qu.prepare("select l.id_load, p.nam, l.val, l.temp, l.dat "
               "from glass_korr_load_par as l "
               "inner join glass_par as p on p.id=l.id_param "
               "where l.id_load = (select id_korr_load from glass_cons_load where id= :id_load)");
    qu.bindValue(":id_load",current_id_load);
    inPar.clear();
    if (qu.exec()){
        while (qu.next()){
            QString val=qu.value(1).toString()+QString::fromUtf8(" = ")+qu.value(2).toString();
            if (!qu.value(3).isNull()){
                val+=QString::fromUtf8(" (")+qu.value(3).toString()+QString::fromUtf8("°С)");
            }
            val+=" "+qu.value(4).toDate().toString("dd.MM.yy");
            inPar.insert(qu.value(0).toInt(),val);
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),qu.lastError().text(),QMessageBox::Cancel);
    }
}

ModelConsStatPar::ModelConsStatPar(QObject *parent) : ModelRo(parent)
{
    dec=3;
}

QVariant ModelConsStatPar::data(const QModelIndex &item, int role) const
{
    if (role==Qt::DisplayRole && item.column()==3){
        return QLocale().toString(data(item,Qt::EditRole).toDouble(),'f',1);
    }
    return ModelRo::data(item,role);
}

void ModelConsStatPar::refresh(int id_load, int id_part)
{
    QSqlQuery query;
    query.prepare("select jp.id_param, gp.nam, jp.val, jp.temp, jp.dat from "
                  "(select id_load, id_param, max(dat) as dat "
                  "from glass_cons_load_par as p where id_load = :id_load "
                  "and dat<=(select dat_part from parti where id = :id_part ) "
                  "group by id_param, id_load) as p "
                  "inner join glass_cons_load_par as jp on jp.id_load= p.id_load and jp.id_param=p.id_param and jp.dat=p.dat "
                  "inner join glass_par as gp on (gp.id=jp.id_param) "
                  "order by jp.id_param desc, jp.dat");
    query.bindValue(":id_load",id_load);
    query.bindValue(":id_part",id_part);
    if (execQuery(query)){
        setHeaderData(1,Qt::Horizontal,QString::fromUtf8("Параметр"));
        setHeaderData(2,Qt::Horizontal,QString::fromUtf8("Значен."));
        setHeaderData(3,Qt::Horizontal,QString::fromUtf8("Тизм.,°С"));
        setHeaderData(4,Qt::Horizontal,QString::fromUtf8("Дата"));
    }
}
