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
        Manipulator(std::vector<std::shared_ptr<Segment>> segments);

        std::vector<std::shared_ptr<Segment>>& GetSegmnets() {return _data;}


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
        void SetSegments(std::vector<std::shared_ptr<Segment>> segments);
        void SetTool(std::shared_ptr<Tool> tool);

        gp_Trsf GetTCP();
        std::vector<Handle(AIS_InteractiveObject)>& GetView();

        ///
        /// \brief IsEmpty
        /// \return
        ///
        bool IsEmpty();

    private:
        void SetActualPosition();

        bool _displaed;
        bool _empty;

        Kinematic::kinematic _kinematic; // кинематика робота.
        std::vector<std::shared_ptr<Segment>> _data; // думаю лучше хранить сегменты в shared_ptr
        std::shared_ptr<Tool> _tool;
         // удобное отображение элементов в OCC формате, необходимо для отображения элементов в Document
        std::vector<Handle(AIS_InteractiveObject)> _view;
    };

}}

#endif // MANIPULATOR_H
