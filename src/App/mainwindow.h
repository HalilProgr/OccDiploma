#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStackedWidget>
#include <QGroupBox>
#include <QTextEdit>

#include "src/common/DocumentCommon.h"
#include "src/view/OcctQtViewer.h"
#include "src/data/Manipulator.h"
#include "src/common/Reader.h"

#include <QMainWindow>


//
class ApplicationCommonWindow: public QMainWindow
{
    Q_OBJECT
public:
    ApplicationCommonWindow();
    ~ApplicationCommonWindow();

private:

    void Init();


    Lib::Common::Reader _reader;
    std::shared_ptr<Lib::Data::Manipulator> shapes;

    Lib::Common::DocumentCommon* document;
    OcctQtViewer* viewer;
    OcctQtViewer* viewer1;
};

#endif // MAINWINDOW_H
