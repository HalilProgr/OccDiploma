#include "Tools.h"


namespace App
{
    namespace Tools
    {

    gp_Ax1 Axis(gp_Pnt point, Data::Mode mode)
    {
        switch (mode)
        {
        case Data::Mode::base:
            return gp_Ax1(point, gp_Dir(0,0,1));
        case Data::Mode::RotZ:
            return gp_Ax1(point, gp_Dir(0,0,1));
        case Data::Mode::RotY:
            return gp_Ax1(point, gp_Dir(0,1,0));
        case Data::Mode::RotX:
            return gp_Ax1(point, gp_Dir(1,0,0));
        case Data::Mode::tool:
            return gp_Ax1(point, gp_Dir(1,0,0));
        default:
            return gp_Ax1(point, gp_Dir(0,0,1));
        }
    }

    Data::Mode Mode(QString mode)
    {
        if (mode == "base")
            return Data::Mode::base;
        else if(mode == "RotZ")
            return Data::Mode::RotZ;
        else if(mode == "RotY")
            return Data::Mode::RotY;
        else if(mode == "RotX")
            return Data::Mode::RotX;
        else if(mode == "tool")
            return Data::Mode::tool;

        return Data::Mode::none;
    }

    gp_Trsf FrameToTrsf (KDL::Frame& input )
    {
        gp_Trsf res;

        auto& M = input.M;
        auto& P = input.p;
        res.SetValues(
            M(0,0),M(0,1),M(0,2),P(0),
            M(1,0),M(1,1),M(1,2),P(1),
            M(2,0),M(2,1),M(2,2),P(2));

        return res;
    }

    KDL::Frame TrsfToFrame(gp_Trsf input)
    {
        KDL::Vector vec;
        KDL::Rotation rot;

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
                rot(i, j) = input.Value(i+1, j+1);
        }

        for (int i = 0; i < 3; i++)
        {
            vec(i) = input.Value(i+1, 4);
        }

        return KDL::Frame(rot, vec);
    }

}}
