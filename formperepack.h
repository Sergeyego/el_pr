#ifndef FORMPEREPACK_H
#define FORMPEREPACK_H

#include <QWidget>
#include <QSettings>
#include "db/dbtablemodel.h"
#include "db/dbmapper.h"
#include "rels.h"
#include "modelnakl.h"

namespace Ui {
class FormPerePack;
}

class ModelPerePack : public DbTableModel
{
    Q_OBJECT

public:
    explicit ModelPerePack(QObject *parent = 0);
    void refresh(int id_nakl);
};

class FormPerePack : public QWidget
{
    Q_OBJECT

public:
    explicit FormPerePack(QWidget *parent = nullptr);
    ~FormPerePack();

private:
    Ui::FormPerePack *ui;
    DbMapper *mapper;
    ModelNakl *modelNakl;
    ModelPerePack *modelPerePack;
    void loadSettings();
    void saveSettings();

private slots:
    void upd();
    void updData(int ind);
    void calcSum();
    void nakl();
};

#endif // FORMPEREPACK_H
