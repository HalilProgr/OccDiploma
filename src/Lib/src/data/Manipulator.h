#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include "Segment.h"
#include "tool.h"
#include "../algorithms/kinematic.h"


namespace Lib
{
    namespace Data
    {

    ///
    /// \brief Manipulator класс реализующий связь между AIS моделями и математичкской моделью.
    /// Является контейниром AIS_Shape и их положения. Адаптер между кинемтаикой и отображением.
    ///
    class Manipulator
    {
    public:

        struct DescriptionSegment
        {
            bool isTool = false;
            std::shared_ptr<Segment> segmentPtr = nullptr;
            std::shared_ptr<Tool> toolPtr = nullptr;
            int numberSegment = -1;
        };

        Manipulator();
        Manipulator(const std::vector<std::shared_ptr<Segment>> segments, const std::shared_ptr<Tool> tool);

        std::vector<std::shared_ptr<Segment>> GetSegmnets() const;
        std::shared_ptr<Tool> GetTool() const;


        DescriptionSegment FindSegment(Handle(AIS_InteractiveObject) obj);

        ///
        /// \brief SetPosition
        /// \param pos
        ///
        void MoveTCP(gp_Trsf pos);
        void MoveSegment(int numberSegment, double delta);

        ///
        /// \brief SetSegments
        /// \param segments
        ///
        void SetSegments(const std::vector<std::shared_ptr<Segment>> segments);
        void SetTool(const std::shared_ptr<Tool> tool);

        gp_Trsf GetTCP() const;
        std::vector<Handle(AIS_InteractiveObject)> GetView() const;
        std::shared_ptr<Lib::Kinematic::IKinematic> GetKinematicModel() const;

        ///
        /// \brief IsEmpty
        /// \return
        ///
        bool IsEmpty() const;

    private:
        void SetActualPosition();

        bool _displaed;
        bool _empty;

        std::shared_ptr<Kinematic::kinematic> _modelKinematic; // кинематика робота.
        std::vector<std::shared_ptr<Segment>> _data; // думаю лучше хранить сегменты в shared_ptr
        std::shared_ptr<Tool> _tool;
         // удобное отображение элементов в OCC формате, необходимо для отображения элементов в Document
        std::vector<Handle(AIS_InteractiveObject)> _view;
    };

}}

#endif // MANIPULATOR_H
