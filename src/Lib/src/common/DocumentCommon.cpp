#include "DocumentCommon.h"

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

namespace App::Common
{

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
        myManipulator = new App::Common::RobotManipulator;
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

    void DocumentCommon::ActivateManipulator(Handle(AIS_InteractiveObject) object)
    {
        for (auto& dun : dynamicObjects)
            for ( auto& seg : dun->GetSegmnets())
                if( seg->IsPart(object))
                {
                    myManipulator->Attach(dun, seg, object);
                    ObjectActiveted = true;
                    return;
                }
    }

    void DocumentCommon::DislayAll()
    {
        myContext->RemoveAll(Standard_False);

        for (auto& el : dynamicObjects)
        {
            for (auto& seg : el->GetSegmnets())
            {
                for (auto& ais : seg->GetAISShapes())
                    myContext->Display(ais, 0);
            }
        }
        myViewer->Redraw();
    }

    void DocumentCommon::AddDynamicObjects(std::shared_ptr<App::Data::CollectionShapes> collection)
    {
        dynamicObjects.push_back(collection);
    }


    void DocumentCommon::Clear()
    {
        myContext->RemoveAll(Standard_True);
        myContextIsEmpty = true;
    }

    void DocumentCommon::DeactivateManipulator()
    {
        if (ObjectActiveted)
        {
            myManipulator->Detach();
            ObjectActiveted = false;
        }
    }

    const Handle(AIS_InteractiveContext)& DocumentCommon::GetContext() { return myContext; }

    const Handle(V3d_Viewer)& DocumentCommon::GetViewer() { return myViewer; }

}
