#include "mainwindow.hpp"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ApplicationCommonWindow w;
    w.show();
    return a.exec();
}
