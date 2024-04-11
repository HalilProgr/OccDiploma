#include "robotmanipulator.h"
#include <gp_Trsf.hxx>
#include <QDebug>


namespace App::Common
{

    RobotManipulator::RobotManipulator()
    : AIS_Manipulator()
    {
        SetPart (0, AIS_ManipulatorMode::AIS_MM_Scaling, Standard_False);
        SetPart (1, AIS_ManipulatorMode::AIS_MM_Scaling, Standard_False);
        SetPart (2, AIS_ManipulatorMode::AIS_MM_Scaling, Standard_False);

        SetPart (0, AIS_ManipulatorMode::AIS_MM_Translation, Standard_False);
        SetPart (1, AIS_ManipulatorMode::AIS_MM_Translation, Standard_False);
        SetPart (2, AIS_ManipulatorMode::AIS_MM_Translation, Standard_False);

        SetPart (0, AIS_ManipulatorMode::AIS_MM_Rotation, Standard_False);
        SetPart (1, AIS_ManipulatorMode::AIS_MM_Rotation, Standard_False);
        SetPart (2, AIS_ManipulatorMode::AIS_MM_Rotation, Standard_False);
    }

    RobotManipulator::RobotManipulator(const gp_Ax2& thePosition)
        : AIS_Manipulator(thePosition)
    {
        SetPart (0, AIS_ManipulatorMode::AIS_MM_Scaling, Standard_False);
        SetPart (1, AIS_ManipulatorMode::AIS_MM_Scaling, Standard_False);
        SetPart (2, AIS_ManipulatorMode::AIS_MM_Scaling, Standard_False);

        SetPart (0, AIS_ManipulatorMode::AIS_MM_Translation, Standard_False);
        SetPart (1, AIS_ManipulatorMode::AIS_MM_Translation, Standard_False);
        SetPart (2, AIS_ManipulatorMode::AIS_MM_Translation, Standard_False);

        SetPart (0, AIS_ManipulatorMode::AIS_MM_Rotation, Standard_False);
        SetPart (1, AIS_ManipulatorMode::AIS_MM_Rotation, Standard_False);
        SetPart (2, AIS_ManipulatorMode::AIS_MM_Rotation, Standard_False);
    }

    void RobotManipulator::Attach(std::shared_ptr<App::Data::CollectionShapes>& collection,
                                  std::shared_ptr<App::Data::Segment>& segment,
                                  Handle(AIS_InteractiveObject)& objToCon)
    {

        SetMode(segment->GetMode());
        //_numberSegment = segment->GetNumberSegment();
        _actualCollection = collection;

        if (_mode != Data::Mode::base)
            AIS_Manipulator::Attach(objToCon);
    }

    void RobotManipulator::Detach()
    {
        SetMode(Data::Mode::none);
        _actualCollection.reset();
        _numberSegment = -1;
    }

