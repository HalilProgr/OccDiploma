#ifndef IKINEMATIC_H
#define IKINEMATIC_H

#define _USE_MATH_DEFINES
#define toRadians M_PI/180.0
#define toDegrees (180.0/M_PI)

#include <cmath>
#include <kdl/segment.hpp>
#include <kdl/frameacc.hpp>
#include <kdl/jntarray.hpp>

namespace App
{
    namespace Kinematic
    {

    ///
    /// \brief Интерфейсный класс описания кинематики робота.
    ///
    class IKinematic
    {
    public:
        IKinematic() = default;
        virtual ~IKinematic() = default;

        ///
        /// \brief Переместить звено
        /// \param Номер звена
        /// \param newValue
        ///
        virtual void MoveSegment(int numberSegment, double newValue) = 0;

        ///
        /// \brief Переместить точку TCP, найти обобщенные координаты
        /// \param Новое положение TCP
        ///
        virtual void MoveTCP(KDL::Frame& newTCP) = 0;

        ///
        /// \brief Возвращает обобщенные координаты
        /// \return Текущие обобщенные координаты
        ///
        virtual KDL::JntArray& GetPosition() = 0;

        ///
        /// \brief Получиьт текущее положение TCP
        /// \return Gоложение и ориентация точки TCP
        ///
        virtual KDL::Frame& GetTCP() = 0;

    private:
        ///
        /// \brief Алгоритм решения прямой задачи кинематики
        /// \return Статус решения
        ///
        virtual bool ForwardKinematic(KDL::JntArray& newPos, KDL::Frame& curTCP) = 0;

        ///
        /// \brief Алгоритм решения обратной задачи кинематики
        /// \return Статус решения
        ///
        virtual bool InverseKinematic(KDL::JntArray& q_inp, KDL::JntArray& q_out, KDL::Frame& _newTcp) = 0;
    };
}}

#endif // IKINEMATIC_H
