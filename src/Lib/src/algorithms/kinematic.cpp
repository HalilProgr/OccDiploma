#include <QDebug>
#include "kinematic.h"



namespace App::Kinematic
{
    kinematic::kinematic()
        : _fksolver(_chain),
          _iksolverv(_chain)
    {}

    void kinematic::Init(std::vector<std::shared_ptr<Data::Segment>>& segments)
    {
        // size - 1 потому что сегменты учитывают базу.
        _jointMin.resize(segments.size() - 1);
        _jointMax.resize(segments.size() - 1);
        _jointCur.resize(segments.size() - 1);
        _jointInit.resize(segments.size() - 1);
        _temp.resize(segments.size() - 1);

        int count = 0;

        for (auto & ptr: segments)
        {
            if (ptr->GetMode() == Data::Mode::base) continue;

            _chain.addSegment(SegmentToKDLSegment(count,ptr));

            _jointInit(count) = ptr->GetLimitations().init * toRadians;
            _jointCur(count) = ptr->GetLimitations().init * toRadians;
            _jointMin(count) = ptr->GetLimitations().min * toRadians;
            _jointMax(count) = ptr->GetLimitations().max * toRadians;

            count++;
        }

        _temp = _jointInit;

        if (ForwardKinematic(_temp, _curTcp))
            qDebug() << "Init Kinematic done!";
        else
            qDebug() << "Error Kinematic init!";
    }

    void kinematic::MoveSegment(int numberSegment, double deltaValue)
    {
        double value = _jointCur(numberSegment) + deltaValue;
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

    KDL::Segment kinematic::SegmentToKDLSegment(int count, std::shared_ptr<Data::Segment>& ptrSegment)
    {
        std::string table = "J" + std::to_string(count);
        KDL::Joint join;

        KDL::Frame frame = KDL::Frame(
            KDL::Vector(
                ptrSegment->GetAxis().Location().X(),
                ptrSegment->GetAxis().Location().Y(),
                ptrSegment->GetAxis().Location().Z()
                ));

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
        KDL::ChainIkSolverPos_NR_JL _iksolver(_chain, _jointMin, _jointMax, _fksolver, _iksolverv);
        int kinematics_status = _iksolver.CartToJnt(_jointCur, _newTcp, q_out);

        if (kinematics_status < 0)
            return false;

        return true;
    }
}