    void RobotManipulator::SetMode(App::Data::Mode mode)
    {
        switch(mode)
        {
        case Data::Mode::RotZ:
            SetPart (2, AIS_ManipulatorMode::AIS_MM_Rotation, Standard_True);
            EnableMode (AIS_ManipulatorMode::AIS_MM_Rotation);
            break;
        case Data::Mode::RotY:
            SetPart (1, AIS_ManipulatorMode::AIS_MM_Rotation, Standard_True);
            EnableMode (AIS_ManipulatorMode::AIS_MM_Rotation);
            break;
        case Data::Mode::RotX:
            SetPart (0, AIS_ManipulatorMode::AIS_MM_Rotation, Standard_True);
            EnableMode (AIS_ManipulatorMode::AIS_MM_Rotation);
            break;
        case Data::Mode::tool:
            SetPart (0, AIS_ManipulatorMode::AIS_MM_Translation, Standard_True);
            SetPart (1, AIS_ManipulatorMode::AIS_MM_Translation, Standard_True);
            SetPart (2, AIS_ManipulatorMode::AIS_MM_Translation, Standard_True);

            SetPart (0, AIS_ManipulatorMode::AIS_MM_Rotation, Standard_True);
            SetPart (1, AIS_ManipulatorMode::AIS_MM_Rotation, Standard_True);
            SetPart (2, AIS_ManipulatorMode::AIS_MM_Rotation, Standard_True);
            EnableMode (AIS_ManipulatorMode::AIS_MM_Rotation);
            EnableMode (AIS_ManipulatorMode::AIS_MM_Translation);
            break;
        default:
            SetPart (0, AIS_ManipulatorMode::AIS_MM_Translation, Standard_False);
            SetPart (1, AIS_ManipulatorMode::AIS_MM_Translation, Standard_False);
            SetPart (2, AIS_ManipulatorMode::AIS_MM_Translation, Standard_False);

            SetPart (0, AIS_ManipulatorMode::AIS_MM_Rotation, Standard_False);
            SetPart (1, AIS_ManipulatorMode::AIS_MM_Rotation, Standard_False);
            SetPart (2, AIS_ManipulatorMode::AIS_MM_Rotation, Standard_False);
        }

        _mode = mode;
    }

    gp_Trsf RobotManipulator::Transform (const Standard_Integer thePX, const Standard_Integer thePY,
                                         const Handle(V3d_View)& theView)
    {
        gp_Trsf aTrsf;
        if (ObjectTransformation (thePX, thePY, theView, aTrsf))
        {
            gp_Dir oldVDir = myStartPosition.Direction();
            gp_Dir oldXDir = myStartPosition.XDirection();
            gp_Dir oldYDir = myStartPosition.YDirection();

            gp_Pnt aPos  = myStartPosition.Location().Transformed (aTrsf);

            gp_Dir aVDir = myStartPosition.Direction().Transformed (aTrsf);
            gp_Dir aXDir = myStartPosition.XDirection().Transformed (aTrsf);
            gp_Dir aYDir = myStartPosition.YDirection().Transformed (aTrsf);

            double angle = 0;

            switch (_mode)
            {
            case App::Data::Mode::RotX:
                angle = aVDir.AngleWithRef (oldVDir, aXDir) / 100;
                _actualCollection->MoveSegment(_numberSegment, angle);
                break;
            case App::Data::Mode::RotY:
                angle = aVDir.AngleWithRef (oldVDir, aYDir) / 100;
                _actualCollection->MoveSegment(_numberSegment, angle);
                break;
            case App::Data::Mode::RotZ:
                angle = aXDir.AngleWithRef (oldXDir, aVDir) / 100;
                _actualCollection->MoveSegment(_numberSegment, angle);
                break;
            case App::Data::Mode::tool:
                _actualCollection->MoveTCP(Transformation());
            default:
                angle = 0;
                break;
            }

            qDebug() << angle;
            SetPosition (gp_Ax2 (aPos, aVDir, aXDir));
        }

        return aTrsf;
    }

    Standard_Boolean RobotManipulator::ProcessDragging (const Handle(AIS_InteractiveContext)& theCtx,
                                                        const Handle(V3d_View)& theView,
                                                        const Handle(SelectMgr_EntityOwner)& theOwner,
                                                        const Graphic3d_Vec2i& theDragFrom,
                                                        const Graphic3d_Vec2i& theDragTo,
                                                        const AIS_DragAction theAction)
    {
        switch (theAction)
        {
        case AIS_DragAction_Start:
        {
            if (HasActiveMode())
            {
                StartTransform (theDragFrom.x(), theDragFrom.y(), theView);
                return Standard_True;
            }
            break;
        }
        case AIS_DragAction_Update:
        {
            Transform (theDragTo.x(), theDragTo.y(), theView);
            return Standard_True;
        }
        case AIS_DragAction_Abort:
        {
            StopTransform (false);
            return Standard_True;
        }
        case AIS_DragAction_Stop:
            break;
        }
        return Standard_False;
    }

}
