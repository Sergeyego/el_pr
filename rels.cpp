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
    /*relGlass = new DbRelation(QString("select id, nam from matr where id_type=3 or id_type=4 order by nam"),0,1,this);
    relPar = new DbRelation(QString("select id, nam from glass_par order by nam"),0,1,this);
    relChem = new DbRelation(QString("select id, sig from chem_tbl order by sig"),0,1,this);
    relMech = new DbRelation(QString("select id, nam from mech_tbl order by nam"),0,1,this);
    relPress = new DbRelation(QString("select id, nam from pres order by nam"),0,1,this);
    relChemDev = new DbRelation(QString("select id, short from chem_dev order by short"),0,1,this);
    */

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
}

