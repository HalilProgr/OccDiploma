#include "mainwindow.hpp"

#include <QVBoxLayout>
#include <QFrame>
#include <QSplitter>
#include <AIS_ViewCube.hxx>
#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeBox.hxx>



#include "view/DocumentCommon.hpp"

ApplicationCommonWindow::ApplicationCommonWindow()
: QMainWindow (nullptr)
{



    // dummy shape for testing
    TopoDS_Shape aBox = BRepPrimAPI_MakeBox (100.0, 50.0, 90.0).Shape();
    Handle(AIS_Shape) aShape = new AIS_Shape (aBox);

    shapes.Append(aShape);

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
