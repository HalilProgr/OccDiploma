#include <OpenGl_GraphicDriver.hxx>

#include "inc/View.h"

#include <AIS_ViewController.hxx>

#include <QMenu>
#include <QMouseEvent>
#include <QRubberBand>
#include <QStyleFactory>

#include <V3d_View.hxx>

#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>

#include <AIS_Shape.hxx>
#include <AIS_Animation.hxx>
#include <AIS_AnimationObject.hxx>

#include <STEPControl_Reader.hxx>

#include <BRepPrimAPI_MakeBox.hxx>

#include <TopoDS_Solid.hxx>
#include <gp_Trsf.hxx>
#include <Standard_Real.hxx>
#include <ProjLib.hxx>
#include <ElSLib.hxx>

#include "inc/myaisshape.h"

#include "inc/myaisowner.h"
#define WNT

#ifdef WNT
#include <WNT_Window.hxx>
#elif defined(__APPLE__) && !defined(MACOSX_USE_GLX)
#include <Cocoa_Window.hxx>
#else
#undef Bool
#undef CursorShape
#undef None
#undef KeyPress
#undef KeyRelease
#undef FocusIn
#undef FocusOut
#undef FontChange
#undef Expose
#include <Xw_Window.hxx>
#endif

static Handle(Graphic3d_GraphicDriver)& GetGraphicDriver()
{
    static Handle(Graphic3d_GraphicDriver) aGraphicDriver;
    return aGraphicDriver;
}

View::View(QWidget* parent )
    : QWidget(parent),
    myXmin(0),
    myYmin(0),
    myXmax(0),
    myYmax(0),
    myCurrentMode(CurAction3d_DynamicRotation),
    myDegenerateModeIsOn(Standard_True),
    myRectBand(NULL)
{

    theViewCtrl = new AIS_ViewController();

    // No Background
    setBackgroundRole( QPalette::NoRole );

    // set focus policy to threat QContextMenuEvent from keyboard
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);

    // Enable the mouse tracking, by default the mouse tracking is disabled.
    setMouseTracking( true );
    init();
    InitGeom();
}

void View::init()
{
    // Create Aspect_DisplayConnection
    Handle(Aspect_DisplayConnection) aDisplayConnection =
        new Aspect_DisplayConnection();

    // Get graphic driver if it exists, otherwise initialise it
    if (GetGraphicDriver().IsNull())
    {
        GetGraphicDriver() = new OpenGl_GraphicDriver(aDisplayConnection);
    }

    // Get window handle. This returns something suitable for all platforms.
    WId window_handle = (WId) winId();

    // Create appropriate window for platform
#ifdef WNT
    Handle(WNT_Window) wind = new WNT_Window((Aspect_Handle) window_handle);
#elif defined(__APPLE__) && !defined(MACOSX_USE_GLX)
    Handle(Cocoa_Window) wind = new Cocoa_Window((NSView *) window_handle);
#else
    Handle(Xw_Window) wind = new Xw_Window(aDisplayConnection, (Window) window_handle);
#endif

    //Create V3dViewer and V3d_View
    //myViewer = new V3d_Viewer(GetGraphicDriver);

    myViewer = new V3d_Viewer(GetGraphicDriver());
    myView = myViewer->CreateView();

    myView->SetWindow(wind);
    if (!wind->IsMapped()) wind->Map();

    // Create AISInteractiveContext
    myContext = new AIS_InteractiveContext(myViewer);

    // Set up lights etc
    myViewer->SetDefaultLights();
    myViewer->SetLightOn();

    myView->SetBackgroundColor(Quantity_NOC_BLACK);
    myView->MustBeResized();
    myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_ZBUFFER);

    myContext->SetDisplayMode(AIS_Shaded, Standard_True);
}

const Handle(AIS_InteractiveContext)& View::getContext() const
{
    return myContext;
}

/*!
Get paint engine for the OpenGL viewer. [ virtual public ]
*/
QPaintEngine* View::paintEngine() const
{
    return 0;
}

void View::paintEvent( QPaintEvent* /*theEvent*/ )
{
    myView->Redraw();
}

void View::resizeEvent( QResizeEvent* /*theEvent*/ )
{
    if( !myView.IsNull() )
    {
        myView->MustBeResized();
    }
}

void View::fitAll( void )
{
    myView->FitAll();
    myView->ZFitAll();
    myView->Redraw();
}

void View::reset( void )
{
    myView->Reset();
}

void View::pan( void )
{
    myCurrentMode = CurAction3d_DynamicPanning;
}

void View::zoom( void )
{
    myCurrentMode = CurAction3d_DynamicZooming;
}

void View::rotate( void )
{
    myCurrentMode = CurAction3d_DynamicRotation;
}

void View::mousePressEvent( QMouseEvent* theEvent )
{
    if (theEvent->button() == Qt::LeftButton)
    {
        onLButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
    }
    else if (theEvent->button() == Qt::MiddleButton)
    {
        onMButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
    }
    else if (theEvent->button() == Qt::RightButton)
    {
        onRButtonDown((theEvent->buttons() | theEvent->modifiers()), theEvent->pos());
    }
}

