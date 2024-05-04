#include "mainwindow.h"
#include <QApplication>

#include "..\Lib\src\common\Tools.h"
#include "..\Lib\src\algorithms\CubicSpline.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ApplicationCommonWindow w;
    w.show();
    return a.exec();
}
