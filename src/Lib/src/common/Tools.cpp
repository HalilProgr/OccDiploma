#include "Tools.h"


namespace Lib::Tools
{

    void printFrame(KDL::Frame& input)
    {
        std::cout<< std::string('-', 10) << std::endl;
        for(int row = 0; row < 4; row++ )
        {
            for(int col = 0; col < 4; col++)
                std::cout << input(row, col) << " ";
            std::cout << std::endl;
        }
    }

    void printTransform(gp_Trsf& input)
    {
        std::cout<< std::string('_', 10) << std::endl;
        for(int row = 1; row < 4; row++ )
        {
            for(int col = 1; col < 5; col++)
                std::cout << input.Value(row, col) << " ";
            std::cout << std::endl;
        }
    }

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

        res.SetValues(
            input(0,0),input(0,1),input(0,2),input(0,3),
            input(1,0),input(1,1),input(1,2),input(1,3),
            input(2,0),input(2,1),input(2,2),input(2,3)
            );

        return res;
    }

    KDL::Frame TrsfToFrame(gp_Trsf& input)
    {
        KDL::Frame res;

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
                res.M(i,j) = input.Value(i+1, j+1);
            res.p[i] = input.Value(i+1, 4);
        }

        return res;
    }

}