void View::mouseReleaseEvent( QMouseEvent* theEvent )
{
    if (theEvent->button() == Qt::LeftButton)
    {
        onLButtonUp(theEvent->buttons() | theEvent->modifiers(), theEvent->pos());
    }
    else if (theEvent->button() == Qt::MiddleButton)
    {
        onMButtonUp(theEvent->buttons() | theEvent->modifiers(), theEvent->pos());
    }
    else if (theEvent->button() == Qt::RightButton)
    {
        onRButtonUp(theEvent->buttons() | theEvent->modifiers(), theEvent->pos());
    }
}

void View::mouseMoveEvent( QMouseEvent * theEvent )
{
    onMouseMove(theEvent->buttons(), theEvent->pos());
}

void View::wheelEvent( QWheelEvent * theEvent )
{
    onMouseWheel(theEvent->buttons(), theEvent->angleDelta(), theEvent->position());
}

void View::onLButtonDown( const int /*theFlags*/, const QPoint thePoint )
{
    // Save the current mouse coordinate in min.

    myXmin = thePoint.x();
    myYmin = thePoint.y();

    myXmax = thePoint.x();
    myYmax = thePoint.y();
}

void View::onMButtonDown( const int /*theFlags*/, const QPoint thePoint )
{
    // Save the current mouse coordinate in min.
    myXmin = thePoint.x();
    myYmin = thePoint.y();
    myXmax = thePoint.x();
    myYmax = thePoint.y();

    if (myCurrentMode == CurAction3d_DynamicRotation)
    {
        myView->StartRotation(thePoint.x(), thePoint.y());
    }
}

void View::onRButtonDown( const int /*theFlags*/, const QPoint thePoint )
{
    /*
    if(myContext->HasSelectedShape()){
        gp_Trsf traslation;

        Handle_AIS_InteractiveObject aisshape = myContext->SelectedInteractive();
        qDebug() << aisshape->Type();
        Handle(AIS_Shape) aShape = Handle(AIS_Shape)::DownCast(aisshape);
        TopLoc_Location cor =  myContext->Location(aisshape);

        traslation.SetTranslation(gp_Vec(gp_Pnt(0,0,0),getTrueMousePosition(thePoint)));

        //myContext->SetSelected(aisshape, false);
        myContext->SetLocation(aisshape, traslation * myContext->Location(aisshape));
        //myContext->SetSelected(aisshape, true);
    }
*/
}

void View::onMouseWheel( const int /*theFlags*/, const QPoint theDelta, const QPointF thePoint )
{
    Standard_Integer aFactor = 16;

    Standard_Integer aX = thePoint.x();
    Standard_Integer aY = thePoint.y();

    if (theDelta.y() > 0)
    {
        aX += aFactor;
        aY += aFactor;
    }
    else
    {
        aX -= aFactor;
        aY -= aFactor;
    }

    myView->Zoom(thePoint.x(), thePoint.y(), aX, aY);
}


void View::popup( const int /*x*/, const int /*y*/ )
{}

void View::onLButtonUp( const int theFlags, const QPoint thePoint )
{

    // Hide the QRubberBand
    if (myRectBand)
    {
        myRectBand->hide();
    }

    // Ctrl for multi selection.
    if (thePoint.x() == myXmin && thePoint.y() == myYmin)
    {
        if (theFlags & Qt::ControlModifier)
        {
            multiInputEvent(thePoint.x(), thePoint.y());
        }
        else
        {
            inputEvent(thePoint.x(), thePoint.y());
        }
    }

}

void View::onMButtonUp( const int /*theFlags*/, const QPoint thePoint )
{
    if (thePoint.x() == myXmin && thePoint.y() == myYmin)
    {
        panByMiddleButton(thePoint);
    }
}

void View::onRButtonUp( const int /*theFlags*/, const QPoint thePoint )
{
    //popup(thePoint.x(), thePoint.y());
}

void View::onMouseMove( const int theFlags, const QPoint thePoint )
{
    // Draw the rubber band.
    if (theFlags & Qt::LeftButton)
    {
        drawRubberBand(myXmin, myYmin, thePoint.x(), thePoint.y());

        dragEvent(thePoint.x(), thePoint.y());
    }

    // Ctrl for multi selection.
    if (theFlags & Qt::ControlModifier)
    {
        multiMoveEvent(thePoint.x(), thePoint.y());
    }
    else
    {
        moveEvent(thePoint.x(), thePoint.y());
    }

    // Middle button.
    if (theFlags & Qt::MiddleButton)
    {
        switch (myCurrentMode)
        {
        case CurAction3d_DynamicRotation:
            myView->Rotation(thePoint.x(), thePoint.y());
            break;

        case CurAction3d_DynamicZooming:
            myView->Zoom(myXmin, myYmin, thePoint.x(), thePoint.y());
            break;

        case CurAction3d_DynamicPanning:
            myView->Pan(thePoint.x() - myXmax, myYmax - thePoint.y());
            myXmax = thePoint.x();
            myYmax = thePoint.y();
            break;

        default:
            break;
        }
    }


    if(theFlags & Qt::RightButton){
        if(myContext->HasSelectedShape()){
            gp_Trsf traslation;

            Handle_AIS_InteractiveObject aisshape = myContext->SelectedInteractive();
            qDebug() << aisshape->Type();
            Handle(AIS_Shape) aShape = Handle(AIS_Shape)::DownCast(aisshape);
            TopLoc_Location cor =  myContext->Location(aisshape);

            traslation.SetTranslation(gp_Vec(gp_Pnt(0,0,0),getTrueMousePosition(thePoint)));

            myContext->SetSelected(aisshape, false);
            myContext->SetLocation(aisshape, traslation * myContext->Location(aisshape));
            myContext->SetSelected(aisshape, true);
            myContext->Redisplay(aisshape,1,true);
        }
    }
}

