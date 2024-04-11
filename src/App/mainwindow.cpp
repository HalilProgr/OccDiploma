#include "mainwindow.h"

#include <QVBoxLayout>
#include <QFrame>
#include <QSplitter>
#include <AIS_ViewCube.hxx>
#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>

#include "src/data/Segment.h"

#include "src/algorithms/kinematic.h"

#include "src/common/DocumentCommon.h"

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
    document->AddDynamicObjects(shapes);
    document->DislayAll();
}
