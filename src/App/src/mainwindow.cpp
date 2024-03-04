#include "inc/mainwindow.h"
#include "V3d_Viewer.hxx"
#include "inc/View.h"

#include <QToolBar>
#include <QTreeView>
#include <QMessageBox>
#include <QDockWidget>

#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Pln.hxx>

#include <gp_Lin2d.hxx>

#include <Geom_ConicalSurface.hxx>
#include <Geom_ToroidalSurface.hxx>
#include <Geom_CylindricalSurface.hxx>

#include <GCE2d_MakeSegment.hxx>

#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TColgp_Array1OfPnt2d.hxx>

#include <BRepLib.hxx>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>

#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeTorus.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>

#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>

#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>

#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>

#include <TopoDS_CompSolid.hxx>

#include <AIS_Animation.hxx>
#include <AIS_AnimationObject.hxx>
#include <STEPControl_Reader.hxx>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    createMainWidget();
    createActions();
    createMenus();
    createToolBars();
}

MainWindow::~MainWindow()
{

}

void MainWindow::createActions()
{
    connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
    // View
    connect(ui.actionZoom, SIGNAL(triggered()), myView, SLOT(zoom()));
    connect(ui.actionPan, SIGNAL(triggered()), myView, SLOT(pan()));
    connect(ui.actionRotate, SIGNAL(triggered()), myView, SLOT(rotate()));

    connect(ui.actionReset, SIGNAL(triggered()), myView, SLOT(reset()));
    connect(ui.actionFitAll, SIGNAL(triggered()), myView, SLOT(fitAll()));
    // Help
    connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{}

void MainWindow::createToolBars()
{
    QToolBar* aToolBar = addToolBar(tr("&Navigate"));
    aToolBar->addAction(ui.actionZoom);
    aToolBar->addAction(ui.actionPan);
    aToolBar->addAction(ui.actionRotate);

    aToolBar = addToolBar(tr("&View"));
    aToolBar->addAction(ui.actionReset);
    aToolBar->addAction(ui.actionFitAll);


    aToolBar = addToolBar(tr("Help"));
    aToolBar->addAction(ui.actionAbout);
}

void MainWindow::createMainWidget(){
    myView = new View(this);
    myView->setMinimumSize(650,100);

    setCentralWidget(myView);
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About occQt"),
                       tr("<h2>occQt 2.0</h2>"
                          "<p>Copyright &copy; 2014 eryar@163.com"
                          "<p>occQt is a demo applicaton about Qt and OpenCASCADE."));
}
