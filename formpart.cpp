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

    modelGlass = new DbTableModel("acc_glyba",this);
    modelGlass->addColumn("id","id");
    modelGlass->addColumn("id_part","id_part");
    modelGlass->addColumn("id_glyb_part",QString::fromUtf8("Стекло"),Rels::instance()->relGlass);
    modelGlass->addColumn("id_cons",QString::fromUtf8("Расходник"),Rels::instance()->relCons);
    modelGlass->setSort("acc_glyba.id");

    ui->tableViewGlass->setModel(modelGlass);
    ui->tableViewGlass->setColumnHidden(0,true);
    ui->tableViewGlass->setColumnHidden(1,true);
    ui->tableViewGlass->setColumnWidth(2,100);
    ui->tableViewGlass->setColumnWidth(3,100);

    modelZam = new DbTableModel("parti_mix",this);
    modelZam->addColumn("id_part","id_part");
    modelZam->addColumn("id_dos",QString::fromUtf8("Партия дозировки"),Rels::instance()->relDos);
    modelZam->addColumn("kvo",QString::fromUtf8("К-во, кг"));
    modelZam->setSort("parti_mix.id_dos");

    ui->tableViewDoz->setModel(modelZam);
    ui->tableViewDoz->setColumnHidden(0,true);
    ui->tableViewDoz->setColumnWidth(1,320);
    ui->tableViewDoz->setColumnWidth(2,80);

    modelZamBreak = new DbTableModel("parti_zam_break",this);
    modelZamBreak->addColumn("id_part","id_part");
    modelZamBreak->addColumn("dat",QString::fromUtf8("Дата"));
    modelZamBreak->addColumn("kvo",QString::fromUtf8("В брак, кг"));
    modelZamBreak->setSort("parti_zam_break.dat");

    ui->tableViewDozDef->setModel(modelZamBreak);
    ui->tableViewDozDef->setColumnHidden(0,true);
    ui->tableViewDozDef->setColumnWidth(1,80);
    ui->tableViewDozDef->setColumnWidth(2,70);

    modelRab = new DbTableModel("part_prod",this);
    modelRab->addColumn("id","id");
    modelRab->addColumn("id_part","id_part");
    modelRab->addColumn("dat",QString::fromUtf8("Дата"));
    modelRab->addColumn("id_press",QString::fromUtf8("Пресс"),Rels::instance()->relPress);
    modelRab->addColumn("id_brig",QString::fromUtf8("Бригадир"),Rels::instance()->relRab);
    modelRab->addColumn("kvo",QString::fromUtf8("Кол-во,кг"));
    modelRab->addColumn("davl",QString::fromUtf8("Давлен."));
    modelRab->addColumn("loss",QString::fromUtf8("Отх.ших."));
    modelRab->addColumn("rods",QString::fromUtf8("Пров.,кг"));
    modelRab->addColumn("garb",QString::fromUtf8("Отх.мус."));
    modelRab->setSort("part_prod.dat");

    ui->tableViewPress->setModel(modelRab);
    ui->tableViewPress->setColumnHidden(0,true);
    ui->tableViewPress->setColumnHidden(1,true);
    ui->tableViewPress->setColumnWidth(2,75);
    ui->tableViewPress->setColumnWidth(3,150);
    ui->tableViewPress->setColumnWidth(4,130);
    ui->tableViewPress->setColumnWidth(5,70);
    ui->tableViewPress->setColumnWidth(6,70);
    ui->tableViewPress->setColumnWidth(7,70);
    ui->tableViewPress->setColumnWidth(8,70);
    ui->tableViewPress->setColumnWidth(9,70);

    ui->comboBoxChemDev->setModel(Rels::instance()->relChemDev->model());
    ui->comboBoxChemDev->setModelColumn(1);
    colVal cDev;
    cDev.val=1;
    ui->comboBoxChemDev->setCurrentData(cDev);

    modelChem = new ModelChemSrc(this);
    ui->tableViewChem->setModel(modelChem);
    ui->tableViewChem->setColumnHidden(0,true);
    ui->tableViewChem->setColumnHidden(1,true);
    ui->tableViewChem->setColumnWidth(2,80);
    ui->tableViewChem->setColumnWidth(3,70);
    ui->tableViewChem->setColumnWidth(4,70);
    ui->tableViewChem->setColumnWidth(5,110);

    modelMech = new ModelMechSrc(this);
    ui->tableViewMech->setModel(modelMech);
    ui->tableViewMech->setColumnHidden(0,true);
    ui->tableViewMech->setColumnWidth(1,180);
    ui->tableViewMech->setColumnWidth(2,80);

    modelConsStatData = new ModelConsStatData(this);
    modelConsStatData->refresh(-1);
    ui->tableViewGlassData->setModel(modelConsStatData);
    ui->tableViewGlassData->setColumnHidden(0,true);
    ui->tableViewGlassData->setColumnWidth(1,40);
    ui->tableViewGlassData->setColumnWidth(2,70);
    ui->tableViewGlassData->setColumnWidth(3,60);
    ui->tableViewGlassData->setColumnWidth(4,70);
    ui->tableViewGlassData->setColumnHidden(5,true);

    modelConsStatPar = new ModelConsStatPar(this);
    modelConsStatPar->refresh(-1,-1);
    ui->tableViewGlassPar->setModel(modelConsStatPar);
    ui->tableViewGlassPar->setColumnHidden(0,true);
    ui->tableViewGlassPar->setColumnWidth(1,80);
    ui->tableViewGlassPar->setColumnWidth(2,60);
    ui->tableViewGlassPar->setColumnWidth(3,70);
    ui->tableViewGlassPar->setColumnWidth(4,80);

    modelPackEl = new ModelPackEl(this);
    ui->tableViewPack->setModel(modelPackEl);

    modelPerePackEl = new ModelPerePackEl(this);
    ui->tableViewPerePack->setModel(modelPerePackEl);

    modelStockEl = new ModelStockEl(this);
    ui->tableViewTrans->setModel(modelStockEl);

    modelSelfEl = new ModelSelfEl(this);
    ui->tableViewSelf->setModel(modelSelfEl);

    modelShipEl = new ModelShipEl(this);
    ui->tableViewShip->setModel(modelShipEl);

    modelBreakEl = new ModelBreakEl(this);
    ui->tableViewDef->setModel(modelBreakEl);

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
    mapper->addEmptyLock(ui->tableViewGlassData);
    mapper->addEmptyLock(ui->tableViewDoz);
    mapper->addEmptyLock(ui->tableViewDozDef);
    mapper->addEmptyLock(ui->tableViewPress);
    mapper->addEmptyLock(ui->tableViewChem);
    mapper->addEmptyLock(ui->tableViewMech);
    mapper->addEmptyLock(ui->pushButtonChem);
    mapper->addEmptyLock(ui->pushButtonSamp);
    mapper->addEmptyLock(ui->checkBoxOnly);
    mapper->addEmptyLock(ui->comboBoxChemDev);

    ui->comboBoxOnly->setModel(Rels::instance()->relMark->model());
    ui->comboBoxOnly->setModelColumn(1);
    ui->comboBoxOnly->setCurrentIndex(-1);

    connect(mapper,SIGNAL(currentIndexChanged(int)),this,SLOT(refreshCont(int)));
    connect(ui->pushButtonUpd,SIGNAL(clicked(bool)),this,SLOT(updPart()));
    connect(ui->comboBoxOnly,SIGNAL(currentIndexChanged(int)),this,SLOT(updPart()));
    connect(ui->checkBoxOnly,SIGNAL(clicked(bool)),this,SLOT(updPart()));

    connect(ui->checkBoxOnly,SIGNAL(clicked(bool)),ui->comboBoxOnly,SLOT(setEnabled(bool)));

    connect(ui->comboBoxRcp,SIGNAL(currentIndexChanged(int)),this,SLOT(insertMark()));
    connect(ui->comboBoxMark,SIGNAL(currentIndexChanged(int)),this,SLOT(insertProvol()));
    connect(ui->lineEditDiam,SIGNAL(editingFinished()),this,SLOT(insertPack()));

    connect(ui->comboBoxChemDev,SIGNAL(currentIndexChanged(int)),this,SLOT(setCurrentChemDev()));
    connect(ui->pushButtonChem,SIGNAL(clicked(bool)),this,SLOT(loadChem()));
    connect(ui->pushButtonSamp,SIGNAL(clicked(bool)),this,SLOT(insertChemSamp()));

    connect(ui->tableViewGlass->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(refreshGlassData(QModelIndex)));

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

