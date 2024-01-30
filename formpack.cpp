#include "formpack.h"
#include "ui_formpack.h"

FormPack::FormPack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPack)
{
    ui->setupUi(this);
    loadSettings();

    ui->dateEditBeg->setDate(QDate(QDate::currentDate().year(),1,1));
    ui->dateEditEnd->setDate(QDate(QDate::currentDate().year(),12,31));

    ui->comboBoxType->setModel(Rels::instance()->relPackType->model());
    if (!Rels::instance()->relPackType->isInital()){
        Rels::instance()->relPackType->refreshModel();
    }
    colVal c;
    c.val=1;
    ui->comboBoxType->setCurrentData(c);
    ui->comboBoxType->setEditable(false);

    modelPack = new ModelNaklData("parti_pack",this);
    modelBreak = new ModelNaklData("parti_break",this);

    modelNakl = new ModelNakl(this);
    ui->tableViewNakl->setModel(modelNakl);
    ui->tableViewNakl->setColumnHidden(0,true);
    ui->tableViewNakl->setColumnWidth(1,80);
    ui->tableViewNakl->setColumnWidth(2,80);
    ui->tableViewNakl->setColumnHidden(3,true);

    mapper = new DbMapper(ui->tableViewNakl);
    ui->horizontalLayoutMap->insertWidget(0,mapper);
    mapper->addMapping(ui->lineEditNum,1);
    mapper->addMapping(ui->dateEditNakl,2);
    mapper->addLock(ui->comboBoxType);
    mapper->addEmptyLock(ui->tableViewNaklData);
    mapper->addEmptyLock(ui->pushButtonNakl);
    mapper->addEmptyLock(ui->pushButtonNaklPer);

    connect(ui->pushButtonUpd,SIGNAL(clicked(bool)),this,SLOT(upd()));
    connect(ui->comboBoxType,SIGNAL(currentIndexChanged(int)),this,SLOT(upd()));
    connect(mapper,SIGNAL(currentIndexChanged(int)),this,SLOT(updData(int)));
    connect(modelPack,SIGNAL(sigUpd()),this,SLOT(calcSum()));
    connect(modelBreak,SIGNAL(sigUpd()),this,SLOT(calcSum()));
    connect(ui->pushButtonNakl,SIGNAL(clicked(bool)),this,SLOT(nakl()));
    connect(ui->pushButtonNaklPer,SIGNAL(clicked(bool)),this,SLOT(naklPer()));

    upd();
}

FormPack::~FormPack()
{
    saveSettings();
    delete ui;
}

void FormPack::loadSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    this->ui->splitter->restoreState(settings.value("pack_splitter_width").toByteArray());
}

void FormPack::saveSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    settings.setValue("pack_splitter_width",ui->splitter->saveState());
}

void FormPack::upd()
{
    if (sender()==ui->pushButtonUpd){
        modelPack->refreshRelsModel();
    }
    int id_type=ui->comboBoxType->getCurrentData().val.toInt();
    modelNakl->refresh(id_type,ui->dateEditBeg->date(),ui->dateEditEnd->date());
}

void FormPack::updData(int ind)
{
    int id_nakl=mapper->modelData(ind,0).toInt();
    modelPack->refresh(id_nakl);
    modelBreak->refresh(id_nakl);
    if (ui->comboBoxType->getCurrentData().val==1){
        ui->pushButtonNaklPer->hide();
        ui->tableViewNaklData->setModel(modelPack);
    } else {
        ui->pushButtonNaklPer->show();
        ui->tableViewNaklData->setModel(modelBreak);
    }
    ui->tableViewNaklData->setColumnHidden(0,true);
    ui->tableViewNaklData->setColumnHidden(1,true);
    ui->tableViewNaklData->setColumnWidth(2,400);
    ui->tableViewNaklData->setColumnWidth(3,80);
    calcSum();
}

void FormPack::calcSum()
{
    double sum=0;
    if (ui->tableViewNaklData->model()){
        for (int i=0; i<ui->tableViewNaklData->model()->rowCount(); i++){
            sum+=ui->tableViewNaklData->model()->data(ui->tableViewNaklData->model()->index(i,3),Qt::EditRole).toDouble();
        }
    }
    QString title = ui->comboBoxType->currentText();
    if (sum>0){
        title += tr(" итого: ")+QLocale().toString(sum,'f',1)+tr(" кг");
    }
    ui->labelItogo->setText(title);
}

void FormPack::nakl()
{
    int id_nakl=mapper->modelData(mapper->currentIndex(),0).toInt();
    QString vid=tr("Электроды");
    QString type=tr("Цех");
    QString filename=ui->comboBoxType->currentText().toUpper()+"_"+mapper->modelData(mapper->currentIndex(),1).toString();
    int year=mapper->modelData(mapper->currentIndex(),2).toDate().year();
    Rels::instance()->invoiceManager->getInvoice("invoices/elrtr/workshop/"+QString::number(id_nakl),vid,type,filename,year);
}

void FormPack::naklPer()
{
    QString vid=tr("Электроды");
    QString type=tr("Цех");
    QString filename=ui->comboBoxType->currentText().toUpper()+"_"+ui->dateEditBeg->date().toString("yyyy-MM-dd")+"_"+ui->dateEditEnd->date().toString("yyyy-MM-dd");
    int year=mapper->modelData(mapper->currentIndex(),2).toDate().year();
    Rels::instance()->invoiceManager->getInvoice("invoices/elrtr/workshopper/"+ui->dateEditBeg->date().toString("yyyy-MM-dd")+"/"+ui->dateEditEnd->date().toString("yyyy-MM-dd"),vid,type,filename,year);
}

ModelNakl::ModelNakl(QObject *parent) : DbTableModel("parti_nakl",parent)
{
    addColumn("id",tr("id"));
    addColumn("num",tr("Номер"));
    addColumn("dat",tr("Дата"));
    addColumn("tip",tr("Тип"));
    setSort(name()+".dat, "+name()+".num");
}

void ModelNakl::refresh(int id_type, QDate begDate, QDate endDate)
{
    QString filter=name()+".dat between '"+begDate.toString("yyyy-MM-dd")+"' and '"+endDate.toString("yyyy-MM-dd")+"' and "+name()+".tip = "+QString::number(id_type);
    setFilter(filter);
    setDefaultValue(3,id_type);
    setDefaultValue(1,"1");
    select();
}

bool ModelNakl::insertRow(int row, const QModelIndex &parent)
{
    select();
    if (rowCount()>0 && !isAdd()) {
        int old_num=this->data(this->index(rowCount()-1,1),Qt::EditRole).toInt();
        setDefaultValue(1,old_num+1);
    }
    setDefaultValue(2,QDate::currentDate());
    return DbTableModel::insertRow(row,parent);
}

ModelNaklData::ModelNaklData(QString table, QObject *parent) : DbTableModel(table,parent)
{
    addColumn("id",tr("id"));
    addColumn("id_nakl",tr("id_nakl"));
    addColumn("id_part",tr("Партия"),Rels::instance()->relElPart);
    addColumn("kvo",tr("Масса, кг"));
    setDecimals(3,2);
    this->setSort(name()+".id");
}

void ModelNaklData::refresh(int id_nakl)
{
    setFilter(name()+".id_nakl = "+QString::number(id_nakl));
    setDefaultValue(1,id_nakl);
    select();
}
