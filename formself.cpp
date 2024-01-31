#include "formself.h"
#include "ui_formself.h"

FormSelf::FormSelf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormSelf)
{
    ui->setupUi(this);
    loadSettings();

    ui->dateEditBeg->setDate(QDate(QDate::currentDate().year(),1,1));
    ui->dateEditEnd->setDate(QDate(QDate::currentDate().year(),12,31));

    ui->comboBoxType->setModel(Rels::instance()->relSelfType->model());
    if (!Rels::instance()->relSelfType->isInital()){
        Rels::instance()->relSelfType->refreshModel();
    }
    colVal c;
    c.val=4;
    ui->comboBoxType->setCurrentData(c);
    ui->comboBoxType->setEditable(false);

    modelSelfData = new ModelSelfData(this);
    ui->tableViewData->setModel(modelSelfData);
    ui->tableViewData->setColumnHidden(0,true);
    ui->tableViewData->setColumnWidth(1,400);
    ui->tableViewData->setColumnWidth(2,80);
    ui->tableViewData->setColumnHidden(3,true);

    modelSelfNakl = new ModelSelfNakl(this);
    ui->tableViewNakl->setModel(modelSelfNakl);
    ui->tableViewNakl->setColumnHidden(0,true);
    ui->tableViewNakl->setColumnWidth(1,80);
    ui->tableViewNakl->setColumnWidth(2,80);
    ui->tableViewNakl->setColumnHidden(3,true);
    ui->tableViewNakl->setColumnWidth(4,200);

    mapper = new DbMapper(ui->tableViewNakl);
    ui->horizontalLayoutMap->insertWidget(0,mapper);
    mapper->addMapping(ui->lineEditNum,1);
    mapper->addMapping(ui->dateEditNakl,2);
    mapper->addMapping(ui->lineEditPol,4);
    mapper->addLock(ui->comboBoxType);
    mapper->addEmptyLock(ui->tableViewData);
    mapper->addEmptyLock(ui->pushButtonNakl);
    mapper->addEmptyLock(ui->pushButtonNaklPer);
    mapper->addLock(ui->pushButtonUpd);
    mapper->addLock(ui->dateEditBeg);
    mapper->addLock(ui->dateEditEnd);

    connect(ui->pushButtonUpd,SIGNAL(clicked(bool)),this,SLOT(upd()));
    connect(ui->comboBoxType,SIGNAL(currentIndexChanged(int)),this,SLOT(upd()));
    connect(mapper,SIGNAL(currentIndexChanged(int)),this,SLOT(updData(int)));
    connect(modelSelfData,SIGNAL(sigUpd()),this,SLOT(calcSum()));
    connect(ui->pushButtonNakl,SIGNAL(clicked(bool)),this,SLOT(nakl()));
    connect(ui->pushButtonNaklPer,SIGNAL(clicked(bool)),this,SLOT(naklPer()));

    upd();
}

FormSelf::~FormSelf()
{
    saveSettings();
    delete ui;
}

void FormSelf::loadSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    this->ui->splitter->restoreState(settings.value("self_splitter_width").toByteArray());
}

void FormSelf::saveSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    settings.setValue("self_splitter_width",ui->splitter->saveState());
}

void FormSelf::upd()
{
    if (sender()==ui->pushButtonUpd){
        modelSelfData->refreshRelsModel();
    }
    int id_type=ui->comboBoxType->getCurrentData().val.toInt();
    modelSelfNakl->refresh(id_type,ui->dateEditBeg->date(),ui->dateEditEnd->date());
}

void FormSelf::updData(int ind)
{
    int id_nakl=mapper->modelData(ind,0).toInt();
    int id_type=ui->comboBoxType->getCurrentData().val.toInt();
    modelSelfData->refresh(id_nakl,id_type);
    calcSum();
}

