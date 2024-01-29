#ifndef FORMPART_H
#define FORMPART_H

#include <QWidget>
#include "db/dbtablemodel.h"
#include "db/dbmapper.h"
#include "rels.h"
#include "models.h"
#include "dialogloadchem.h"
#include "dialogtmp.h"

namespace Ui {
class FormPart;
}

class ModelPart : public DbTableModel
{
    Q_OBJECT
public:
    ModelPart(QObject *parent);
    QVariant data(const QModelIndex &index, int role) const;
    void refresh(const QDate &beg, const QDate &end, int id_el);
    bool insertRow(int row, const QModelIndex &parent);
public slots:
    void refreshState();
private:
    QDate dBeg;
    QDate dEnd;
    QMap <int,int> colorState;
};

class FormPart : public QWidget
{
    Q_OBJECT

public:
    explicit FormPart(QWidget *parent = nullptr);
    ~FormPart();

private:
    Ui::FormPart *ui;
    DbMapper *mapper;
    ModelPart *modelPart;
    DbTableModel *modelGlass;
    DbTableModel *modelZam;
    DbTableModel *modelZamBreak;
    DbTableModel *modelRab;
    ModelChemSrc *modelChem;
    ModelMechSrc *modelMech;
    ModelConsStatData *modelConsStatData;
    ModelConsStatPar *modelConsStatPar;
    void loadSettings();
    void saveSettings();

private slots:
    void updPart();
    void refreshCont(int ind);
    void setCurrentChemDev();
    void loadChem();
    void insertChemSamp();
    void refreshGlassData(QModelIndex index);
    void insertMark();
    void insertProvol();
    void insertPack();
};

#endif // FORMPART_H
