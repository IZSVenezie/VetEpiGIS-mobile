#include "frmmain.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QStyle * android = QStyleFactory::create("Android");
    a.setStyle(android);

    frmMain w;
    w.show();

    return a.exec();
}
