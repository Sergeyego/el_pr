#ifndef FORMSELF_H
#define FORMSELF_H

#include <QWidget>
#include "rels.h"
#include "db/dbtablemodel.h"
#include "db/dbmapper.h"

namespace Ui {
class FormSelf;
}

class ModelSelfNakl : public DbTableModel
{
    Q_OBJECT

public:
    explicit ModelSelfNakl(QObject *parent = 0);
    void refresh(int id_type, QDate begDate, QDate endDate);
    bool insertRow(int row, const QModelIndex &parent=QModelIndex());
};

class ModelSelfData : public DbTableModel
{
    Q_OBJECT

public:
    explicit ModelSelfData(QObject *parent = 0);
    void refresh(int id_nakl, int id_type);
};

class FormSelf : public QWidget
{
    Q_OBJECT

public:
    explicit FormSelf(QWidget *parent = nullptr);
    ~FormSelf();

private:
    Ui::FormSelf *ui;
    void loadSettings();
    void saveSettings();
    ModelSelfNakl *modelSelfNakl;
    ModelSelfData *modelSelfData;
    DbMapper *mapper;

private slots:
    void upd();
    void updData(int ind);
    void calcSum();
    void nakl();
    void naklPer();
};

#endif // FORMSELF_H
