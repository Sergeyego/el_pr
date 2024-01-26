#include "formpart.h"
#include "ui_formpart.h"

FormPart::FormPart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPart)
{
    ui->setupUi(this);

    loadSettings();

    ui->dateEditBeg->setDate(QDate(QDate::currentDate().year(),1,1));
    ui->dateEditEnd->setDate(QDate(QDate::currentDate().year(),12,31));

    modelPart = new ModelPart(this);
    ui->tableViewPart->setModel(modelPart);
    ui->tableViewPart->setColumnHidden(0,true);
    for (int i=7; i<ui->tableViewPart->model()->columnCount(); i++){
        ui->tableViewPart->setColumnHidden(i,true);
    }
    ui->tableViewPart->setColumnWidth(1,50);
    ui->tableViewPart->setColumnWidth(2,70);
    ui->tableViewPart->setColumnWidth(3,130);
    ui->tableViewPart->setColumnWidth(4,40);
    ui->tableViewPart->setColumnWidth(5,90);
    ui->tableViewPart->setColumnWidth(6,100);

    mapper = new DbMapper(ui->tableViewPart,this);
    ui->horizontalLayoutMapper->insertWidget(0,mapper);

    mapper->addMapping(ui->lineEditPart,1);
    mapper->addMapping(ui->dateEditPart,2);
    mapper->addMapping(ui->comboBoxMark,3);
    mapper->addMapping(ui->lineEditDiam,4);
    mapper->addMapping(ui->comboBoxSrc,5);
    mapper->addMapping(ui->comboBoxVar,6);
    mapper->addMapping(ui->lineEditPlan,7);
    mapper->addMapping(ui->comboBoxRcp,8);
    mapper->addMapping(ui->comboBoxWire,9);
    mapper->addMapping(ui->lineEditpartWire,10);
    mapper->addMapping(ui->comboBoxPack,11);
    mapper->addMapping(ui->comboBoxLen,12);
    mapper->addMapping(ui->plainTextEditNote,13);
    mapper->addMapping(ui->lineEditPlot,14);
    mapper->addMapping(ui->lineEditVyaz,15);
    mapper->addMapping(ui->lineEditObm,16);
    mapper->addMapping(ui->lineEditOst,17);
    mapper->addMapping(ui->lineEditKfmp,18);
    mapper->addMapping(ui->lineEditDl,19);
    mapper->addMapping(ui->lineEditPokr,20);
    mapper->addMapping(ui->lineEditFil,21);
    mapper->addMapping(ui->lineEditDel,22);
    mapper->addMapping(ui->lineEditMassDry,23);
    mapper->addMapping(ui->lineEditMassGl,24);

    mapper->setDefaultFocus(1);
    mapper->addLock(ui->dateEditBeg);
    mapper->addLock(ui->dateEditEnd);
    mapper->addLock(ui->pushButtonUpd);
    mapper->addLock(ui->comboBoxOnly);
    mapper->addEmptyLock(ui->tableViewGlass);
    mapper->addEmptyLock(ui->tableViewGlassPar);
    mapper->addEmptyLock(ui->tableViewGlassMeas);
    mapper->addEmptyLock(ui->tableViewDoz);
    mapper->addEmptyLock(ui->tableViewPress);
    mapper->addEmptyLock(ui->tableViewChem);
    mapper->addEmptyLock(ui->tableViewMech);
    mapper->addEmptyLock(ui->pushButtonChem);
    mapper->addEmptyLock(ui->pushButtonSamp);

    ui->comboBoxOnly->setModel(Rels::instance()->relMark->model());
    ui->comboBoxOnly->setModelColumn(1);
    ui->comboBoxOnly->setCurrentIndex(-1);

    connect(mapper,SIGNAL(currentIndexChanged(int)),this,SLOT(refreshCont(int)));
    connect(ui->pushButtonUpd,SIGNAL(clicked(bool)),this,SLOT(updPart()));
    connect(ui->comboBoxOnly,SIGNAL(currentIndexChanged(int)),this,SLOT(updPart()));
    connect(ui->checkBoxOnly,SIGNAL(clicked(bool)),this,SLOT(updPart()));

    connect(ui->checkBoxOnly,SIGNAL(clicked(bool)),ui->comboBoxOnly,SLOT(setEnabled(bool)));

    updPart();
}

FormPart::~FormPart()
{
    saveSettings();
    delete ui;
}

void FormPart::loadSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    this->ui->splitter->restoreState(settings.value("part_splitter_width").toByteArray());
    ui->tabWidget->setCurrentIndex(settings.value("part_tab_index").toInt());
}

void FormPart::saveSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    settings.setValue("part_splitter_width",ui->splitter->saveState());
    settings.setValue("part_tab_index",ui->tabWidget->currentIndex());
}

