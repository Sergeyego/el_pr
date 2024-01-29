#ifndef MODELS_H
#define MODELS_H

#include <QObject>
#include "db/dbtablemodel.h"
#include "rels.h"
#include "modelro.h"

struct range {
    QVariant min;
    QVariant max;
};

class ModelChem : public DbTableModel
{
public:
    ModelChem(QString tname, QObject *parent=0);
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    ~ModelChem();
    void refresh(int id_part, QString dopFlt="");
private:
    QColor getColor(const QModelIndex &index) const;
    QString getToolTip(const QModelIndex &index) const;

protected:
    QMap <int,range> map;
    QString tablename;
    int colVal;
    int colIdChem;
    int colIdPart;
    QString flt;
    QString tuQuery;
};

class ModelChemSrc : public ModelChem
{
public:
    ModelChemSrc(QObject *parent=0);
    int addChem(int id_chem, double val, int id_dev);
    QList <int> ids();
};

class ModelMechSrc : public ModelChem
{
public:
    ModelMechSrc(QObject *parent=0);
};

class ModelConsStatData : public ModelRo
{
    Q_OBJECT
public:
    ModelConsStatData(QObject *parent);
    void refresh(int id_load);
    QVariant data(const QModelIndex &item, int role) const;
private:
    QMultiMap <int,QString> inPar;
    int current_id_load;
private slots:
    void refreshInPar();
};

class ModelConsStatPar : public ModelRo
{
    Q_OBJECT
public:
    ModelConsStatPar(QObject *parent);
    QVariant data(const QModelIndex &item, int role) const;
    void refresh(int id_load, int id_part);
};
#endif // MODELS_H
