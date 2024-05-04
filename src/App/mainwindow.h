#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStackedWidget>
#include <QGroupBox>
#include <QTextEdit>

#include "src/common/DocumentCommon.h"
#include "src/view/OcctQtViewer.h"
#include "src/data/Manipulator.h"
#include "src/common/Reader.h"
#include "src/common/Controller.h"

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
    Lib::Common::Controller* controller;
    OcctQtViewer* viewer;
};

#endif // MAINWINDOW_H
