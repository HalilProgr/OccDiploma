#include "mainwindow.hpp"

#include <QVBoxLayout>
#include <QFrame>
#include <QSplitter>
#include <AIS_ViewCube.hxx>
#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>

#include "data/Segment.h"

#include "algorithms/kinematic.h"

#include "view/DocumentCommon.hpp"

ApplicationCommonWindow::ApplicationCommonWindow()
    : QMainWindow (nullptr)
{
    shapes.Init();

    document = new DocumentCommon(this);

    document->SetObjects(shapes, true);

    QWidget* temp = new QWidget(this);
    viewer = new OcctQtViewer(document, temp);

    setCentralWidget(viewer);
}

ApplicationCommonWindow::~ApplicationCommonWindow()
{
    delete viewer;
    delete document;
}
