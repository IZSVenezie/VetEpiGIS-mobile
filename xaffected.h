#ifndef XAFFECTED_H
#define XAFFECTED_H

#include <QDialog>

namespace Ui {
class xaffected;
}

class xaffected : public QDialog
{
    Q_OBJECT

public:
    explicit xaffected(QWidget *parent = 0);
    ~xaffected();
    Ui::xaffected *ui;

};

#endif // XAFFECTED_H
