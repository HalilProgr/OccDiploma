#include <QDebug>
#include <kdl/chainfksolverpos_recursive.hpp>
#include <kdl/chainiksolvervel_pinv.hpp>
#include <kdl/chainiksolverpos_nr.hpp>
#include "kinematic.h"

#define _USE_MATH_DEFINES
#define toRadians (M_PI/180.0)
#define toDegrees (180.0/M_PI)

namespace Lib::Kinematic
{
    kinematic::kinematic() : _fksolver(_chain), _iksolverv(_chain) {}

    void kinematic::Init(std::vector<std::shared_ptr<Data::Segment>>& segments)
    {
        // size - 1 потому что сегменты не учитывают базу.
        _jointMin.resize(segments.size() - 1);
        _jointMax.resize(segments.size() - 1);
        _jointCur.resize(segments.size() - 1);
        _jointInit.resize(segments.size() - 1);
        _temp.resize(segments.size() - 1);

        int count = 0;
        for (auto& ptrSegment : segments)
        {
            if (ptrSegment->GetMode() == Data::Mode::base) continue;

            _chain.addSegment(SegmentToKDLSegment(count,ptrSegment));

            _jointInit(count) = ptrSegment->GetLimitations().init * toRadians;
            _jointCur(count) = ptrSegment->GetLimitations().init * toRadians;
            _jointMin(count) = ptrSegment->GetLimitations().min * toRadians;
            _jointMax(count) = ptrSegment->GetLimitations().max * toRadians;
            count++;
        }

        _temp = _jointInit;

        ForwardKinematic(_jointInit, _curTcp);
    }

    void kinematic::MoveSegment(int numberSegment, double deltaValue)
    {
        double value = _jointCur(numberSegment) + deltaValue;

        if (value < _jointMin(numberSegment) || value > _jointMax(numberSegment))
            return;
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

    KDL::Segment kinematic::SegmentToKDLSegment(int count, std::shared_ptr<Data::Segment>& ptrSegment)
    {
        std::string table = "J" + std::to_string(count);
        KDL::Frame frame = KDL::Frame(ptrSegment->GetGeom());

        switch (ptrSegment->GetMode())
        {
        case Data::Mode::RotZ:
            return KDL::Segment(table, KDL::Joint(KDL::Joint::RotZ), frame);
        case Data::Mode::RotY:
            return KDL::Segment(table, KDL::Joint(KDL::Joint::RotY), frame);
        case Data::Mode::RotX:
            return KDL::Segment(table, KDL::Joint(KDL::Joint::RotX), frame);
        case Data::Mode::tool:
            return KDL::Segment(table, KDL::Joint(KDL::Joint::RotX), frame);
        }

        return KDL::Segment(table, KDL::Joint(KDL::Joint::RotAxis), frame);
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
        KDL::ChainFkSolverPos_recursive fksolver1(_chain);//Forward position solver
        KDL::ChainIkSolverVel_pinv iksolver1v(_chain);//Inverse velocity solver
        KDL::ChainIkSolverPos_NR iksolver1(_chain,fksolver1,iksolver1v,100,1e-6);
        //KDL::ChainIkSolverPos_NR_JL _iksolver(_chain, _jointMin, _jointMax, _fksolver, _iksolverv);
        //int kinematics_status = _iksolver.CartToJnt(q_inp, _newTcp, q_out);
        int kinematics_status = iksolver1.CartToJnt(q_inp, _newTcp, q_out);

        if (kinematics_status < 0)
            return false;

        return true;
    }
}
