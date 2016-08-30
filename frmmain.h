#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QMainWindow>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QSqlDatabase>

namespace Ui {
class frmMain;
}

class frmMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();
    Ui::frmMain *ui;
    QSqlDatabase db;
    QGeoPositionInfoSource *locInf;


public slots:
    void addCase();
    void addPOI();
    void editPOI();
    QString hrider();
    void getGPSloc(QGeoPositionInfo gp);
    void date1();
    void date2();
    void date3();
    void tabupdate(QString s);
    void delrow();
    QString dateCheck(QDate qd);
    void addAffected();
    void delAffected();
};

#endif // FRMMAIN_H
