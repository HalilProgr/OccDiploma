#ifndef KINEMATIC_H
#define KINEMATIC_H

#include "Ikinematic.h"
#include "kdl/chain.hpp"
#include "kdl/chainfksolverpos_recursive.hpp"
#include "kdl/chainiksolvervel_pinv.hpp"
#include "kdl/ChainIkSolverPos_NR_JL.hpp"


namespace Lib
{
    namespace Kinematic
    {

    class kinematic : public IKinematic
    {
    public:
        kinematic();
        ~kinematic() = default;

        ///
        /// \brief SetSegments
        /// \param segments
        ///
        void Init(std::vector<std::shared_ptr<Data::Segment>>& segments) override;
        \
        ///
        /// \brief Переместить звено
        /// \param Номер звена
        /// \param новый угол
        ///
        void MoveSegment(int numberSegment, double deltaValue) override;

        ///
        /// \brief Переместить точку TCP, найти обобщенные координаты
        /// \param Новое положение TCP
        ///
        bool MoveTCP(const KDL::Frame& newTCP) override;

        ///
        /// \brief SetPosition
        /// \param input
        ///
        void SetPosition(const KDL::JntArray input);

        ///
        /// \brief Возвращает обобщенные координаты
        /// \return Текущие матрицы преобразований манипулятора
        ///
        KDL::JntArray GetPosition() const override;

        ///
        /// \brief Получить текущее положение TCP
        /// \return Gоложение и ориентация точки TCP
        ///
        KDL::Frame GetTCP() const  override;

    private:

        ///
        /// \brief SegmentToKDLSegment
        /// \param count
        /// \param mode
        /// \return
        ///
        KDL::Segment SegmentToKDLSegment(int count, std::shared_ptr<Data::Segment>& ptrSegment);

        ///
        /// \brief Алгоритм решения прямой задачи кинематики
        ///
        ///
        bool ForwardKinematic(KDL::JntArray& newPos, KDL::Frame& curTCP) override;

        ///
        /// \brief Алгоритм решения обратной задачи кинематики
        ///
        bool InverseKinematic(KDL::JntArray& q_inp, KDL::JntArray& q_out,const KDL::Frame& _newTcp) override;

    private:
        KDL::Chain _chain;

        KDL::ChainFkSolverPos_recursive _fksolver;
        KDL::ChainIkSolverVel_pinv      _iksolverv;

        KDL::Frame _curTcp;

        KDL::JntArray  _jointCur;
        KDL::JntArray  _temp;
        KDL::JntArray  _jointInit;
        KDL::JntArray  _jointMin;
        KDL::JntArray  _jointMax;
    };
}}

#endif // KINEMATIC_H
