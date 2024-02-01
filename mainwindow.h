#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QAction>
#include "olap/cubewidget.h"
#include "formpart.h"
#include "formpack.h"
#include "formperepack.h"
#include "formself.h"
#include "formfix.h"
#include "formreport.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool exist(QObject *a);
    void actAction(QAction *a, void (MainWindow::*sl)());
    void addSubWindow(QWidget *w, QObject *a);
    bool setActiveSubWindow(QString t);
    QMap <QString,QAction*> actions;

private slots:
    void closeTab(int index);
    void newFormPart();
    void newFormPack();
    void newFormPerePack();
    void newFormSelf();
    void newFormFix();
    void newFormReport();
    void newAnPack();
    void newAnPackZp();
    void newAnPackDef();
    void newAnTrans();

private:
    Ui::MainWindow *ui;
    void loadSettings();
    void saveSettings();
};
#endif // MAINWINDOW_H