void FormPart::updPart()
{
    int id_el=-1;
    if (ui->checkBoxOnly->isChecked()){
        if (sender()==ui->checkBoxOnly){
            colVal d;
            d.val=mapper->modelData(mapper->currentIndex(),3).toInt();
            ui->comboBoxOnly->setCurrentData(d);
        }

        id_el=ui->comboBoxOnly->getCurrentData().val.toInt();
    }
    if (sender()==ui->pushButtonUpd){
        modelPart->refreshRelsModel();
    }
    modelPart->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date(),id_el);
}

void FormPart::refreshCont(int /*ind*/)
{

}

ModelPart::ModelPart(QObject *parent) : DbTableModel("parti",parent)
{
    addColumn("id",QString::fromUtf8("id"));
    addColumn("n_s",QString::fromUtf8("Парт."));
    addColumn("dat_part",QString::fromUtf8("Дата"));
    addColumn("id_el",QString::fromUtf8("Марка"),Rels::instance()->relMark);
    addColumn("diam",QString::fromUtf8("Диам."));
    addColumn("id_ist",QString::fromUtf8("Источник"),Rels::instance()->relSrc);
    addColumn("id_var",QString::fromUtf8("Вариант"),Rels::instance()->relVar);
    addColumn("ob_em",QString::fromUtf8("План"));
    addColumn("id_rcp",QString::fromUtf8("Рецептура"),Rels::instance()->relRcp);
    addColumn("id_prfact",QString::fromUtf8("Проволока"),Rels::instance()->relProv);
    addColumn("parti_prov",QString::fromUtf8("Партия пров."));
    addColumn("id_pack",QString::fromUtf8("Упаковка"),Rels::instance()->relPack);
    addColumn("id_long",QString::fromUtf8("Длина"),Rels::instance()->relLong);
    addColumn("prim",QString::fromUtf8("Примечание"));
    addColumn("dens",QString::fromUtf8("Плотность"));
    addColumn("visc",QString::fromUtf8("Вязкозть"));
    addColumn("liq_glass",QString::fromUtf8("% сод.в обм."));
    addColumn("dry_rst",QString::fromUtf8("% сух. ост."));
    addColumn("kfmp",QString::fromUtf8("кфмп"));
    addColumn("le",QString::fromUtf8("Дл. эл-да"));
    addColumn("lo",QString::fromUtf8("Дл. покр."));
    addColumn("dfil",QString::fromUtf8("ф филь."));
    addColumn("del",QString::fromUtf8("ф эл."));
    addColumn("mas_dry",QString::fromUtf8("Масса шихты"));
    addColumn("mas_lqgl",QString::fromUtf8("Масса ж.стекла"));

    setDecimals(14,3);
    setDecimals(16,2);
    setDecimals(17,2);
    setDecimals(18,2);
    setDecimals(21,2);
    setDecimals(22,2);

    setDefaultValue(6,1);

    setSort("parti.n_s, parti.dat_part");
}

QVariant ModelPart::data(const QModelIndex &index, int role) const
{
    if((role == Qt::BackgroundColorRole)) {
        int area = colorState.value(data(this->index(index.row(),0),Qt::EditRole).toInt());
        if(area == 1) return QVariant(QColor(255,170,170)); else
            if(area == 2) return QVariant(QColor(170,255,170)); else
                if(area == 3) return QVariant(QColor(Qt::yellow)); else
                    return DbTableModel::data(index,role);
    } else return DbTableModel::data(index,role);
}

void ModelPart::refresh(const QDate &beg, const QDate &end, int id_el)
{
    dBeg=beg;
    dEnd=end;
    QString f="parti.dat_part between '"+beg.toString("yyyy-MM-dd")+"' and '"+end.toString("yyyy-MM-dd")+"'";
    if (id_el>=0){
        f.append(" and parti.id_el = "+QString::number(id_el));
    }
    setFilter(f);
    refreshState();
    select();
}

bool ModelPart::insertRow(int row, const QModelIndex &parent)
{
    refresh(dBeg,dEnd,-1);
    int old_num=0;
    if (rowCount()>0) old_num=this->data(this->index(rowCount()-1,1),Qt::EditRole).toInt();
    setDefaultValue(1,QString("%1").arg((old_num+1),4,'d',0,QChar('0')));
    setDefaultValue(2,QDate::currentDate());
    return DbTableModel::insertRow(row,parent);
}

void ModelPart::refreshState()
{
    QSqlQuery query;
    query.prepare("select parti.id, "
                  "(select case when exists(select id from parti_chem where id_part=parti.id) "
                  "then 1 else 0 end "
                  "+ "
                  "case when exists(select id from parti_mech where id_part=parti.id) "
                  "then 2 else 0 end "
                  "as r) from parti where parti.dat_part between :d1 and :d2");
    query.bindValue(":d1",dBeg);
    query.bindValue(":d2",dEnd);
    if (query.exec()){
        colorState.clear();
        while (query.next()){
            colorState[query.value(0).toInt()]=query.value(1).toInt();
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
}
