#include <QtSql>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QSignalMapper>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QDate>
#include <QDesktopWidget>
#include <QCryptographicHash>
#include <QDebug>
#include <QNetworkInterface>
#include <QNetworkConfigurationManager>
#include <QGeoCoordinate>
#include <QGeoPositionInfo>
#include <QGeoPositionInfoSource>
#include <QGeoSatelliteInfoSource>

#include "frmmain.h"
#include "ui_frmmain.h"
#include "xaffected.h"
#include "ui_xaffected.h"

frmMain::frmMain(QWidget *parent): QMainWindow(parent), ui(new Ui::frmMain){
    ui->setupUi(this);

    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
//    connect(ui->actionNew_case, SIGNAL(triggered()), this, SLOT(addCase()));
//    connect(ui->actionNew_POI, SIGNAL(triggered()), this, SLOT(addPOI()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(addPOI()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(addCase()));
    connect(ui->checkBox, SIGNAL(clicked()), this, SLOT(date1()));
    connect(ui->checkBox_2, SIGNAL(clicked()), this, SLOT(date2()));
    connect(ui->checkBox_3, SIGNAL(clicked()), this, SLOT(date3()));

    locInf = QGeoPositionInfoSource::createDefaultSource(this);
    locInf->setPreferredPositioningMethods(QGeoPositionInfoSource::AllPositioningMethods);
    connect(locInf, SIGNAL(positionUpdated(QGeoPositionInfo)), this, SLOT(getGPSloc(QGeoPositionInfo)));
    locInf->startUpdates();

    QString targ = "./db.sqlite";
    QString arch = "assets:/db.sqlite";
    QFile dfile(arch);
    QFile df(targ);

    df.remove();

    if (dfile.exists()){
        if(!df.exists()){
            dfile.copy(targ);
        }
    }

    QFile::setPermissions(targ, QFile::WriteOwner | QFile::ReadOwner);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(targ);
    db.open();

    QSqlQuery query;
    QString sql;

    ui->comboBox_2->addItem("");
    sql = "select disease from xdiseases where lang='en' order by disease";
    query.exec(sql);
    while (query.next()) {
        ui->comboBox_2->addItem(query.value(0).toString());
    }

    ui->comboBox_3->addItem("");
    ui->comboBox_3->addItem("Suspected");
    ui->comboBox_3->addItem("Confirmed");
    ui->comboBox_3->addItem("Not confirmed");
    ui->comboBox_3->addItem("Expired");

    ui->comboBox_4->addItem("");
    ui->comboBox_4->addItem("No");
    ui->comboBox_4->addItem("Yes");

    ui->comboBox_5->addItem("");
    sql = "select poitype from xpoitypes where lang='en' order by poitype";
    query.exec(sql);
    while (query.next()) {
        ui->comboBox_5->addItem(query.value(0).toString());
    }

    db.close();

    QString st = "QCalendarWidget { font-size:40px; width: 300px; left: 20px; }";
    ui->dateEdit->setStyleSheet(st);
    ui->dateEdit_2->setStyleSheet(st);
    ui->dateEdit_3->setStyleSheet(st);

    ui->comboBox->addItem("");
    ui->comboBox->addItem("Outbreaks");
    ui->comboBox->addItem("POIs");

    connect(ui->comboBox, SIGNAL(currentIndexChanged(QString)) , this, SLOT(tabupdate(QString)));
    connect(ui->toolButton_4, SIGNAL(clicked()), this, SLOT(delrow()));
    connect(ui->toolButton, SIGNAL(clicked()), this, SLOT(addAffected()));
    connect(ui->toolButton_2, SIGNAL(clicked()), this, SLOT(delAffected()));

}

frmMain::~frmMain(){
    delete ui;
}

void frmMain::delAffected(){

}

void frmMain::addAffected(){
    xaffected *dlg = new xaffected(this);

    dlg->setWindowTitle("Affected animals");
    dlg->ui->comboBox->addItem("");

    db.open();
    QSqlQuery query;
    QString sql = "select species from xspecies where lang='en' order by species";
    query.exec(sql);
    while (query.next()) {
        dlg->ui->comboBox->addItem(query.value(0).toString());
    }
    db.close();

    if (dlg->exec() == QDialog::Accepted){
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        int nr = ui->tableWidget->rowCount()-1;
        QTableWidgetItem *it = new QTableWidgetItem(dlg->ui->comboBox->currentText());
        ui->tableWidget->setItem(nr, 0, it);
        QString s = "-";
        if (dlg->ui->lineEdit->text()!="")
            s = dlg->ui->lineEdit->text();
        QTableWidgetItem *itb = new QTableWidgetItem(QString("%1").arg(s));
        ui->tableWidget->setItem(nr, 1, itb);
    }
}


