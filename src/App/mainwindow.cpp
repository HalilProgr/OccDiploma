#include "mainwindow.hpp"

#include <QVBoxLayout>
#include <QFrame>
#include <QSplitter>
#include <AIS_ViewCube.hxx>
#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>

#include "data/Segment.h"

#include "algorithms/kinematic.h"

#include "common/DocumentCommon.hpp"

ApplicationCommonWindow::ApplicationCommonWindow()
    : QMainWindow (nullptr)
{
    document = new App::Common::DocumentCommon(this);
    QWidget* temp = new QWidget(this);
    viewer = new OcctQtViewer(document, temp);
    setCentralWidget(viewer);

    Init();
}

ApplicationCommonWindow::~ApplicationCommonWindow()
{
    delete viewer;
    delete document;
}

void ApplicationCommonWindow::Init()
{
    shapes = std::make_shared<App::Data::CollectionShapes>();
    shapes->Init();
    document->AddDynamicObjects(shapes);
    document->DislayAll();
}