void FormPart::refreshStat(QGroupBox *g, TableView *v)
{
    ModelStat *model = qobject_cast<ModelStat *>(v->model());
    if (model){
        int id_part=mapper->modelData(mapper->currentIndex(),0).isNull() ? -1 : mapper->modelData(mapper->currentIndex(),0).toInt();
        model->refresh(id_part);
        g->setTitle(model->getTitle());
        if (model->rowCount()){
            g->show();
            v->resizeToContents();
#if (QT_VERSION >= QT_VERSION_CHECK(5, 11, 0))
            int tw=5+g->fontMetrics().horizontalAdvance(g->title());
#else
            int tw=5+g->fontMetrics().width(g->title());
#endif
            int w=30+v->verticalHeader()->frameSize().width();
            for (int i=0; i<v->model()->columnCount(); i++){
                if (!v->isColumnHidden(i)){
                    w+=v->columnWidth(i);
                }
            }
            w=(tw>w)? tw : w;
            g->setMinimumSize(w,0);
        } else {
            g->hide();
        }
    }
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

void FormPart::refreshCont(int ind)
{
    int id_part=mapper->modelData(ind,0).isNull() ? -1 : mapper->modelData(ind,0).toInt();
    QDate dat_part=mapper->modelData(ind,2).toDate();

    modelGlass->setFilter("acc_glyba.id_part = "+QString::number(id_part));
    modelGlass->setDefaultValue(1,id_part);
    modelGlass->select();
    ui->tableViewGlass->setCurrentIndex(ui->tableViewGlass->model()->index(0,1));

    modelZam->setFilter("parti_mix.id_part = "+QString::number(id_part));
    modelZam->setDefaultValue(0,id_part);
    modelZam->select();

    modelZamBreak->setFilter("parti_zam_break.id_part = "+QString::number(id_part));
    modelZamBreak->setDefaultValue(0,id_part);
    modelZamBreak->setDefaultValue(1,dat_part);
    modelZamBreak->select();

    modelRab->setFilter("part_prod.id_part = "+QString::number(id_part));
    modelRab->setDefaultValue(1,id_part);
    modelRab->setDefaultValue(2,dat_part);
    modelRab->select();

    modelChem->refresh(id_part);
    modelMech->refresh(id_part);

    refreshStat(ui->groupBoxPack,ui->tableViewPack);
    refreshStat(ui->groupBoxPerepack,ui->tableViewPerePack);
    refreshStat(ui->groupBoxTrans,ui->tableViewTrans);
    refreshStat(ui->groupBoxSelf,ui->tableViewSelf);
    refreshStat(ui->groupBoxShip,ui->tableViewShip);
    refreshStat(ui->groupBoxDef,ui->tableViewDef);
}

void FormPart::setCurrentChemDev()
{
    int id_dev = ui->comboBoxChemDev->getCurrentData().val.toInt();
    modelChem->setDefaultValue(4,id_dev);
    modelChem->select();
}

void FormPart::loadChem()
{
    DialogLoadChem d;
    if (d.exec()==QDialog::Accepted){
        QList <int> l = modelChem->ids();
        foreach (int key,l){
            QString chem=Rels::instance()->relChem->getDisplayValue(key);
            double val=d.chemVal(chem);
            if (val>0){
                modelChem->addChem(key,val,2);
            }
        }
    }
    modelChem->select();
    modelPart->refreshState();
}

void FormPart::insertChemSamp()
{
    QList<int> ids;
    int id_dev=ui->comboBoxChemDev->getCurrentData().val.toInt();
    QList <int> l = modelChem->ids();
    foreach (int key,l){
        int id=modelChem->addChem(key,0.0,id_dev);
        ids.push_back(id);
    }
    DialogTmp dt;
    dt.load(mapper->modelData(mapper->currentIndex(),0).toInt(),id_dev,ids);
    dt.exec();
    modelChem->select();
    modelPart->refreshState();
}

void FormPart::refreshGlassData(QModelIndex index)
{
    QVariant id_c=ui->tableViewGlass->model()->data(ui->tableViewGlass->model()->index(index.row(),3),Qt::EditRole);
    int id_cons = id_c.isNull()? -1 : id_c.toInt();
    int id_part = mapper->modelData(mapper->currentIndex(),0).toInt();
    int id_load=-1;

    QSqlQuery query;
    query.prepare("select gcl.id  from glass_cons_load as gcl "
                  "where gcl.dat_load = (select max(gcl2.dat_load) from glass_cons_load as gcl2 "
                  "where gcl2.dat_load<=(select p.dat_part from parti as p where p.id = :id_part ) "
                  "and gcl2.id_cons=gcl.id_cons) and gcl.id_cons = :id_cons ");
    query.bindValue(":id_part",id_part);
    query.bindValue(":id_cons",id_cons);
    if (query.exec()){
        while (query.next()){
            id_load = query.value(0).toInt();
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }

    modelConsStatData->refresh(id_load);
    modelConsStatPar->refresh(id_load,id_part);
    ui->tableViewGlassData->resizeColumnsToContents();
    ui->tableViewGlassPar->resizeColumnsToContents();
}

void FormPart::insertMark()
{
    int id_rcp=ui->comboBoxRcp->getCurrentData().val.toInt();
    if (modelPart->isAdd() && id_rcp>0){
        colVal id_el;
        id_el.val = modelPart->sqlRelation(8)->getDisplayValue(id_rcp,"id_el").toInt();
        ui->comboBoxMark->setCurrentData(id_el);
    }
}

void FormPart::insertProvol()
{
    int id_el=ui->comboBoxMark->getCurrentData().val.toInt();
    if (modelPart->isAdd() && id_el>0){
        colVal id_pr;
        id_pr.val = modelPart->sqlRelation(3)->getDisplayValue(id_el,"id_gost").toInt();
        ui->comboBoxWire->setCurrentData(id_pr);
    }
}

void FormPart::insertPack()
{
    int id_el=ui->comboBoxMark->getCurrentData().val.toInt();
    double diam = ui->lineEditDiam->text().toDouble();
    if (modelPart->isAdd() && id_el>0 && diam>0){
        colVal id_pack, id_long, id_var;
        QSqlQuery query;
        query.prepare("select p.id_pack, p.id_long, p.id_var, count(p.id_pack) as stat "
                      "from parti p "
                      "where p.dat_part >= :dat and p.id_el = :id_el and p.diam = :diam "
                      "group by p.id_pack, p.id_long, p.id_var order by stat desc");
        query.bindValue(":dat",QDate::currentDate().addDays(-365));
        query.bindValue(":id_el",id_el);
        query.bindValue(":diam", diam);
        if (query.exec()){
            if (query.next()){
                id_pack.val=query.value(0);
                ui->comboBoxPack->setCurrentData(id_pack);
                id_long.val=query.value(1);
                ui->comboBoxLen->setCurrentData(id_long);
                id_var.val=query.value(2);
                ui->comboBoxVar->setCurrentData(id_var);
            }
        } else {
            QMessageBox::critical(nullptr,tr("Ошибка"),query.lastError().text(),QMessageBox::Ok);
        }
    }
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

ModelPackEl::ModelPackEl(QObject *parent) : ModelStat(parent)
{

}

void ModelPackEl::refresh(int id_part)
{
    double sum=0;
    QString title=tr("Упаковка");
    QSqlQuery query;
    query.prepare("select pn.dat, pn.num, pp.kvo  from parti_pack pp "
                  "inner join parti_nakl pn on pn.id = pp.id_nakl "
                  "where pp.id_part = :id_part order by pn.dat, pn.num");
    query.bindValue(":id_part",id_part);
    if (execQuery(query)){
        setHeaderData(0,Qt::Horizontal,tr("Дата"));
        setHeaderData(1,Qt::Horizontal,tr("№ нак."));
        setHeaderData(2,Qt::Horizontal,tr("К-во, кг"));
        for (int i=0; i<rowCount(); i++){
            sum+=data(index(i,2),Qt::EditRole).toDouble();
        }
    }
    QString s;
    s = (sum!=0)? (title + tr(" итого: ")+QLocale().toString(sum,'f',1)+tr(" кг")) : title;
    setTitle(s);
}

ModelPerePackEl::ModelPerePackEl(QObject *parent) : ModelStat(parent)
{

}

void ModelPerePackEl::refresh(int id_part)
{
    double sum=0;
    double sumBreak=0;
    QString title=tr("Переупаковка");
    QSqlQuery query;
    query.prepare(QString::fromUtf8("select * from ( "
                  "(select pn.dat as dat, pn.num as num, "
                  "(case when pp.id_new_part<>0 then'Переуп. в парт. '||p.n_s ||'-'||date_part('year',p.dat_part) else 'Брак при переуп.' end) as part, "
                  "pp.kvo*(-1) as kvo, pp.kvo_break as break "
                  "from parti_perepack pp "
                  "inner join parti_nakl pn on pn.id = pp.id_nakl "
                  "inner join parti p on p.id = pp.id_new_part "
                  "where pp.id_part = :id_part and  pn.tip = 7 ) "
                  "union "
                  "(select pn.dat as dat, pn.num as num, 'Переуп. из парт. '||p.n_s ||'-'||date_part('year',p.dat_part) as part, "
                  "pp.kvo as kvo, NULL as break "
                  "from parti_perepack pp "
                  "inner join parti_nakl pn on pn.id = pp.id_nakl "
                  "inner join parti p on p.id = pp.id_part "
                  "where pp.id_new_part = :id_new_part and  pn.tip = 7 ) "
                  ") as z order by z.dat, z.num"));
    query.bindValue(":id_part",id_part);
    query.bindValue(":id_new_part",id_part);
    if (execQuery(query)){
        setHeaderData(0,Qt::Horizontal,tr("Дата"));
        setHeaderData(1,Qt::Horizontal,tr("№ нак."));
        setHeaderData(2,Qt::Horizontal,tr("Операция"));
        setHeaderData(3,Qt::Horizontal,tr("К-во, кг"));
        setHeaderData(4,Qt::Horizontal,tr("Брак, кг"));
        for (int i=0; i<rowCount(); i++){
            sum+=data(index(i,3),Qt::EditRole).toDouble();
            sumBreak+=data(index(i,4),Qt::EditRole).toDouble();
        }
    }

    QString s;
    s = (sum!=0)? (title + tr(" итого: ")+QLocale().toString(sum,'f',1)+tr(" кг")) : title;
    if (sumBreak!=0){
        s+=tr(" брак ")+QLocale().toString(sumBreak,'f',1)+tr(" кг");
    }
    setTitle(s);

}

ModelBreakEl::ModelBreakEl(QObject *parent) : ModelStat(parent)
{

}

void ModelBreakEl::refresh(int id_part)
{
    double sum=0;
    QString title=tr("Брак");
    QSqlQuery query;
    query.prepare("select pn.dat, pn.num, pb.kvo "
                  "from parti_break pb "
                  "inner join parti_nakl pn on pn.id = pb.id_nakl "
                  "where pn.tip=2 and pb.id_part = :id_part "
                  "order by pn.dat, pn.num");
    query.bindValue(":id_part",id_part);
    if (execQuery(query)){
        setHeaderData(0,Qt::Horizontal,tr("Дата"));
        setHeaderData(1,Qt::Horizontal,tr("№ нак."));
        setHeaderData(2,Qt::Horizontal,tr("К-во, кг"));
        for (int i=0; i<rowCount(); i++){
            sum+=data(index(i,2),Qt::EditRole).toDouble();
        }
    }
    QString s;
    s = (sum!=0)? (title + tr(" итого: ")+QLocale().toString(sum,'f',1)+tr(" кг")) : title;
    setTitle(s);
}

ModelSelfEl::ModelSelfEl(QObject *parent) : ModelStat(parent)
{

}

void ModelSelfEl::refresh(int id_part)
{
    double sum=0;
    QString title=tr("Собств. потреб.");
    QSqlQuery query;
    query.prepare("select ps.dat, ps.num, ps.kto, psi.kvo*sc.koef "
                  "from prod_self_items psi "
                  "inner join prod_self ps on ps.id = psi.id_self "
                  "inner join self_cons sc on sc.id = psi.id_cons "
                  "where psi.id_part = :id_part order by ps.dat, ps.num");
    query.bindValue(":id_part",id_part);
    if (execQuery(query)){
        setHeaderData(0,Qt::Horizontal,tr("Дата"));
        setHeaderData(1,Qt::Horizontal,tr("№ нак."));
        setHeaderData(2,Qt::Horizontal,tr("Куда"));
        setHeaderData(3,Qt::Horizontal,tr("К-во, кг"));
        for (int i=0; i<rowCount(); i++){
            sum+=data(index(i,3),Qt::EditRole).toDouble();
        }
    }
    QString s;
    s = (sum!=0)? (title + tr(" итого: ")+QLocale().toString(sum,'f',1)+tr(" кг")) : title;
    setTitle(s);
}

ModelStockEl::ModelStockEl(QObject *parent) : ModelStat(parent)
{

}

void ModelStockEl::refresh(int id_part)
{
    QString title=tr("Склад");
    QMap <QString,double> map;
    QSqlQuery query;
    query.prepare("select pn.dat, pn.num, i.nam, p.kvo*i.koef "
                  "from prod p "
                  "inner join prod_nakl pn on pn.id=p.id_nakl "
                  "inner join istoch i on i.id=pn.id_ist "
                  "where p.id_part = :id_part "
                  "order by pn.dat, pn.num");
    query.bindValue(":id_part",id_part);
    if (execQuery(query)){
        setHeaderData(0,Qt::Horizontal,tr("Дата"));
        setHeaderData(1,Qt::Horizontal,tr("№ нак."));
        setHeaderData(2,Qt::Horizontal,tr("Источник"));
        setHeaderData(3,Qt::Horizontal,tr("К-во, кг"));
        for (int i=0; i<rowCount(); i++){
            QString key=data(index(i,2),Qt::EditRole).toString();
            double val=data(index(i,3),Qt::EditRole).toDouble();
            if (map.contains(key)){
                double oldval=map.value(key);
                map[key]=oldval+val;
            } else {
                map[key]=val;
            }
        }
    }
    QString s;
    QStringList list = map.keys();
    for (QString l : list) {
        if (!s.isEmpty()){
            s+=tr("; ");
        }
        s+=l+tr(": ")+QLocale().toString(map.value(l),'f',1)+tr(" кг");
    }
    setTitle(title+tr(" ")+s);
}

ModelShipEl::ModelShipEl(QObject *parent) : ModelStat(parent)
{

}

void ModelShipEl::refresh(int id_part)
{
    double sum=0;
    QString title=tr("Отгрузки");
    QSqlQuery query;
    query.prepare("select s.dat_vid, s.nom_s, p.short, o.massa "
                  "from otpusk o "
                  "inner join sertifikat s on o.id_sert=s.id "
                  "inner join poluch p on s.id_pol=p.id "
                  "where o.id_part = :id_part and s.id_type = 1 "
                  "order by s.dat_vid, s.nom_s");
    query.bindValue(":id_part",id_part);
    if (execQuery(query)){
        setHeaderData(0,Qt::Horizontal,tr("Дата"));
        setHeaderData(1,Qt::Horizontal,tr("№ нак."));
        setHeaderData(2,Qt::Horizontal,tr("Получатель"));
        setHeaderData(3,Qt::Horizontal,tr("К-во, кг"));
        for (int i=0; i<rowCount(); i++){
            sum+=data(index(i,3),Qt::EditRole).toDouble();
        }
    }
    QString s;
    s = (sum!=0)? (title + tr(" итого: ")+QLocale().toString(sum,'f',1)+tr(" кг")) : title;
    setTitle(s);
}

ModelStat::ModelStat(QObject *parent) : ModelRo(parent)
{
    setDecimal(1);
}

void ModelStat::refresh(int /*id_part*/)
{

}

QString ModelStat::getTitle()
{
    return tit;
}

void ModelStat::setTitle(QString t)
{
    tit=t;
    emit sigSum(tit);
}
