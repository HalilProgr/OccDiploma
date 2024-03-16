#include <QDebug>
#include "kinematic.h"



namespace App::Kinematic
{
kinematic::kinematic()
    : _fksolver(_chain),
      _iksolverv(_chain)
{}

void kinematic::init()
    {
        _chain.addSegment(KDL::Segment("J0",KDL::Joint(KDL::Joint::RotZ), KDL::Frame(KDL::Vector(25.0,0.0,400.0))));  //0.0 to J2
        _chain.addSegment(KDL::Segment("J1",KDL::Joint(KDL::Joint::RotY), KDL::Frame(KDL::Vector(0.0,0.0,455.0))));   //J2 to J3
        _chain.addSegment(KDL::Segment("J2",KDL::Joint(KDL::Joint::RotY), KDL::Frame(KDL::Vector(0.0,0.0,35.0))));    //J3 to J5
        _chain.addSegment(KDL::Segment("J3",KDL::Joint(KDL::Joint::RotX), KDL::Frame(KDL::Vector(420.0,0.0,0.0))));   //J4
        _chain.addSegment(KDL::Segment("J4",KDL::Joint(KDL::Joint::RotY), KDL::Frame(KDL::Vector(80.0,0.0,0.0))));    //J5 to end-effector (robot flange axis 6)
        _chain.addSegment(KDL::Segment("J5",KDL::Joint(KDL::Joint::RotX), KDL::Frame(KDL::Vector(0.0,0.0,0.0))));     //without tool

        _jointMin.resize(_chain.getNrOfSegments());
        _jointMax.resize(_chain.getNrOfSegments());
        _jointCur.resize(_chain.getNrOfSegments());
        _jointInit.resize(_chain.getNrOfSegments());
        _temp.resize(_chain.getNrOfSegments());

        _jointInit(0)= 0 * toRadians; //joint to radians
        _jointCur(0)= 0 * toRadians;
        _jointMin(0)= -170 * toRadians;
        _jointMax(0)= 170 * toRadians;

        _jointInit(1)= 0 * toRadians; //joint to radians
        _jointCur(1)= 0 * toRadians;
        _jointMin(1)= -100 * toRadians;
        _jointMax(1)= 135 * toRadians;

        _jointInit(2)= 0 * toRadians; //joint to radians
        _jointCur(2)= 0 * toRadians;
        _jointMin(2)= -210 * toRadians;
        _jointMax(2)= 66 * toRadians;

        _jointInit(3)= 0 * toRadians; //joint to radians
        _jointCur(3)= 0 * toRadians;
        _jointMin(3)= -185 * toRadians;
        _jointMax(3)= 185 * toRadians;

        _jointInit(4)= 0 * toRadians; //joint to radians
        _jointCur(4)= 0 * toRadians;
        _jointMin(4)= -120 * toRadians;
        _jointMax(4)= 120 * toRadians;

        _jointInit(5)= 0 * toRadians; //joint to radians
        _jointCur(5)= 0 * toRadians;
        _jointMin(5)= -350 * toRadians;
        _jointMax(5)= 350 * toRadians;

        _temp = _jointInit;

        if (ForwardKinematic(_temp, _curTcp))
            qDebug() << "Init Kinematic done!";
        else
            qDebug() << "Error Kinematic init!";
    }

    void kinematic::MoveSegment(int numberSegment, double newValue)
    {
        double value = newValue * toRadians;
        if (value < _jointMin(numberSegment))
            _temp(numberSegment) = _jointMin(numberSegment);
        else if (value > _jointMax(numberSegment))
            _temp(numberSegment) = _jointMax(numberSegment);

        _temp(numberSegment) = value;


        if (!ForwardKinematic(_temp, _curTcp))
        {
            _temp = _jointCur;
            return;
        }

        _jointCur = _temp;
    }

    void kinematic::MoveTCP(KDL::Frame& newTCP)
    {
        if (!InverseKinematic(_jointCur, _temp, newTCP))
        {
            _temp = _jointCur;
            return;
        }

        _jointCur = _temp;
        ForwardKinematic(_jointCur, _curTcp);
    }

    bool kinematic::ForwardKinematic(KDL::JntArray& newPos, KDL::Frame& curTCP)
    {
        int kinematics_status = _fksolver.JntToCart(newPos, curTCP);

        if (kinematics_status < 0)
            return false;

        return true;
    }

    bool kinematic::InverseKinematic(KDL::JntArray& q_inp, KDL::JntArray& q_out, KDL::Frame& _newTcp)
    {
        KDL::ChainIkSolverPos_NR_JL _iksolver(_chain, _jointMin, _jointMax, _fksolver, _iksolverv);
        int kinematics_status = _iksolver.CartToJnt(_jointCur, _newTcp, q_out);

        if (kinematics_status < 0)
            return false;

        return true;
    }
}
