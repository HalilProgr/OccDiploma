#ifndef IKINEMATIC_H
#define IKINEMATIC_H

#include <cmath>
#include <kdl/segment.hpp>
#include <kdl/frameacc.hpp>
#include <kdl/jntarray.hpp>
#include "src/data/Segment.h"


namespace Lib
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
        /// \brief Init
        /// \param segments
        ///
        virtual void Init(std::vector<std::shared_ptr<Data::Segment>>& segments) = 0;

        ///
        /// \brief Переместить звено
        /// \param Номер звена
        /// \param newValue
        ///
        virtual void MoveSegment(int numberSegment, double deltaValue) = 0;

        ///
        /// \brief Переместить точку TCP, найти обобщенные координаты
        /// \param Новое положение TCP
        ///
        virtual bool MoveTCP(const KDL::Frame& newTCP) = 0;

        ///
        /// \brief SetPosition
        /// \param input
        ///
        virtual void SetPosition(const KDL::JntArray input) = 0;

        ///
        /// \brief Возвращает обобщенные координаты
        /// \return Текущие обобщенные координаты
        ///
        virtual KDL::JntArray GetPosition() const = 0;

        ///
        /// \brief Получиьт текущее положение TCP
        /// \return Gоложение и ориентация точки TCP
        ///
        virtual KDL::Frame GetTCP() const = 0;

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
        virtual bool InverseKinematic(KDL::JntArray& q_inp, KDL::JntArray& q_out, const KDL::Frame& _newTcp) = 0;
    };
}}

#endif // IKINEMATIC_H
