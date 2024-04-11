#ifndef ROBOTMANIPULATOR_H
#define ROBOTMANIPULATOR_H

#include <AIS_Manipulator.hxx>
#include <AIS_ManipulatorMode.hxx>
#include "src/data/CollectionShapes.h"
#include "src/data/Segment.h"
#include "src/data/CommonType.h"

///
/// Какой же я крутой! Я понял как работает эта фигня!!! УРА УРА УРА УРА УРА УРА УРА!
/// Женщины встньте в ряд! Я Вас жду!
///
namespace App
{
    namespace Common
    {

        ///
        /// \brief The RobotManipulator class
        ///
        class RobotManipulator : public AIS_Manipulator
        {
        public:

            RobotManipulator();
            RobotManipulator(const gp_Ax2& thePosition);

            gp_Trsf Transform (const Standard_Integer thePX, const Standard_Integer thePY,
                               const Handle(V3d_View)& theView);

            virtual Standard_Boolean ProcessDragging (const Handle(AIS_InteractiveContext)& theCtx,
                                                      const Handle(V3d_View)& theView,
                                                      const Handle(SelectMgr_EntityOwner)& theOwner,
                                                      const Graphic3d_Vec2i& theDragFrom,
                                                      const Graphic3d_Vec2i& theDragTo,
                                                      const AIS_DragAction theAction) override;

            ///
            /// \brief Основной метод
            /// \param shapes Фигуры для инициализации сегмента
            /// \param crsys система координат звена
            /// \param mode режим звена
            ///
            void Attach(std::shared_ptr<App::Data::CollectionShapes>& collection,
                        std::shared_ptr<App::Data::Segment>& segment,
                        Handle(AIS_InteractiveObject)& objToCon);

            ///
            /// \brief Detach
            ///
            void Detach();

        private:
            void SetMode(App::Data::Mode mode);

            std::shared_ptr<App::Data::CollectionShapes> _actualCollection;
            App::Data::Mode _mode;
            int _numberSegment;
        };

}}

#endif // ROBOTMANIPULATOR_H
