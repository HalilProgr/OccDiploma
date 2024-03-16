#ifndef KINEMATIC_H
#define KINEMATIC_H

#include "ikinematic.h"
#include "kdl/chain.hpp"
#include "kdl/chainfksolverpos_recursive.hpp"
#include "kdl/chainiksolvervel_pinv.hpp"
#include "kdl/ChainIkSolverPos_NR_JL.hpp"

namespace App
{
    namespace Kinematic
    {

    class kinematic : public IKinematic
    {
    public:
        kinematic();
        ~kinematic() = default;

        ///
        /// \brief Временная инициализация (MOC, далее будет удавлено)
        ///
        void init();

        ///
        /// \brief Переместить звено
        /// \param Номер звена
        /// \param новый угол
        ///
        void MoveSegment(int numberSegment, double newValue) override;

        ///
        /// \brief Переместить точку TCP, найти обобщенные координаты
        /// \param Новое положение TCP
        ///
        void MoveTCP(KDL::Frame& newTCP) override;

        ///
        /// \brief Возвращает обобщенные координаты
        /// \return Текущие матрицы преобразований манипулятора
        ///
        KDL::JntArray& GetPosition() override
        {
            return _jointCur;
        }

        ///
        /// \brief Получить текущее положение TCP
        /// \return Gоложение и ориентация точки TCP
        ///
        KDL::Frame& GetTCP() override
        {
            return _curTcp;
        }

    private:
        ///
        /// \brief Алгоритм решения прямой задачи кинематики
        ///
        ///
        bool ForwardKinematic(KDL::JntArray& newPos, KDL::Frame& curTCP) override;

        ///
        /// \brief Алгоритм решения обратной задачи кинематики
        ///
        bool InverseKinematic(KDL::JntArray& q_inp, KDL::JntArray& q_out, KDL::Frame& _newTcp) override;

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
