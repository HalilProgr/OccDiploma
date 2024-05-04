#include "Tool.h"
#include <TopoDS_Shape.hxx>
#include <AIS_Shape.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <QDebug>


namespace Lib
{
namespace Data {


    Tool::Tool(gp_Pnt startPnt, gp_Ax2 axis,
           double lenght, double height,
           double bottomDiameter, double topDiameter)
    {
        gp_Pnt point = {startPnt.X() - height, startPnt.Y(), startPnt.Z()};
        TopoDS_Shape cone = BRepPrimAPI_MakeCone(axis, bottomDiameter, topDiameter, height).Shape();

        _object = new AIS_Shape(cone);

        auto dir = axis.Direction();

        _currentTCP.SetValues(1,0,0, startPnt.X(),
                              0,1,0, startPnt.Y(),
                              0,0,1, startPnt.Z());

        qDebug() << "----------------------------";
        qDebug() << dir.X();
        qDebug() << dir.Y();
        qDebug() << dir.Z();

        _axis = gp_Ax1(gp_Pnt(point.X(), point.Y(), point.Z()), axis.Direction());

        _object->SetColor(Quantity_NOC_GREEN);
        _object->SetTransparency(0.5);
        _object->SetMaterial(Graphic3d_NOM_PLASTIC);
        //_currentPosition.SetRotation(gp_Ax1(point, axis.Direction()));
    }

    bool Tool::IsPart (Handle(AIS_InteractiveObject)& object)
    {
        if (_object == object) return true;
        return false;
    }

    void Tool::SetTCP (gp_Trsf& newTCP)
    {
        _currentTCP = newTCP;
    }

    void Tool::SetTCP (gp_Trsf newTCP)
    {
        _currentTCP = newTCP;
    }

    void Lib::Data::Tool::SetTransform(gp_Trsf &newPosition)
    {
        _object->SetLocalTransformation(newPosition);
    }

    Mode Tool::GetMode()
    {
        return Mode::tool;
    }

    gp_Trsf Tool::GetTCP()
    {
        return _currentTCP;
    }

    gp_Trsf Tool::GetTransform()
    {
        return _currentPosition;
    }

    Handle (AIS_InteractiveObject) Tool::GetAISShape()
    {
        return _object;
    }
}}
