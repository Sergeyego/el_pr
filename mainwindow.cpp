#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(bool part, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if (part){
        ui->menuOp->setEnabled(false);
        ui->menuRep->setEnabled(false);
        ui->menuAn->setEnabled(false);
    }

    actAction(ui->actionPart,&MainWindow::newFormPart);

    actAction(ui->actionPack,&MainWindow::newFormPack);
    actAction(ui->actionPerePack,&MainWindow::newFormPerePack);
    actAction(ui->actionSelf,&MainWindow::newFormSelf);
    actAction(ui->actionFix,&MainWindow::newFormFix);

    actAction(ui->actionRep,&MainWindow::newFormReport);

    actAction(ui->actionAnPack,&MainWindow::newAnPack);
    actAction(ui->actionAnPackZp,&MainWindow::newAnPackZp);
    actAction(ui->actionAnPackDef,&MainWindow::newAnPackDef);
    actAction(ui->actionAnTrans,&MainWindow::newAnTrans);
    actAction(ui->actionAnPress,&MainWindow::newAnPress);
    actAction(ui->actionAnPressZp,&MainWindow::newAnPressZp);

    loadSettings();
    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
}

MainWindow::~MainWindow()
{
    saveSettings();
    delete Rels::instance();
    delete ui;
}

bool MainWindow::exist(QObject *a)
{
    bool b=false;
    QAction *action = qobject_cast<QAction *>(a);
    if (action){
        b=setActiveSubWindow(action->text());
    }
    return b;
}

void MainWindow::actAction(QAction *a, void (MainWindow::*sl)())
{
    connect(a, &QAction::triggered, this, sl);
    actions.insert(a->text(),a);
}

void MainWindow::addSubWindow(QWidget *w, QObject *a)
{
    w->setAttribute(Qt::WA_DeleteOnClose);
    QAction *action = qobject_cast<QAction *>(a);
    if (action){
        w->setWindowTitle(action->text());
    }
    ui->tabWidget->addTab(w,w->windowTitle());
    ui->tabWidget->setCurrentWidget(w);
}

bool MainWindow::setActiveSubWindow(QString t)
{
    bool b=false;
    for (int i=0; i<ui->tabWidget->count(); i++){
        if (ui->tabWidget->tabText(i)==t){
            ui->tabWidget->setCurrentIndex(i);
            b=true;
            break;
        }
    }
    return b;
}

void MainWindow::closeTab(int index)
{
    ui->tabWidget->widget(index)->close();
}

void MainWindow::newFormPart()
{
    if (!exist(sender())){
        addSubWindow(new FormPart(),sender());
    }
}

void MainWindow::newFormPack()
{
    if (!exist(sender())){
        addSubWindow(new FormPack(),sender());
    }
}

void MainWindow::newFormPerePack()
{
    if (!exist(sender())){
        addSubWindow(new FormPerePack(),sender());
    }
}

void MainWindow::newFormSelf()
{
    if (!exist(sender())){
        addSubWindow(new FormSelf(),sender());
    }
}

void MainWindow::newFormFix()
{
    if (!exist(sender())){
        addSubWindow(new FormFix(),sender());
    }
}

void MainWindow::newFormReport()
{
    if (!exist(sender())){
        addSubWindow(new FormReport(),sender());
    }
}

void MainWindow::newAnPack()
{
    if (!exist(sender())){
        addSubWindow(new CubeWidget(14),sender());
    }
}

void MainWindow::newAnPackZp()
{
    if (!exist(sender())){
        addSubWindow(new CubeWidget(15),sender());
    }
}

void MainWindow::newAnPackDef()
{
    if (!exist(sender())){
        addSubWindow(new CubeWidget(18),sender());
    }
}

void MainWindow::newAnTrans()
{
    if (!exist(sender())){
        addSubWindow(new CubeWidget(19),sender());
    }
}

void MainWindow::newAnPress()
{
    if (!exist(sender())){
        addSubWindow(new CubeWidget(16),sender());
    }
}

void MainWindow::newAnPressZp()
{
    if (!exist(sender())){
        addSubWindow(new CubeWidget(3),sender());
    }
}

void MainWindow::loadSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    this->restoreGeometry(settings.value("main_geometry").toByteArray());
    this->restoreState(settings.value("main_state").toByteArray());
    QString opentab=settings.value("main_opentab").toString();
    QString current=settings.value("main_currenttab").toString();

    if (!opentab.isEmpty()){
        QStringList l=opentab.split("|");
        foreach (QString a, l) {
            if (actions.contains(a)){
                actions.value(a)->trigger();
            }
        }
    } else {
        ui->actionPart->trigger();
    }
    setActiveSubWindow(current);
}

void MainWindow::saveSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    settings.setValue("main_state", this->saveState());
    settings.setValue("main_geometry", this->saveGeometry());
    QString opentab, currenttab;
    for (int i=0; i<ui->tabWidget->count(); i++){
        if (!opentab.isEmpty()){
            opentab+="|";
        }
        opentab+=ui->tabWidget->tabText(i);
    }
    currenttab=ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    settings.setValue("main_opentab", opentab);
    settings.setValue("main_currenttab",currenttab);
}

