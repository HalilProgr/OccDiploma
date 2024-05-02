#include "Manipulator.h"

#include <OSD.hxx>
#include <AIS_Trihedron.hxx>
#include <AIS_ViewCube.hxx>
#include <AIS_Selection.hxx>
#include <AIS_ColoredShape.hxx>
#include <AIS_ColoredDrawer.hxx>
#include <BRepLib.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <BRepOffsetAPI_MakeThickSolid.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepTools_ReShape.hxx>
#include <BRepTools.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <gp_Trsf.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopExp.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Compound.hxx>
#include <TPrsStd_AISPresentation.hxx>
#include <STEPControl_Writer.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <STEPCAFControl_Controller.hxx>
#include <Geom_Line.hxx>
#include <Geom_Circle.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_CartesianPoint.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeArcOfEllipse.hxx>
#include <GC_MakeCircle.hxx>
#include <GC_MakeEllipse.hxx>
#include <GC_MakeSegment.hxx>
#include <gce_MakeRotation.hxx>
#include <TopExp.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Edge.hxx>
#include <TDocStd_Document.hxx>
#include <TDocStd_Application.hxx>
#include <TDF_Label.hxx>
#include <TDF_AttributeIterator.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_LabelSequence.hxx>
#include <TDataStd_Name.hxx>
#include <TDataStd_TreeNode.hxx>
#include <TDataStd_UAttribute.hxx>
#include <TNaming_NamedShape.hxx>
#include <TopTools.hxx>
#include <Geom_Plane.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom2d_Ellipse.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include "Geom_Axis2Placement.hxx"
#include <GCE2d_MakeSegment.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <XCAFDoc_Area.hxx>
#include <XCAFDoc_Centroid.hxx>
#include <XCAFDoc_Datum.hxx>
#include <XCAFDoc_Dimension.hxx>
#include <XCAFDoc_Location.hxx>
#include <XCAFDoc_Volume.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <Quantity_Color.hxx>
#include <Quantity_ColorRGBA.hxx>

#include "XCAFPrs_DocumentExplorer.hxx"
#include <TDataStd_Name.hxx>
#include <XCAFDoc_AssemblyItemId.hxx>
#include <XCAFDoc_AssemblyItemRef.hxx>

#include <BRepTools.hxx>
#include <Font_BRepFont.hxx>
#include <Font_BRepTextBuilder.hxx>
#include <Bnd_Box.hxx>

#include <src/common/Tools.h>

#define _USE_MATH_DEFINES
#define toRadians (M_PI/180.0)
#define toDegrees (180.0/M_PI)


namespace Lib::Data
{

using Lib::Tools::FrameToTrsf;

    Manipulator::Manipulator(){}

    Manipulator::DescriptionSegment Manipulator::FindSegment(Handle(AIS_InteractiveObject) obj)
    {
        DescriptionSegment res;

        for (int i = 0; i < _data.size(); i++)
        {
            if (_data[i]->IsPart(obj))
            {
                if (_data[i]->GetMode() == Mode::base)
                    return res;

                res.numberSegment = i -1;
                res.segmentPtr = _data[i];
                return res;
            }
        }

        if (_tool->IsPart(obj))
        {
            res.isTool = true;
            res.toolPtr = _tool;
            return res;
        }

        return res;
    }

    void Manipulator::MoveTCP(gp_Trsf pos)
    {
        KDL::Frame newTCP = Lib::Tools::TrsfToFrame(pos);
        _kinematic.MoveTCP(newTCP);
        KDL::Frame actualTCP = _kinematic.GetTCP();
        _tool->SetTCP(Lib::Tools::FrameToTrsf(actualTCP));
        SetActualPosition();
    }

    void Manipulator::MoveSegment(int numberSegment, double newPosition)
    {
        _kinematic.MoveSegment(numberSegment,newPosition);
        SetActualPosition();
    }

    void Manipulator::SetSegments(std::vector<std::shared_ptr<Segment> > segments)
    {
        _data = segments;
        _kinematic.Init(segments);

        for (auto& seg : _data)
            for (auto& ais : seg->GetAISShapes())
                _view.push_back(ais);

        if (_tool != nullptr) SetActualPosition();
    }

    void Manipulator::SetTool(std::shared_ptr<Tool> tool)
    {
        _tool = tool;
        _view.push_back(_tool->GetAISShape());
        SetActualPosition();
    }

    std::vector<Handle (AIS_InteractiveObject)>& Lib::Data::Manipulator::GetView()
    {
        return _view;
    }

    bool Manipulator::IsEmpty()
    {
        return _data.empty();
    }

    void Manipulator::SetActualPosition()
    {
        gp_Trsf local = _data[0]->GetTransform();

        auto actualPos = _kinematic.GetPosition();
        for(int i = 1; i < _data.size(); i++)
        {
            gp_Trsf temp = _data[i]->GetTransform();
            temp.SetRotation(_data[i]->GetAxis(), actualPos(i - 1));
            local.Multiply(temp);
            _data[i]->SetTransform(local);
        }

        gp_Trsf temp = _tool->GetTransform();
        temp.Multiply(local);
        _tool->SetTransform(temp);
    }

}