void View::dragEvent( const int x, const int y )
{
    myContext->Select(myXmin, myYmin, x, y, myView, Standard_True);

    emit selectionChanged();
}

void View::multiDragEvent( const int x, const int y )
{
    myContext->ShiftSelect(myXmin, myYmin, x, y, myView, Standard_True);

    emit selectionChanged();
}

void View::inputEvent( const int x, const int y )
{
    Q_UNUSED(x);
    Q_UNUSED(y);

    myContext->Select(Standard_True);

    emit selectionChanged();
}

void View::multiInputEvent( const int x, const int y )
{
    Q_UNUSED(x);
    Q_UNUSED(y);

    myContext->ShiftSelect(Standard_True);

    emit selectionChanged();
}

void View::moveEvent( const int x, const int y )
{
    myContext->MoveTo(x, y, myView, Standard_True);
}

void View::multiMoveEvent( const int x, const int y )
{
    myContext->MoveTo(x, y, myView, Standard_True);
}

void View::drawRubberBand( const int minX, const int minY, const int maxX, const int maxY )
{
    QRect aRect;

    // Set the rectangle correctly.
    (minX < maxX) ? (aRect.setX(minX)) : (aRect.setX(maxX));
    (minY < maxY) ? (aRect.setY(minY)) : (aRect.setY(maxY));

    aRect.setWidth(abs(maxX - minX));
    aRect.setHeight(abs(maxY - minY));

    if (!myRectBand)
    {
        myRectBand = new QRubberBand(QRubberBand::Rectangle, this);

        // setStyle is important, set to windows style will just draw
        // rectangle frame, otherwise will draw a solid rectangle.
        myRectBand->setStyle(QStyleFactory::create("windows"));
    }

    myRectBand->setGeometry(aRect);
    myRectBand->show();
}

void View::panByMiddleButton( const QPoint& thePoint )
{
    Standard_Integer aCenterX = 0;
    Standard_Integer aCenterY = 0;

    QSize aSize = size();

    aCenterX = aSize.width() / 2;
    aCenterY = aSize.height() / 2;

    myView->Pan(aCenterX - thePoint.x(), thePoint.y() - aCenterY);
}

void View::InitGeom(){
    TopoDS_Shape box = BRepPrimAPI_MakeBox(50, 50, 50).Shape();
    Handle(MyAisShape) AIS_box = new MyAisShape(box);
    AIS_box->SetOwner(new MyAisOwner(AIS_box));

    auto anim = theViewCtrl->ObjectsAnimation();
    //AIS_box->SetAnimation (getContext() ->ObjectsAnimation());

    getContext()->Display(AIS_box, false);
}


gp_Pnt View::getTrueMousePosition(const QPoint point)
{
    gp_Pnt point3d = convert2DPointTo3DPoint(point.x(),point.y(), myView); // convert2DPointTo3DPoint convert2DpointTo3DPointOnPlane
    gp_Pnt pointTest(point3d.X(),point3d.Y(),point3d.Z());
    return pointTest;
}

gp_Pnt View::convert2DPointTo3DPoint(Standard_Real x, Standard_Real y, Handle(V3d_View) aView)
{
    Standard_Real  XEye,YEye,ZEye,XAt,YAt,ZAt;
    aView->Eye(XEye,YEye,ZEye);
    aView->At(XAt,YAt,ZAt);
    gp_Pnt EyePoint(XEye,YEye,ZEye);
    gp_Pnt AtPoint(XAt,YAt,ZAt);

    gp_Vec EyeVector(EyePoint,AtPoint);
    gp_Dir EyeDir(EyeVector);

    gp_Pln PlaneOfTheView = gp_Pln(AtPoint,EyeDir);
    Standard_Real X,Y,Z;
    aView->Convert(int(x),int(y),X,Y,Z);
    gp_Pnt ConvertedPoint(X,Y,Z);
    gp_Pnt2d ConvertedPointOnPlane = ProjLib::Project(PlaneOfTheView,ConvertedPoint);

    gp_Pnt ResultPoint = ElSLib::Value(ConvertedPointOnPlane.X(),
                                       ConvertedPointOnPlane.Y(),
                                       PlaneOfTheView);

    return ResultPoint;
}
