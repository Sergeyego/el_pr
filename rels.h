#ifndef RELS_H
#define RELS_H

#include <QObject>
#include "db/dbtablemodel.h"
#include "invoicemanager.h"

class Rels : public QObject
{
    Q_OBJECT
public:
    static Rels *instance();
    InvoiceManager *invoiceManager;
    DbSqlRelation *relGlass;
    DbSqlRelation *relCons;
    DbSqlRelation *relSrc;
    DbSqlRelation *relRcp;
    DbSqlRelation *relMark;
    DbSqlRelation *relProv;
    DbSqlRelation *relChem;
    DbSqlRelation *relMech;
    DbSqlRelation *relPress;
    DbSqlRelation *relChemDev;
    DbSqlRelation *relPack;
    DbSqlRelation *relLong;
    DbSqlRelation *relVar;
    DbSqlRelation *relDos;
    DbSqlRelation *relRab;
    DbSqlRelation *relPackType;
    DbSqlRelation *relElPart;

protected:
    explicit Rels(QObject *parent = 0);

private:
    static Rels *rels_instance;

};

#endif // RELS_H
