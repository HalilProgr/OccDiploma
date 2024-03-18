#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStackedWidget>
#include <QGroupBox>
#include <QTextEdit>

#include "common/DocumentCommon.hpp"
#include "view/OcctQtViewer.h"

#include "data/CollectionShapes.h"

#include "algorithms/kinematic.h"
#include <QMainWindow>


class View;
class GeomWidget;
class DocumentCommon;

//! Qt main window which include OpenCASCADE for its central widget.
class ApplicationCommonWindow: public QMainWindow
{
    Q_OBJECT
public:
    ApplicationCommonWindow();
    ~ApplicationCommonWindow();

private:

    void Init();

    std::shared_ptr<App::Data::CollectionShapes> shapes;

    App::Common::DocumentCommon* document;
    OcctQtViewer* viewer;
};

#endif // MAINWINDOW_H
