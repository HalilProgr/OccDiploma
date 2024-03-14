#include "DocumentCommon.hpp"

#include <Standard_WarningsDisable.hxx>
#include <QApplication>
#include <QColor>
#include <QColorDialog>
#include <QStatusBar>
#include <Standard_WarningsRestore.hxx>

#include <AIS_InteractiveObject.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <Graphic3d_NameOfMaterial.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <OSD_Environment.hxx>

#include <TCollection_AsciiString.hxx>


Handle(V3d_Viewer) DocumentCommon::Viewer()
{
    Handle(Aspect_DisplayConnection) aDisp = new Aspect_DisplayConnection();
    Handle(OpenGl_GraphicDriver) aDriver = new OpenGl_GraphicDriver (aDisp, false);
    // lets QOpenGLWidget to manage buffer swap
    aDriver->ChangeOptions().buffersNoSwap = true;
    // don't write into alpha channel
    aDriver->ChangeOptions().buffersOpaqueAlpha = true;
    // offscreen FBOs should be always used
    aDriver->ChangeOptions().useSystemBuffer = false;

    // create viewer
    myViewer = new V3d_Viewer (aDriver);
    myViewer->SetDefaultBackgroundColor (Quantity_NOC_BLACK);
    myViewer->SetDefaultLights();
    myViewer->SetLightOn();
    myViewer->ActivateGrid (Aspect_GT_Rectangular, Aspect_GDM_Lines);

    return myViewer;
}

DocumentCommon::DocumentCommon(QWidget* theApp)
    : QObject (theApp), myContextIsEmpty(true), ObjectActiveted(false)
{
    myManipulator = new AIS_Manipulator;
    myViewer = Viewer();
    myContext = new AIS_InteractiveContext(myViewer);
}

DocumentCommon::~DocumentCommon()
{
    if(!myContextIsEmpty)
         myContext->RemoveAll(false);
    myContext.Nullify();
    myViewer.Nullify();
    // release OCCT viewer
    myViewer.Nullify();
}

void DocumentCommon::SetObjects (const NCollection_Vector<Handle(AIS_InteractiveObject)>& theObjects,
                                 Standard_Boolean theDisplayShaded)
{
    for(NCollection_Vector<Handle(AIS_InteractiveObject)>::Iterator anIter(theObjects);
         anIter.More(); anIter.Next())
    {
        const Handle(AIS_InteractiveObject)& anObject = anIter.Value();
        if (!theDisplayShaded)
            myContext->Display(anObject, Standard_False);
        else
            myContext->Display(anObject, AIS_Shaded, 0, Standard_False);
    }

    myViewer->Redraw();
}

void DocumentCommon::ActivateObject(Handle(AIS_InteractiveObject) object)
{
    myManipulator->Attach(object);
    ObjectActiveted = true;
}


void DocumentCommon::setMousePosition(int anXPix, int anYPix, Handle(V3d_View) view)
{
    if (myManipulator->HasActiveMode())
        myManipulator->StartTransform (anXPix, anYPix, view);
}

void DocumentCommon::Clear()
{
    myContext->RemoveAll(Standard_True);
    myContextIsEmpty = true;
}

void DocumentCommon::DeactivateObject()
{
    if (ObjectActiveted)
    {
        myManipulator->Detach();
        ObjectActiveted = false;
    }
}
