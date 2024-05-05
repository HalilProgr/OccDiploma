#ifndef TOOLS_H
#define TOOLS_H

#include <QString>
#include <kdl/frames.hpp>
#include <gp_Trsf.hxx>
#include <gp_Ax1.hxx>
#include <gp_Quaternion.hxx>
#include <Eigen/Dense>
#include "src/data/CommonType.h"


namespace Lib
{
    namespace Tools
    {

    void printFrame(KDL::Frame& input);

    void printTransform(gp_Trsf& input);

    gp_Ax1 Axis(gp_Pnt point, Data::Mode mode);

    Data::Mode Mode(QString mode);

    gp_Trsf FrameToTrsf(KDL::Frame& input );

    KDL::Frame TrsfToFrame(gp_Trsf& input);

    Eigen::Vector3d TrsfToVector3D(gp_Trsf input);

    gp_Trsf MakeTrsf(gp_Pnt point, gp_Quaternion angl);

}}

#endif // TOOLS_H
