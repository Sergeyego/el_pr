#ifndef DIALOGPACKLOAD_H
#define DIALOGPACKLOAD_H

#include <QDialog>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QTextFrame>
#include <QTextFrameFormat>
#include <QDate>
#include "httpsyncmanager.h"
#include "modelro.h"

namespace Ui {
class DialogPackLoad;
}

class DialogPackLoad : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPackLoad(QWidget *parent = nullptr);
    ~DialogPackLoad();

private:
    Ui::DialogPackLoad *ui;
    void setHtml(const QString &html);
    void updMaster(QDate date);
    ModelRo *modelMaster;
    QString currentIdMaster();
    int currentIdNakl;

public slots:
    void setCurrentDate(QDate date);
    void setIdNakl(int id);

private slots:
    void loadDoc();
    void print();
    void loadData();
};

#endif // DIALOGPACKLOAD_H