void FormSelf::calcSum()
{
    double sum=0;
    if (ui->tableViewData->model()){
        for (int i=0; i<ui->tableViewData->model()->rowCount(); i++){
            sum+=ui->tableViewData->model()->data(ui->tableViewData->model()->index(i,2),Qt::EditRole).toDouble();
        }
    }
    QString title = ui->comboBoxType->currentText();
    if (sum>0){
        title += tr(" итого: ")+QLocale().toString(sum,'f',1)+tr(" кг");
    }
    ui->labelItogo->setText(title);
}

void FormSelf::nakl()
{
    int id_nakl=mapper->modelData(mapper->currentIndex(),0).toInt();
    QString vid=tr("Электроды");
    QString type=tr("Цех");
    QString filename=ui->comboBoxType->currentText().toUpper()+"_"+mapper->modelData(mapper->currentIndex(),1).toString();
    int year=mapper->modelData(mapper->currentIndex(),2).toDate().year();
    Rels::instance()->invoiceManager->getInvoice("invoices/elrtr/self/"+QString::number(id_nakl),vid,type,filename,year);
}

void FormSelf::naklPer()
{
    QString id_type=ui->comboBoxType->getCurrentData().val.toString();
    QString vid=tr("Электроды");
    QString type=tr("Цех");
    QString filename=ui->comboBoxType->currentText().toUpper()+"_"+ui->dateEditBeg->date().toString("yyyy-MM-dd")+"_"+ui->dateEditEnd->date().toString("yyyy-MM-dd");
    int year=mapper->modelData(mapper->currentIndex(),2).toDate().year();
    Rels::instance()->invoiceManager->getInvoice("invoices/elrtr/selfper/"+id_type+"/"+ui->dateEditBeg->date().toString("yyyy-MM-dd")+"/"+ui->dateEditEnd->date().toString("yyyy-MM-dd"),vid,type,filename,year);
}

ModelSelfNakl::ModelSelfNakl(QObject *parent) : DbTableModel("prod_self",parent)
{
    addColumn("id",tr("id"));
    addColumn("num",tr("Номер"));
    addColumn("dat",tr("Дата"));
    addColumn("id_cons",tr("Тип"));
    addColumn("kto",tr("Получатель"));
    setSort(name()+".dat, "+name()+".num");
}

void ModelSelfNakl::refresh(int id_type, QDate begDate, QDate endDate)
{
    QString filter=name()+".dat between '"+begDate.toString("yyyy-MM-dd")+"' and '"+endDate.toString("yyyy-MM-dd")+"' and "+name()+".id_cons = "+QString::number(id_type);
    setFilter(filter);
    setDefaultValue(3,id_type);
    setDefaultValue(1,"1");
    select();
}

bool ModelSelfNakl::insertRow(int row, const QModelIndex &parent)
{
    select();
    if (rowCount()>0 && !isAdd()) {
        int old_num=this->data(this->index(rowCount()-1,1),Qt::EditRole).toInt();
        QVariant old_date=this->data(this->index(rowCount()-1,2),Qt::EditRole);
        setDefaultValue(1,old_num+1);
        setDefaultValue(2,!old_date.isNull()? old_date.toDate().addDays(1) : QDate::currentDate());
    }
    return DbTableModel::insertRow(row,parent);
}

ModelSelfData::ModelSelfData(QObject *parent): DbTableModel("prod_self_items",parent)
{
    addColumn("id_self",tr("id_self"));
    addColumn("id_part",tr("Партия"),Rels::instance()->relElPart);
    addColumn("kvo",tr("Масса, кг"));
    addColumn("id_cons",tr("назначение"),Rels::instance()->relSelfType);
    setDecimals(2,2);
    this->setSort(name()+".id_part");
}

void ModelSelfData::refresh(int id_nakl, int id_type)
{
    setFilter(name()+".id_self = "+QString::number(id_nakl));
    setDefaultValue(0,id_nakl);
    setDefaultValue(3,id_type);
    select();
}
