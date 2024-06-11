QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql network printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(xlsx/qtxlsx.pri)

SOURCES += \
    db/dbcombobox.cpp \
    db/dbdateedit.cpp \
    db/dbdelegate.cpp \
    db/dblogin.cpp \
    db/dbmapper.cpp \
    db/dbrelationeditdialog.cpp \
    db/dbtablemodel.cpp \
    db/dbviewer.cpp \
    db/executor.cpp \
    db/tablemodel.cpp \
    dialogloadchem.cpp \
    dialogpackload.cpp \
    dialogtmp.cpp \
    formfix.cpp \
    formpack.cpp \
    formpart.cpp \
    formperepack.cpp \
    formreport.cpp \
    formself.cpp \
    httpsyncmanager.cpp \
    invoicemanager.cpp \
    main.cpp \
    mainwindow.cpp \
    modelnakl.cpp \
    modelro.cpp \
    models.cpp \
    olap/axiswidget.cpp \
    olap/cubewidget.cpp \
    olap/cubic.cpp \
    olap/dialogolapflt.cpp \
    olap/formfilter.cpp \
    olap/olapmodel.cpp \
    olap/tableview.cpp \
    progressexecutor.cpp \
    progressreportdialog.cpp \
    rels.cpp

HEADERS += \
    db/dbcombobox.h \
    db/dbdateedit.h \
    db/dbdelegate.h \
    db/dblogin.h \
    db/dbmapper.h \
    db/dbrelationeditdialog.h \
    db/dbtablemodel.h \
    db/dbviewer.h \
    db/executor.h \
    db/tablemodel.h \
    dialogloadchem.h \
    dialogpackload.h \
    dialogtmp.h \
    formfix.h \
    formpack.h \
    formpart.h \
    formperepack.h \
    formreport.h \
    formself.h \
    httpsyncmanager.h \
    invoicemanager.h \
    mainwindow.h \
    modelnakl.h \
    modelro.h \
    models.h \
    olap/axiswidget.h \
    olap/cubewidget.h \
    olap/cubic.h \
    olap/dialogolapflt.h \
    olap/formfilter.h \
    olap/olapmodel.h \
    olap/tableview.h \
    progressexecutor.h \
    progressreportdialog.h \
    rels.h

FORMS += \
    db/dblogin.ui \
    db/dbrelationeditdialog.ui \
    dialogloadchem.ui \
    dialogpackload.ui \
    dialogtmp.ui \
    formfix.ui \
    formpack.ui \
    formpart.ui \
    formperepack.ui \
    formreport.ui \
    formself.ui \
    mainwindow.ui \
    olap/axiswidget.ui \
    olap/cubewidget.ui \
    olap/dialogolapflt.ui \
    olap/formfilter.ui \
    progressreportdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_FILE = ico.rc

RESOURCES += \
    res.qrc

DISTFILES += \
    ico.rc
