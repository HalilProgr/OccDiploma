#include "mainwindow.h"
#include <QApplication>

#include "..\Lib\src\common\Tools.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ApplicationCommonWindow w;
    w.show();
    return a.exec();
}