void frmMain::date1(){
    if (ui->checkBox->isChecked()){
        ui->dateEdit->setEnabled(true);
        ui->dateEdit->setDate(QDate::currentDate());
    } else {
        ui->dateEdit->setEnabled(false);
        ui->dateEdit->setDate(QDate(2000,1,1));
    }
}

void frmMain::date2(){
    if (ui->checkBox_2->isChecked()){
        ui->dateEdit_2->setEnabled(true);
        ui->dateEdit_2->setDate(QDate::currentDate());
    } else {
        ui->dateEdit_2->setEnabled(false);
        ui->dateEdit_2->setDate(QDate(2000,1,1));
    }
}

void frmMain::date3(){
    if (ui->checkBox_3->isChecked()){
        ui->dateEdit_3->setEnabled(true);
        ui->dateEdit_3->setDate(QDate::currentDate());
    } else {
        ui->dateEdit_3->setEnabled(false);
        ui->dateEdit_3->setDate(QDate(2000,1,1));
    }
}


void frmMain::getGPSloc(QGeoPositionInfo gp){
    if (gp.isValid()){
        QGeoCoordinate gc = gp.coordinate();
        ui->lineEdit->setText(QString("%1").arg(gc.longitude(), 0, 'g', 8));
        ui->lineEdit_2->setText(QString("%1").arg(gc.latitude(), 0, 'g', 8));
    }
}


QString frmMain::hrider(){
    QString mac = "";
    foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces()){
        if (!(netInterface.flags() & QNetworkInterface::IsLoopBack) && !netInterface.humanReadableName().toLower().contains("vmware") && !netInterface.humanReadableName().toLower().contains("virtual")){
            mac = QString(netInterface.hardwareAddress()).replace(":","_");
        }
    }
    QString most = QDateTime::currentDateTimeUtc().toString("dd/MM/yyyy hh:mm:ss.z");

    QByteArray uid = QString("%1 %2").arg(mac).arg(most).toUtf8();
    QString hrid = QString(QCryptographicHash::hash(uid, QCryptographicHash::Sha256).toHex());
    return hrid;
}

//def dateCheck(self, qd):
//    dt = qd.toString('dd/MM/yyyy')
//    if dt=='01/01/2000':
//        dt = ''
//    return  dt

QString frmMain::dateCheck(QDate qd){
    QString dt = qd.toString("dd/MM/yyyy");
    if (dt=="01/01/2000")
        dt = "";
    return dt;
}


void frmMain::addCase(){
    QString species = "";
    QString production = "";
    QString most = QDateTime::currentDateTimeUtc().toString("dd/MM/yyyy hh:mm:ss");

    int rows = ui->tableWidget->rowCount();
    for(int i=0; i<rows; i++){
        if (i==0){
            species = ui->tableWidget->item(i, 0)->text();
            production = ui->tableWidget->item(i, 1)->text();
        } else {
            species = species + " | " + ui->tableWidget->item(i, 0)->text();
            production = production + " | " + ui->tableWidget->item(i, 1)->text();
        }
    }



    db.open();
    QSqlQuery query;
    QString sql = "insert into outbreaks (localid, code, largescale, disease, animalno, species, production, year, status, suspect, confirmation, expiration, notes, hrid, timestamp, lon, lat) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    query.prepare(sql);
    query.addBindValue(ui->lineEdit_3->text());
    query.addBindValue(ui->lineEdit_5->text());
    query.addBindValue(ui->comboBox_4->currentText());
    query.addBindValue(ui->comboBox_2->currentText());
    query.addBindValue(ui->lineEdit_6->text());
    query.addBindValue(species);
    query.addBindValue(production);
    query.addBindValue(ui->lineEdit_4->text());
    query.addBindValue(ui->comboBox_3->currentText());
    query.addBindValue(dateCheck(ui->dateEdit->date()));
    query.addBindValue(dateCheck(ui->dateEdit_2->date()));
    query.addBindValue(dateCheck(ui->dateEdit_3->date()));
    query.addBindValue(ui->textEdit->toPlainText());
    query.addBindValue(hrider());
    query.addBindValue(most);
    query.addBindValue(ui->lineEdit->text());
    query.addBindValue(ui->lineEdit_2->text());
    query.exec();
    db.close();

    ui->lineEdit_3->setText("");
    ui->lineEdit_5->setText("");
    ui->comboBox_4->setCurrentIndex(0);
    ui->comboBox_2->setCurrentIndex(0);
    ui->lineEdit_6->setText("");
    ui->tableWidget->clear();
    ui->lineEdit_4->setText("");
    ui->comboBox_3->setCurrentIndex(0);
    ui->dateEdit->setDate(QDate(2000,1,1));
    ui->dateEdit_2->setDate(QDate(2000,1,1));
    ui->dateEdit_3->setDate(QDate(2000,1,1));
    ui->textEdit->clear();

//    feat.setAttribute(feat.fieldNameIndex('localid'), dlg.lineEdit_3.text())
//    feat.setAttribute(feat.fieldNameIndex('code'), dlg.lineEdit_5.text())
//    feat.setAttribute(feat.fieldNameIndex('largescale'), dlg.comboBox_4.currentText())
//    feat.setAttribute(feat.fieldNameIndex('disease'), dlg.comboBox_2.currentText())
//    feat.setAttribute(feat.fieldNameIndex('animalno'), dlg.lineEdit_6.text())
//    feat.setAttribute(feat.fieldNameIndex('species'), species)
//    feat.setAttribute(feat.fieldNameIndex('production'), production)
//    feat.setAttribute(feat.fieldNameIndex('year'), dlg.lineEdit_4.text())
//    feat.setAttribute(feat.fieldNameIndex('status'), dlg.comboBox_3.currentText())
//    feat.setAttribute(feat.fieldNameIndex('suspect'), self.dateCheck(dlg.dateEdit.date()))
//    feat.setAttribute(feat.fieldNameIndex('confirmation'), self.dateCheck(dlg.dateEdit_2.date()))
//    feat.setAttribute(feat.fieldNameIndex('expiration'), self.dateCheck(dlg.dateEdit_3.date()))
//    feat.setAttribute(feat.fieldNameIndex('notes'), dlg.textEdit.toPlainText())
//    feat.setAttribute(feat.fieldNameIndex('hrid'), self.hashIDer(most))
//    feat.setAttribute(feat.fieldNameIndex('timestamp'), most.toString('dd/MM/yyyy hh:mm:ss'))
//    return feat
}

