#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <src/data/Manipulator.h>
#include <src/common/Animation.h>
#include <src/common/DocumentCommon.h>
#include <AIS_Shape.hxx>


namespace Lib
{
    namespace Common
    {

        class Controller : public QWidget
        {
            Q_OBJECT

            struct Interface
            {
                QLineEdit* x;
                QLineEdit* y;
                QLineEdit* z;

                QPushButton* changeTCP;
                QPushButton* addPoint;
                QPushButton* removeLastPoint;
            } interface;

        public:
            Controller(DocumentCommon* doc, QWidget* parent = nullptr);

            void Init();
            void SetManipulator(std::shared_ptr<Lib::Data::Manipulator> manip);

        public slots:
            void TryUpdateTCP();
            void AddPoint();
            void RemovePoint();

        private:
            Handle(AIS_Shape) DrawSpline(std::vector<gp_Pnt>& points);
            Handle(AIS_Shape) DrawAISWire(std::vector<gp_Pnt>& points);

            std::shared_ptr<Lib::Data::Manipulator> _manipulater;
            std::vector<gp_Pnt> _points;
            DocumentCommon* _document;

            Handle(AIS_Shape) _wire;
        };
}}

#endif // CONTROLLER_H
