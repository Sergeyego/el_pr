#ifndef FORMPACK_H
#define FORMPACK_H

#include <QWidget>
#include "rels.h"
#include "db/dbtablemodel.h"
#include "db/dbmapper.h"

namespace Ui {
class FormPack;
}

class ModelNakl : public DbTableModel
{
    Q_OBJECT

public:
    explicit ModelNakl(QObject *parent = 0);
    void refresh(int id_type, QDate begDate, QDate endDate);
    bool insertRow(int row, const QModelIndex &parent=QModelIndex());
};

class ModelNaklData : public DbTableModel
{
    Q_OBJECT

public:
    explicit ModelNaklData(QString table, QObject *parent = 0);
    void refresh(int id_nakl);
signals:
    void sigStock(QString mes);
};

class FormPack : public QWidget
{
    Q_OBJECT

public:
    explicit FormPack(QWidget *parent = nullptr);
    ~FormPack();

private:
    Ui::FormPack *ui;
    void loadSettings();
    void saveSettings();
    ModelNakl *modelNakl;
    ModelNaklData *modelPack;
    ModelNaklData *modelBreak;

    DbMapper *mapper;

private slots:
    void upd();
    void updData(int ind);
    void calcSum();
    void nakl();
    void naklPer();
};

#endif // FORMPACK_H
