#include "controller.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <gp_Trsf.hxx>
#include <Eigen3/include/eigen3/Eigen/Dense>
#include <src/algorithms/CubicSpline.h>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include "src/common/Tools.h"
#include <QDebug>


namespace Lib
{
    namespace Common
    {

Controller::Controller(OcctQtViewer* view, DocumentCommon* doc, QWidget* parent) : QWidget(parent), _document(doc), _view(view)
        {
            InitGUI();
            InitSignals();
        }

        void Lib::Common::Controller::InitGUI()
        {
            QHBoxLayout* lines = new QHBoxLayout;
            _interface.x = new QLineEdit("X value", this);
            _interface.y = new QLineEdit("y value", this);
            _interface.z = new QLineEdit("z value", this);

            lines->addWidget(_interface.x);
            lines->addWidget(_interface.y);
            lines->addWidget(_interface.z);

            QVBoxLayout* mainLayout = new QVBoxLayout;

            _interface.changeTCP = new QPushButton("ChangeTCP", this);
            _interface.addPoint = new QPushButton("Add Point", this);
            _interface.removeLastPoint = new QPushButton("Remove Last Point", this);
            _interface.startAnimation = new QPushButton("Start Animation", this);

            mainLayout->addLayout(lines);
            mainLayout->addWidget(_interface.changeTCP);
            mainLayout->addWidget(_interface.addPoint);
            mainLayout->addWidget(_interface.removeLastPoint);
            mainLayout->addWidget(_interface.startAnimation);

            setLayout(mainLayout);
        }

        void Controller::InitSignals()
        {
            connect(_interface.changeTCP, &QPushButton::clicked, this, &Controller::TryUpdateTCP);
            connect(_interface.addPoint, &QPushButton::clicked, this, &Controller::AddPoint);
            connect(_interface.removeLastPoint, &QPushButton::clicked, this, &Controller::RemovePoint);
            connect(_interface.startAnimation, &QPushButton::clicked, this, &Controller::StartAnimation);
        }

        void Controller::SetManipulator(std::shared_ptr<Data::Manipulator> manipPtr)
        {
            _manipulater = manipPtr;
            _animation.SetManipulator(_manipulater);
        }

        void Controller::TryUpdateTCP()
        {
            if (_manipulater == nullptr) return;

            gp_Trsf TCP = _manipulater->GetTCP();
            gp_XYZ xyz = TCP.TranslationPart();
            gp_Vec part(xyz.X() + _interface.x->text().toDouble(),
                        xyz.Y() + _interface.y->text().toDouble(),
                        xyz.Z() + _interface.z->text().toDouble());
            TCP.SetTranslation(part);

            _manipulater->MoveTCP(TCP);
        }

        void Controller::AddPoint()
        {
            _points.push_back(_manipulater->GetTCP());

            if ( _points.size() < 3) return;

            if (!_wire.IsNull())
                _document->GetContext()->Erase(_wire,true);

            _wire = ComputeSpline(_points);
            _document->GetContext()->Display(_wire, 0);
        }

        void Controller::RemovePoint()
        {
            if (!_wire.IsNull())
                _document->GetContext()->Erase(_wire,true);

            if (_points.empty()) return;

            _points.pop_back();

            if ( _points.size() < 3) return;

            _wire = ComputeSpline(_points);
            _document->GetContext()->Display(_wire, 0);
        }

        void Controller::StartAnimation()
        {
            _animation = Animation(_document,_manipulater);

            qDebug() << QString('_', 10);
            qDebug() << "Start animation";
            if (_pointsWire.empty()) return;

            /// Это надо отсюда убрать !
            std::vector<gp_Trsf> temp;
            gp_Quaternion angle;

            /// Это шляпа тоже.
            for (auto & point : _pointsWire)
                temp.push_back(Lib::Tools::MakeTrsf(point, angle));

            _animation.SetPoints(temp);
            if (!_animation.Compute())
            {
                qDebug() << "Error compute animation";
                return;
            }

            auto anim = _view->ObjectsAnimation();
            anim->Add(_animation.GetAnimation());

            anim->StartTimer(0, 1.0, true);
        }

        Handle(AIS_Shape) Controller::ComputeSpline(std::vector<gp_Trsf>& points)
        {
            std::vector<Eigen::Vector3d> path; // libspline input
            for(unsigned int i = 0; i < points.size(); i++)
                path.push_back(Lib::Tools::TrsfToVector3D(points[i]));

            CubicSpline c_spline(path);
            c_spline.BuildSpline(path);
            std::vector<double> c_pathx(c_spline.GetPositionProfile().size());
            std::vector<double> c_pathy(c_spline.GetPositionProfile().size());
            std::vector<double> c_pathz(c_spline.GetPositionProfile().size());

            // Store wire points(p) into pointvector (pvec)
            gp_Pnt p;
            std::vector<gp_Pnt> pvec;
            // Get profile data for position, speed, acceleration, and curvature
            std::vector<double> ti(c_spline.GetPositionProfile().size());

            for(unsigned int i = 0; i < c_pathx.size(); ++i)
            {
                c_pathx[i] = c_spline.GetPositionProfile()[i].x(); // < это плохо написано, можно короче и красиивее.
                c_pathy[i] = c_spline.GetPositionProfile()[i].y();
                c_pathz[i] = c_spline.GetPositionProfile()[i].z();

                p = {c_pathx[i],c_pathy[i],c_pathz[i]};
                pvec.push_back(p);
            }
            _pointsWire = pvec;
            qDebug() << "Spline size:" << pvec.size();
            qDebug() << " Spline:";
            for (auto& el : pvec)
                qDebug() << el.X() << " " << el.Y() << " " << el.Z();

            Handle(AIS_Shape) spline = DrawAISWire(pvec);
            return spline;
        }

        Handle(AIS_Shape) Controller::DrawAISWire(std::vector<gp_Pnt>& points){
            BRepBuilderAPI_MakeWire wire;
            for(unsigned int i = 0; i < points.size() - 1; i++)
            {
                TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(points[i], points[i+1]);
                wire.Add(edge);
            }
            return new AIS_Shape(wire);
        }
}}
