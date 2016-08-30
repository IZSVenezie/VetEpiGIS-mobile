#include "xaffected.h"
#include "ui_xaffected.h"

xaffected::xaffected(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::xaffected)
{
    ui->setupUi(this);
}

xaffected::~xaffected()
{
    delete ui;
}
