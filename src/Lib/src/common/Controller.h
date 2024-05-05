#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <src/data/Manipulator.h>
#include <src/algorithms/Animation.h>
#include <src/common/DocumentCommon.h>
#include "src/view/OcctQtViewer.h"
#include <src/algorithms/Animation.h>
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
                QPushButton* startAnimation;
            } _interface;

        public:
            Controller(OcctQtViewer* view, DocumentCommon* doc, QWidget* parent = nullptr);

            void SetManipulator(std::shared_ptr<Lib::Data::Manipulator> manip);

        private:
            void InitSignals();
            void InitGUI();

            Handle(AIS_Shape) ComputeSpline(std::vector<gp_Trsf>& points);
            Handle(AIS_Shape) DrawAISWire(std::vector<gp_Pnt>& points);

        private slots:
            void TryUpdateTCP();
            void AddPoint();
            void RemovePoint();
            void StartAnimation();

        private:
            Animation _animation;
            std::shared_ptr<Lib::Data::Manipulator> _manipulater;
            std::vector<gp_Trsf> _points;
            std::vector<gp_Pnt> _pointsWire;
            DocumentCommon* _document;
            OcctQtViewer* _view;
            Handle(AIS_Shape) _wire;
        };

}}

#endif // CONTROLLER_H