void frmMain::addPOI(){
    QApplication::setOverrideCursor(Qt::WaitCursor);
//    frmPOI *dlg = new frmPOI(this);

//    if (dlg->exec() == QDialog::Rejected ){
    db.open();
    QSqlQuery query;
    QString sql = "insert into pois (localid, code, activity, hrid, lon, lat) VALUES (?, ?, ?, ?, ?, ?)";
    query.prepare(sql);
    query.addBindValue(ui->lineEdit_9->text());
    query.addBindValue(ui->lineEdit_10->text());
    query.addBindValue(ui->comboBox_5->currentText());
    query.addBindValue(hrider());
    query.addBindValue(ui->lineEdit->text());
    query.addBindValue(ui->lineEdit_2->text());
    query.exec();
    db.close();

    ui->lineEdit_9->setText("");
    ui->lineEdit_10->setText("");
    ui->comboBox_5->setCurrentIndex(0);

//    }
    QApplication::restoreOverrideCursor();
}


void frmMain::editPOI(){
    QApplication::setOverrideCursor(Qt::WaitCursor);

    QApplication::restoreOverrideCursor();
}


void frmMain::tabupdate(QString s){
    QApplication::setOverrideCursor(Qt::WaitCursor);
    db.open();

    QString sql="";

    if(s=="Outbreaks")
        sql = "select gid, localid, code, largescale, disease, animalno, species, production, year, status, suspect, confirmation, expiration, notes from outbreaks order by gid;";
    if(s=="POIs")
        sql = "select gid, localid, code, activity from pois order by gid;";

    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(sql);
//    model1->setHeaderData(0, Qt::Horizontal, "gid");
    QSortFilterProxyModel *sort_filter = new QSortFilterProxyModel(this);
    sort_filter->setSortLocaleAware(true);
    sort_filter->setSourceModel(model);
    sort_filter->sort (0);
    ui->tableView->setModel(sort_filter);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setSortingEnabled(true);
    sort_filter->sort(0, Qt::AscendingOrder);
    db.close();
    QApplication::restoreOverrideCursor();
}


void frmMain::delrow(){
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QString sql="";

    QModelIndex idx;
    idx = ui->tableView->selectionModel()->currentIndex();
    QString gid = ui->tableView->model()->data(idx.sibling(idx.row(),0)).toString();

    if (ui->comboBox->currentText()=="Outbreaks")
        sql = QString("delete from outbreaks where gid=%1").arg(gid);
    if (ui->comboBox->currentText()=="POIs")
        sql = QString("delete from pois where gid=%1").arg(gid);

//    ui->lineEdit_9->setText(sql);

    db.open();
    QSqlQuery query(sql);
    query.exec();
    db.close();

    tabupdate(ui->comboBox->currentText());

    QApplication::restoreOverrideCursor();
}



