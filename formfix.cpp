#include "formfix.h"
#include "ui_formfix.h"

FormFix::FormFix(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormFix)
{
    ui->setupUi(this);

    modelDat = new QSqlQueryModel(this);
    ui->comboBoxDate->setModel(modelDat);

    modelFix = new DbTableModel("prod_ost",this);
    modelFix->addColumn("dat",tr("Дата"));
    modelFix->addColumn("id_part",tr("Партия"),Rels::instance()->relElPart);
    modelFix->addColumn("kvo",tr("Кол-во, кг"));

    modelFix->setSort("parti.str");

    ui->dateEditFix->setDate(QDate::currentDate().addDays(-QDate::currentDate().day()));
    modelFix->setDefaultValue(0,ui->dateEditFix->date());

    ui->tableViewOst->setModel(modelFix);
    ui->tableViewOst->setColumnHidden(0,true);
    ui->tableViewOst->setColumnWidth(1,400);
    ui->tableViewOst->setColumnWidth(2,80);

    connect(ui->comboBoxDate,SIGNAL(currentIndexChanged(int)),this,SLOT(updFixData(int)));
    connect(ui->pushButtonFix,SIGNAL(clicked(bool)),this,SLOT(fixNewOst()));
    connect(ui->pushButtonUpd,SIGNAL(clicked(bool)),this,SLOT(updFix()));

    updFix();
}

FormFix::~FormFix()
{
    delete ui;
}

void FormFix::updFix()
{
    modelDat->setQuery("select distinct dat from prod_ost order by dat desc");
    if (modelDat->lastError().isValid()){
        QMessageBox::critical(this,"Error",modelDat->lastError().text(),QMessageBox::Cancel);
    }
    if (modelDat->rowCount()){
        ui->comboBoxDate->setCurrentIndex(0);
    } else {
        modelFix->select();
    }
}

void FormFix::updFixData(int index)
{
    if (index>=0){
        QDate date=ui->comboBoxDate->model()->data(ui->comboBoxDate->model()->index(index,0),Qt::EditRole).toDate();
        modelFix->setDefaultValue(0,date);
        modelFix->setFilter(QString("prod_ost.dat = '%1'").arg(date.toString("yyyy-MM-dd")));
        modelFix->select();
    }
}

void FormFix::fixNewOst()
{
    QDate d=ui->dateEditFix->date();
    if (d.day()!=d.daysInMonth()){
        QMessageBox::information(this,tr("Внимание"),tr("Дата должна быть последним числом месяца!"),QMessageBox::Cancel);
    } else {
        int n=QMessageBox::question(this,tr("Подтвердите действие"),tr("Зафиксировать остатки на ")+d.toString("dd.MM.yy")+tr("?"),QMessageBox::Yes,QMessageBox::No);
        if (n==QMessageBox::Yes){
            delOst(d);
            QSqlQuery query;
            query.prepare("insert into prod_ost (dat, id_part, kvo) (select :d1, p.id_part, p.ostend from calc_prod(:d2) as p where p.ostend<>0)");
            query.bindValue(":d1",d);
            query.bindValue(":d2",d);
            if (!query.exec()){
                QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Cancel);
            } else {
                updFix();
            }
        }
    }
}

void FormFix::delOst(QDate date)
{
    QSqlQuery query;
    query.prepare("delete from prod_ost where dat = :d ");
    query.bindValue(":d",date);
    if (!query.exec()){
        QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Cancel);
    } else {
        updFix();
    }
}
