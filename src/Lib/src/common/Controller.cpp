#include "controller.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <gp_Trsf.hxx>
#include <Eigen3/include/eigen3/Eigen/Dense>
#include <src/algorithms/CubicSpline.h>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>

#include <QDebug>


namespace Lib
{
    namespace Common
    {

Controller::Controller(DocumentCommon* doc, QWidget* parent) : QWidget(parent), _document(doc)
        {
            QHBoxLayout* lines = new QHBoxLayout;
            interface.x = new QLineEdit("X value", this);
            interface.y = new QLineEdit("y value", this);
            interface.z = new QLineEdit("z value", this);

            lines->addWidget(interface.x);
            lines->addWidget(interface.y);
            lines->addWidget(interface.z);

            QVBoxLayout* mainLayout = new QVBoxLayout;

            interface.changeTCP = new QPushButton("ChangeTCP", this);
            interface.addPoint = new QPushButton("Add Point", this);
            interface.removeLastPoint = new QPushButton("Remove Last Point", this);

            mainLayout->addLayout(lines);
            mainLayout->addWidget(interface.changeTCP);
            mainLayout->addWidget(interface.addPoint);
            mainLayout->addWidget(interface.removeLastPoint);

            setLayout(mainLayout);

            Init();
        }

        void Controller::Init()
        {
            connect(interface.changeTCP, &QPushButton::clicked, this, &Controller::TryUpdateTCP);
            connect(interface.addPoint, &QPushButton::clicked, this, &Controller::AddPoint);
            connect(interface.removeLastPoint, &QPushButton::clicked, this, &Controller::RemovePoint);
        }

        void Controller::SetManipulator(std::shared_ptr<Data::Manipulator> manipPtr)
        {
            _manipulater = manipPtr;
        }

        void Controller::TryUpdateTCP()
        {
            if (_manipulater == nullptr) return;

            gp_Trsf TCP = _manipulater->GetTCP();
            gp_XYZ xyz = TCP.TranslationPart();
            gp_Vec part(xyz.X() + interface.x->text().toDouble(),
                        xyz.Y() + interface.y->text().toDouble(),
                        xyz.Z() + interface.z->text().toDouble());
            TCP.SetTranslation(part);

            _manipulater->MoveTCP(TCP);
        }

        void Controller::AddPoint()
        {
            gp_XYZ xyz = _manipulater->GetTCP().TranslationPart();
            _points.push_back(gp_Pnt(xyz.X(), xyz.Y(), xyz.Z()));

            if ( _points.size() < 3) return;

            if (!_wire.IsNull())
                _document->GetContext()->Erase(_wire,true);

            _wire = DrawSpline(_points);
            _document->GetContext()->Display(_wire, 0);
        }

        void Controller::RemovePoint()
        {
            if (!_wire.IsNull())
                _document->GetContext()->Erase(_wire,true);

            if (_points.empty()) return;

            _points.pop_back();

            if ( _points.size() < 3) return;

            _wire = DrawSpline(_points);
            _document->GetContext()->Display(_wire, 0);
        }

        Handle(AIS_Shape) Controller::DrawSpline(std::vector<gp_Pnt> &points)
        {
            std::vector<Eigen::Vector3d> path; // libspline input
            for(unsigned int i = 0; i < points.size(); i++)
                path.push_back( Vector3d(points[i].X(),
                                         points[i].Y(),
                                         points[i].Z()));

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
                c_pathx[i] = c_spline.GetPositionProfile()[i].x();
                c_pathy[i] = c_spline.GetPositionProfile()[i].y();
                c_pathz[i] = c_spline.GetPositionProfile()[i].z();

                p = {c_pathx[i],c_pathy[i],c_pathz[i]};
                pvec.push_back(p);
            }

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
