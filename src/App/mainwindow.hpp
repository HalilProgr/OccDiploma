#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStackedWidget>
#include <QGroupBox>
#include <QTextEdit>

#include "ui_mainwindow.h"
#include "view/DocumentCommon.hpp"
#include "view/OcctQtViewer.h"

#include "data/CollectionShapes.h"

#include "algorithms/kinematic.h"

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
    App::Data::CollectionShapes shapes;

    DocumentCommon* document;
    OcctQtViewer* viewer;
};

#endif // MAINWINDOW_H
