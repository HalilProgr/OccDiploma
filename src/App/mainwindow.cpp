#include "mainwindow.h"
#include <QHBoxLayout>
#include "src/data/Segment.h"
#include "src/algorithms/kinematic.h"
#include "src/common/DocumentCommon.h"
#include <QHBoxLayout>


ApplicationCommonWindow::ApplicationCommonWindow()
    : QMainWindow (nullptr)
{
    document = new Lib::Common::DocumentCommon(this);
    viewer = new OcctQtViewer(document, this);
    controller = new Lib::Common::Controller(viewer, document, this);

    QWidget* mainWidget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(viewer);
    layout->addWidget(controller);
    mainWidget->setLayout(layout);

    setCentralWidget(mainWidget);
    Init();
}

ApplicationCommonWindow::~ApplicationCommonWindow()
{
    delete viewer;
    delete document;
    delete controller;
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
    controller->SetManipulator(shapes);
    document->AddDynamicObjects(shapes);
    document->DislayAll();
}
