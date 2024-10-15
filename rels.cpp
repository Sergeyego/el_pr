#include "rels.h"

Rels* Rels::rels_instance=0;

Rels *Rels::instance()
{
    if (rels_instance==0)
        rels_instance = new Rels();
    return rels_instance;
}


Rels::Rels(QObject *parent) : QObject(parent)
{  
    invoiceManager = new InvoiceManager(this);

    relMech = new DbSqlRelation("mech_tbl","id","nam",this);
    relMech->setSort("nam");

    relChem = new DbSqlRelation("chem_tbl","id","sig",this);
    relChem->setSort("sig");

    relChemDev = new DbSqlRelation("chem_dev","id","short",this);
    relChemDev->setSort("short");

    relRab = new DbSqlRelation("rab_rab","id","snam",this);
    relRab->setSort("snam");
    relRab->setFilter("rab_rab.id in (select distinct rr.id from rab_rab rr "
                      "inner join kamin_inf ki on ki.id_empl = rr.id_kamin "
                      "where ki.id_job = '91a0304c-a6e4-11e4-aa08-0018e7159e2e')");

    relPress = new DbSqlRelation("pres","id","nam",this);
    relPress->setSort("nam");

    relDos = new DbSqlRelation("dosage","id","str",this);
    relDos->setSort("dosage.dat desc, dosage.parti");
    relDos->model()->setLimit(4000);

    relGlass = new DbSqlRelation("matr","id","nam",this);
    relGlass->setFilter("matr.id_type=3 or matr.id_type=4");
    relGlass->setSort("nam");

    relCons = new DbSqlRelation("glass_cons","id","num",this);
    relCons->setSort("num");

    relSrc = new DbSqlRelation("istoch","id","nam",this);
    relSrc->setSort("nam");

    relRcp = new DbSqlRelation("rcp_nam","id","nam",this);
    relRcp->setSort("nam");
    relRcp->setFilter("rcp_nam.lev=1");

    relMark = new DbSqlRelation("elrtr","id","marka",this);
    relMark->setSort("marka");
    relMark->setFilter("elrtr.id<>0");

    relProv = new DbSqlRelation("provol","id","nam",this);
    relProv->setSort("nam");

    relPack = new DbSqlRelation("el_pack_view","id","nam",this);
    relPack->setSort("nam");

    relLong = new DbSqlRelation("el_long","id","nam",this);
    relLong->setSort("nam");

    relVar = new DbSqlRelation("elrtr_vars","id","nam",this);
    relVar->setSort("id");

    relPackType = new DbSqlRelation("parti_nakl_tip","id","nam",this);
    relPackType->setSort("id");
    relPackType->setFilter("en=true");

    relElPart = new DbSqlRelation("parti","id","str",this);
    relElPart->setSort("parti.dat_part desc, parti.n_s desc");
    relElPart->setFilter("parti.id<>0");
    relElPart->model()->setLimit(4000);

    relElPartNew = new DbSqlRelation("parti","id","str",this);
    relElPartNew->setAlias("parti_new");
    relElPartNew->setSort("parti_new.dat_part desc, parti_new.n_s desc");
    relElPartNew->setFilter("parti_new.id<>0");
    relElPartNew->model()->setLimit(4000);

    relSelfType = new DbSqlRelation("self_cons","id","nam",this);
    relSelfType->setSort("nam");
    relSelfType->setFilter("en=true");
}

