#include "Segment.h"

namespace Lib
{
namespace Data {

    Segment::Segment(std::vector<Handle (AIS_InteractiveObject)> shapes,
                 gp_Pnt crsys,
                 Mode mode,
                 Limit limit)
        : _objects(shapes),
          _mode(mode),
          _limit(limit)
    {
        switch (mode)
        {
        case Mode::base:
            _coorsys = gp_Ax1(crsys, gp_Dir(0,0,1));
            break;
        case Mode::RotZ:
            _coorsys = gp_Ax1(crsys, gp_Dir(0,0,1));
            break;
        case Mode::RotY:
            _coorsys = gp_Ax1(crsys, gp_Dir(0,1,0));
            break;
        case Mode::RotX:
            _coorsys = gp_Ax1(crsys, gp_Dir(1,0,0));
            break;
        case Mode::tool:
            _coorsys = gp_Ax1(crsys, gp_Dir(1,0,0));
            break;
        default:
            _coorsys = gp_Ax1(crsys, gp_Dir(0,0,1));
        }
    }

    void Segment::SetGeom(double x, double y, double z)
    {
        _geom = KDL::Vector(x, y, z);
    }

    bool Segment::IsPart(Handle(AIS_InteractiveObject)& object)
    {
        for(auto& element : _objects)
            if(element == object)
                return true;

        return false;
    }

    void Segment::SetTransform(gp_Trsf& newPos)
    {
        _currentTransform = newPos;

        for(auto& element : _objects)
            element->SetLocalTransformation(_currentTransform);
    }

    void Segment::SetMode(Mode newMode)
    {
        _mode = newMode;
    }

    void Segment::SetAxis(gp_Ax1 sys)
    {
        _coorsys = sys;
    }

    void Segment::SetLimitations(Limit limit)
    {
        _limit = limit;
    }

    KDL::Vector Segment::GetGeom()
    {
        return _geom;
    }

    gp_Ax1 Segment::GetAxis()
    {
        return _coorsys;
    }

    Segment::Limit Segment::GetLimitations()
    {
        return _limit;
    }

    gp_Trsf Segment::GetTransform()
    {
        return _currentTransform;
    }

    Mode Segment::GetMode()
    {
        return _mode;
    }

    void Segment::SetShapes(std::vector<Handle (AIS_InteractiveObject)> shapes)
    {
        _objects = shapes;
    }

    void Segment::AddAISShape(Handle (AIS_InteractiveObject) shape)
    {
        _objects.push_back(shape);
    }

    std::vector<Handle (AIS_InteractiveObject)> Segment::GetAISShapes()
    {
        return _objects;
    }
    }}
