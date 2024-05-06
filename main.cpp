#include "mainwindow.h"

#include <QApplication>
#include "db/dblogin.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/ico.ico"));

    QString key;
    if (argc>1){
        key=QString(argv[1]);
    }

    QPixmap logo(":/images/simb_labl.png");
    DbLogin d(QObject::tr("Производство электродов"),logo);
    if (d.exec()!=QDialog::Accepted) exit(1);

    MainWindow w(key==QString("part"));
    w.show();
    return a.exec();
}
