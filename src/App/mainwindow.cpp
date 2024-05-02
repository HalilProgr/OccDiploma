#include "mainwindow.h"

#include <QVBoxLayout>
#include <QFrame>
#include <QSplitter>
#include <AIS_ViewCube.hxx>
#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>

#include <QHBoxLayout>

#include "src/data/Segment.h"
#include "src/algorithms/kinematic.h"
#include "src/common/DocumentCommon.h"

ApplicationCommonWindow::ApplicationCommonWindow()
    : QMainWindow (nullptr)
{
    document = new Lib::Common::DocumentCommon(this);
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
    _reader.SetPath("C:/Qtprogect/OccDiploma/src/App/robot/init.json");
    _reader.ParseData();
    auto ptr = _reader.GetSegments();
    auto ptrTool = _reader.GetTool();
    shapes = std::make_shared<Lib::Data::Manipulator>();
    shapes->SetSegments(ptr);
    shapes->SetTool(ptrTool);
    document->AddDynamicObjects(shapes);
    document->DislayAll();
}
