#include "robotmanipulator.h"
#include <gp_Trsf.hxx>
#include <gp_Quaternion.hxx>
#include <QDebug>
#include <AIS_InteractiveContext.hxx>
#include "Tools.h"


namespace Lib::Common
{

    RobotManipulator::RobotManipulator()
    : AIS_Manipulator()
    {
        for (int i = 0; i < 4; i++)
        {
            SetPart (i, AIS_ManipulatorMode::AIS_MM_Scaling, Standard_False);
            SetPart (i, AIS_ManipulatorMode::AIS_MM_Translation, Standard_False);
            SetPart (i, AIS_ManipulatorMode::AIS_MM_Rotation, Standard_False);
        }
    }

    RobotManipulator::RobotManipulator(const gp_Ax2& thePosition)
        : AIS_Manipulator(thePosition)
    {
        for (int i = 0; i < 4; i++)
        {
            SetPart (i, AIS_ManipulatorMode::AIS_MM_Scaling, Standard_False);
            SetPart (i, AIS_ManipulatorMode::AIS_MM_Translation, Standard_False);
            SetPart (i, AIS_ManipulatorMode::AIS_MM_Rotation, Standard_False);
        }
    }

    void RobotManipulator::Attach(std::shared_ptr<Lib::Data::Manipulator>& collection,
                                  Data::Manipulator::DescriptionSegment segment)
    {
        _descp = segment; /// Описание сигмента с которым происходит работа.
        _actualCollection = collection;
        gp_Trsf temp;
        if (_descp.isTool)
        {
            SetMode(_descp.toolPtr->GetMode());
            AIS_Manipulator::Attach(segment.toolPtr->GetAISShape());
            temp = segment.toolPtr->GetTransform();
        }
        else
        {
            SetMode(_descp.segmentPtr->GetMode());
            AIS_Manipulator::Attach(segment.segmentPtr->GetAISShapes().front());
            temp = segment.segmentPtr->GetTransform();
        }
        /// некрасиво, но удобно
        gp_Ax2 local = Position();
        /// debug  в общем еще переделывать.
        Lib::Tools::printTransform(temp);
        /// local.Transform(segment.segmentPtr->GetTransform());
        local = local.Transformed(temp);
        SetPosition(local);
    }

    void RobotManipulator::Detach()
    {
        AIS_Manipulator::Detach();
        SetMode(Data::Mode::none);
        _actualCollection.reset();
    }

    void RobotManipulator::SetMode(Lib::Data::Mode mode)
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
            for(int i = 0; i < 3; i++)
            {
                SetPart (i, AIS_ManipulatorMode::AIS_MM_Translation, Standard_True);
                SetPart (i, AIS_ManipulatorMode::AIS_MM_Rotation, Standard_True);
            }

            EnableMode (AIS_ManipulatorMode::AIS_MM_Rotation);
            EnableMode (AIS_ManipulatorMode::AIS_MM_Translation);
            break;
        default:
            for(int i = 0; i < 3; i++)
            {
                SetPart (i, AIS_ManipulatorMode::AIS_MM_Translation, Standard_False);
                SetPart (i, AIS_ManipulatorMode::AIS_MM_Rotation, Standard_False);
            }
        }

        _mode = mode;
    }

    gp_Trsf RobotManipulator::Transform (const Standard_Integer thePX, const Standard_Integer thePY,
                                         const Handle(V3d_View)& theView)
    {
        gp_Trsf aTrsf;
        gp_Trsf temp;
        if (ObjectTransformation (thePX, thePY, theView, aTrsf))
        {
            switch (_mode)
            {
            case Lib::Data::Mode::RotX:
                _actualCollection->MoveSegment(_descp.numberSegment, aTrsf.GetRotation().X());
                break;
            case Lib::Data::Mode::RotY:
                _actualCollection->MoveSegment(_descp.numberSegment, aTrsf.GetRotation().Y());
                break;
            case Lib::Data::Mode::RotZ:
                _actualCollection->MoveSegment(_descp.numberSegment, aTrsf.GetRotation().Z());
                break;
            case Lib::Data::Mode::tool:
                std::cout << "Transform: " << std::endl;
                Lib::Tools::printTransform(aTrsf);
                temp = _descp.toolPtr->GetTransform();
                std::cout << "Transform from segment: " << std::endl;
                temp = _descp.toolPtr->GetTCP() * aTrsf;
                Lib::Tools::printTransform(temp);
                _actualCollection->MoveTCP(temp);
                break;
            default:
                break;
            }
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
