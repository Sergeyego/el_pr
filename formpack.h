#ifndef FORMPACK_H
#define FORMPACK_H

#include <QWidget>
#include "rels.h"
#include "db/dbtablemodel.h"
#include "db/dbmapper.h"
#include "modelnakl.h"
#include "dialogpackload.h"

namespace Ui {
class FormPack;
}

class ModelNaklData : public DbTableModel
{
    Q_OBJECT

public:
    explicit ModelNaklData(QString table, QObject *parent = 0);
    void refresh(int id_nakl);
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
    void loadPack();
};

#endif // FORMPACK_H
