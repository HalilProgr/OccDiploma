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

namespace Lib
{
namespace Common
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
        myManipulator = new Lib::Common::RobotManipulator;
        myViewer = Viewer();
        myContext = new AIS_InteractiveContext(myViewer);
    }

    DocumentCommon::~DocumentCommon()
    {
        if(!myContextIsEmpty)
             myContext->RemoveAll(false);
        myContext.Nullify();
        myViewer.Nullify();
    }

    void DocumentCommon::ActivateManipulator(Handle(AIS_InteractiveObject) object)
    {
        if (ObjectActiveted)
        {
            myManipulator->StopTransform();
            myManipulator->Detach();
            ObjectActiveted = false;
        }

        for (auto& robot : dynamicObjects)
        {
            Data::Manipulator::DescriptionSegment descrp = robot->FindSegment(object);
            if (descrp.segmentPtr != nullptr || descrp.toolPtr != nullptr)
            {
                myManipulator->Attach(robot, descrp);
                ObjectActiveted = true;
                return;
            }
        }
    }

    void DocumentCommon::DeactivateManipulator()
    {
        if (ObjectActiveted)
        {
            myManipulator->StopTransform();
            myManipulator->Detach();
            ObjectActiveted = false;
        }
    }

    void DocumentCommon::DislayAll()
    {
        myContext->RemoveAll(Standard_False);

        for (auto& el : dynamicObjects)
        {
            for (auto& ais : el->GetView())
                myContext->Display(ais, 0);
        }
        myViewer->Redraw();
    }

    void DocumentCommon::AddDynamicObjects(std::shared_ptr<Lib::Data::Manipulator> collection)
    {
        dynamicObjects.push_back(collection);
    }


    void DocumentCommon::Clear()
    {
        myContext->RemoveAll(Standard_True);
        myContextIsEmpty = true;
    }

    const Handle(AIS_InteractiveContext)& DocumentCommon::GetContext() { return myContext; }

    const Handle(V3d_Viewer)& DocumentCommon::GetViewer() { return myViewer; }

}}
