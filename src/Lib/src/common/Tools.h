#ifndef TOOLS_H
#define TOOLS_H

#include <QString>
#include <kdl/frames.hpp>
#include <gp_Trsf.hxx>
#include <gp_Ax1.hxx>
#include "src/data/CommonType.h"

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

    gp_Ax1 Axis(gp_Pnt point, Data::Mode mode);

    Data::Mode Mode(QString mode);

    gp_Trsf FrameToTrsf (KDL::Frame& input );

    KDL::Frame TrsfToFrame(gp_Trsf input);

}}

#endif // TOOLS_H
