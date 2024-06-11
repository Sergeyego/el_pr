#ifndef FORMPART_H
#define FORMPART_H

#include <QWidget>
#include <QGroupBox>
#include <QScrollBar>
#include "db/dbtablemodel.h"
#include "db/dbmapper.h"
#include "rels.h"
#include "models.h"
#include "dialogloadchem.h"
#include "dialogtmp.h"
#include "modelro.h"
#include "olap/tableview.h"

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

class ModelStat : public ModelRo
{
    Q_OBJECT
public:
    ModelStat(QObject *parent=0);
    virtual void refresh(int id_part);
    QString getTitle();
public slots:
    void setTitle(QString t);
private:
    QString tit;
signals:
    void sigSum(QString s);

};

class ModelPackPal : public ModelStat
{
    Q_OBJECT
public:
    ModelPackPal(QObject *parent=0);
    void refresh(int id_part);

};

class ModelPackEl : public ModelStat
{
    Q_OBJECT
public:
    ModelPackEl(QObject *parent=0);
    void refresh(int id_part);

};

class ModelThermoPack : public ModelStat
{
    Q_OBJECT
public:
    ModelThermoPack(QObject *parent=0);
    void refresh(int id_part);

};

class ModelPerePackEl : public ModelStat
{
    Q_OBJECT
public:
    ModelPerePackEl(QObject *parent=0);
    void refresh(int id_part);
};

class ModelBreakEl : public ModelStat
{
    Q_OBJECT
public:
    ModelBreakEl(QObject *parent=0);
    void refresh(int id_part);
};

class ModelSelfEl : public ModelStat
{
    Q_OBJECT
public:
    ModelSelfEl(QObject *parent=0);
    void refresh(int id_part);
};

class ModelStockEl : public ModelStat
{
    Q_OBJECT
public:
    ModelStockEl(QObject *parent=0);
    void refresh(int id_part);
};

class ModelShipEl : public ModelStat
{
    Q_OBJECT
public:
    ModelShipEl(QObject *parent=0);
    void refresh(int id_part);
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
    ModelPackPal *modelPackPal;
    ModelPackEl *modelPackEl;
    ModelThermoPack *modelThermoPack;
    ModelPerePackEl *modelPerePackEl;
    ModelStockEl *modelStockEl;
    ModelSelfEl *modelSelfEl;
    ModelShipEl *modelShipEl;
    ModelBreakEl *modelBreakEl;
    void loadSettings();
    void saveSettings();
    void refreshStat(QGroupBox *g, TableView *v);

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
