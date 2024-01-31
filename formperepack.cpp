#include "formperepack.h"
#include "ui_formperepack.h"

FormPerePack::FormPerePack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPerePack)
{
    ui->setupUi(this);
    loadSettings();

    ui->dateEditBeg->setDate(QDate(QDate::currentDate().year(),1,1));
    ui->dateEditEnd->setDate(QDate(QDate::currentDate().year(),12,31));

    modelPerePack = new ModelPerePack(this);
    ui->tableViewData->setModel(modelPerePack);
    ui->tableViewData->setColumnHidden(0,true);
    ui->tableViewData->setColumnHidden(1,true);
    ui->tableViewData->setColumnWidth(2,370);
    ui->tableViewData->setColumnWidth(3,370);
    ui->tableViewData->setColumnWidth(4,90);
    ui->tableViewData->setColumnWidth(5,90);

    modelNakl = new ModelNakl(this);
    ui->tableViewNakl->setModel(modelNakl);
    ui->tableViewNakl->setColumnHidden(0,true);
    ui->tableViewNakl->setColumnWidth(1,90);
    ui->tableViewNakl->setColumnWidth(2,90);
    ui->tableViewNakl->setColumnHidden(3,true);

    mapper = new DbMapper(ui->tableViewNakl);
    ui->horizontalLayoutMap->insertWidget(0,mapper);
    mapper->addMapping(ui->lineEditNum,1);
    mapper->addMapping(ui->dateEditNakl,2);
    mapper->addEmptyLock(ui->tableViewData);
    mapper->addEmptyLock(ui->pushButtonNakl);
    mapper->addLock(ui->pushButtonUpd);
    mapper->addLock(ui->dateEditBeg);
    mapper->addLock(ui->dateEditEnd);

    connect(ui->pushButtonUpd,SIGNAL(clicked(bool)),this,SLOT(upd()));
    connect(mapper,SIGNAL(currentIndexChanged(int)),this,SLOT(updData(int)));
    connect(modelPerePack,SIGNAL(sigUpd()),this,SLOT(calcSum()));
    connect(ui->pushButtonNakl,SIGNAL(clicked(bool)),this,SLOT(nakl()));

    upd();
}

FormPerePack::~FormPerePack()
{
    saveSettings();
    delete ui;
}

void FormPerePack::loadSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    this->ui->splitter->restoreState(settings.value("perepack_splitter_width").toByteArray());
}

void FormPerePack::saveSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    settings.setValue("perepack_splitter_width",ui->splitter->saveState());
}

void FormPerePack::upd()
{
    if (sender()==ui->pushButtonUpd){
        modelNakl->refreshRelsModel();
    }
    modelNakl->refresh(7,ui->dateEditBeg->date(),ui->dateEditEnd->date());
}

void FormPerePack::updData(int ind)
{
    int id_nakl=mapper->modelData(ind,0).toInt();
    modelPerePack->refresh(id_nakl);
    calcSum();
}

void FormPerePack::calcSum()
{
    double sum=0;
    if (ui->tableViewData->model()){
        for (int i=0; i<ui->tableViewData->model()->rowCount(); i++){
            sum+=ui->tableViewData->model()->data(ui->tableViewData->model()->index(i,4),Qt::EditRole).toDouble();
        }
    }
    QString title = tr("Переупаковка");
    if (sum>0){
        title += tr(" итого: ")+QLocale().toString(sum,'f',1)+tr(" кг");
    }
    ui->labelItogo->setText(title);
}

void FormPerePack::nakl()
{
    int id_nakl=mapper->modelData(mapper->currentIndex(),0).toInt();
    QString vid=tr("Электроды");
    QString type=tr("Переупаковка");
    QString filename=tr("ПЕРЕУПАКОВКА_")+mapper->modelData(mapper->currentIndex(),1).toString();
    int year=mapper->modelData(mapper->currentIndex(),2).toDate().year();
    Rels::instance()->invoiceManager->getInvoice("invoices/elrtr/perepack/"+QString::number(id_nakl),vid,type,filename,year);
}

ModelPerePack::ModelPerePack(QObject *parent) : DbTableModel("parti_perepack",parent)
{
    addColumn("id",tr("id"));
    addColumn("id_nakl",tr("id_nakl"));
    addColumn("id_part",tr("Старая партия"),Rels::instance()->relElPart);
    addColumn("id_new_part",tr("Новая партия"),Rels::instance()->relElPartNew);
    addColumn("kvo",tr("Масса, кг"));
    addColumn("kvo_break",tr("В т.ч.брак,кг"));
    setDecimals(4,2);
    setDecimals(5,2);
    this->setSort(name()+".id");
}

void ModelPerePack::refresh(int id_nakl)
{
    setFilter(name()+".id_nakl = "+QString::number(id_nakl));
    setDefaultValue(1,id_nakl);
    select();
}
