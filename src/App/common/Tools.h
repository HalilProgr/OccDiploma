#ifndef TOOLS_H
#define TOOLS_H

#include <kdl/frames.hpp>
#include <gp_Trsf.hxx>


//! x' = a11 x + a12 y + a13 z + a14
//! y' = a21 x + a22 y + a23 z + a24
//! z' = a31 x + a32 y + a33 z + a34
/**
    \brief represents a frame transformation in 3D space (rotation + translation)

    if V2 = Frame*V1 (V2 expressed in frame A, V1 expressed in frame B)
    then V2 = Frame.M*V1+Frame.p

    Frame.M contains columns that represent the axes of frame B wrt frame A
    Frame.p contains the origin of frame B expressed in frame A.
*/
namespace App
{
    namespace Tools
    {

    gp_Trsf FrameToTrsf (KDL::Frame& input )
    {
        gp_Trsf res;

        auto& M = input.M;
        auto& P = input.p;
        res.SetValues(
            M(0,0),M(0,1),M(0,2),P(0),
            M(1,0),M(1,1),M(1,2),P(1),
            M(2,0),M(2,1),M(2,2),P(2));
        /*
        res.SetValues(
            input(0,0),input(1,1),input(2,0),input(3,0),
            input(1,0),input(1,1),input(2,1),input(3,1),
            input(2,0),input(2,1),input(2,2),input(3,2));
*/
        return res;
    }



}}

#endif // TOOLS_H
