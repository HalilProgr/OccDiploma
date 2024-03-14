#define _USE_MATH_DEFINES

#include "kinematic.h"
#include <cmath>


#define toRadians M_PI/180.0
#define toDegrees (180.0/M_PI)


void kinematic::init()
{
    chain.addSegment(KDL::Segment("J0",KDL::Joint(KDL::Joint::RotZ), KDL::Frame(KDL::Vector(25.0,0.0,400.0))));      //0.0 to J2
    chain.addSegment(KDL::Segment("J1",KDL::Joint(KDL::Joint::RotY), KDL::Frame(KDL::Vector(0.0,0.0,455.0))));       //J2 to J3
    chain.addSegment(KDL::Segment("J2",KDL::Joint(KDL::Joint::RotY), KDL::Frame(KDL::Vector(0.0,0.0,35.0))));        //J3 to J5
    chain.addSegment(KDL::Segment("J3",KDL::Joint(KDL::Joint::RotX), KDL::Frame(KDL::Vector(420.0,0.0,0.0))));       //J4
    chain.addSegment(KDL::Segment("J4",KDL::Joint(KDL::Joint::RotY), KDL::Frame(KDL::Vector(80.0,0.0,0.0))));        //J5 to end-effector (robot flange axis 6)
    chain.addSegment(KDL::Segment("J5",KDL::Joint(KDL::Joint::RotX), KDL::Frame(KDL::Vector(105.0,0.0,0.0))));       //Tool cone lenght.

    jointMin.resize(chain.getNrOfSegments());
    jointMax.resize(chain.getNrOfSegments());
    jointCur.resize(chain.getNrOfSegments());
    jointInit.resize(chain.getNrOfSegments());

    jointInit(0)= 0 * toRadians; //joint to radians
    jointCur(0)= 0 * toRadians;
    jointMin(0)= -170 * toRadians;
    jointMax(0)= 170 * toRadians;

    jointInit(1)= 0 * toRadians; //joint to radians
    jointCur(1)= 0 * toRadians;
    jointMin(1)= -100 * toRadians;
    jointMax(1)= 135 * toRadians;

    jointInit(2)= 0 * toRadians; //joint to radians
    jointCur(2)= 0 * toRadians;
    jointMin(2)= -210 * toRadians;
    jointMax(2)= 66 * toRadians;

    jointInit(3)= 0 * toRadians; //joint to radians
    jointCur(3)= 0 * toRadians;
    jointMin(3)= -185 * toRadians;
    jointMax(3)= 185 * toRadians;

    jointInit(4)= 0 * toRadians; //joint to radians
    jointCur(4)= 0 * toRadians;
    jointMin(4)= -120 * toRadians;
    jointMax(4)= 120 * toRadians;

    jointInit(5)= 0 * toRadians; //joint to radians
    jointCur(5)= 0 * toRadians;
    jointMin(5)= -350 * toRadians;
    jointMax(5)= 350 * toRadians;
}
