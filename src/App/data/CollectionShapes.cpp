#include "CollectionShapes.h"

#include <OSD.hxx>
#include <AIS_Trihedron.hxx>
#include <AIS_ViewCube.hxx>
#include <AIS_Selection.hxx>
#include <AIS_ColoredShape.hxx>
#include <AIS_ColoredDrawer.hxx>
#include <BRepLib.hxx>
//#include <BRepOffsetAPI_ThruSections.hxx>
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

#include <common/Tools.h>


namespace App::Data
{

using App::Tools::FrameToTrsf;

CollectionShapes::CollectionShapes()
{}

void CollectionShapes::Init()
{
    _kinematic.init();

    auto AIS_shapes = ReadFile("C:/Qtprogect/OccDiploma/robot/kuka_base.step");

    gp_Pnt x0;
    std::shared_ptr<Segment> basa = std::make_shared<Segment>(AIS_shapes,x0,Mode::base);

    _data.push_back(basa);
    for (auto& shap : AIS_shapes)
        _view.Append(shap);

    AIS_shapes = ReadFile("C:/Qtprogect/OccDiploma/robot/kuka_joint_1.step");

    std::shared_ptr<Segment> basa1 = std::make_shared<Segment>(AIS_shapes,gp_Pnt(0,0,0),Mode::RotZ, 0);


    _data.push_back(basa1);
    for (auto& shap : AIS_shapes)
        _view.Append(shap);


    AIS_shapes = ReadFile("C:/Qtprogect/OccDiploma/robot/kuka_joint_2.step");

    std::shared_ptr<Segment> basa2 = std::make_shared<Segment>(AIS_shapes,gp_Pnt(25,0,400),Mode::RotY, 1);

    _data.push_back(basa2);
    for (auto& shap : AIS_shapes)
        _view.Append(shap);

    AIS_shapes = ReadFile("C:/Qtprogect/OccDiploma/robot/kuka_joint_3.step");

    std::shared_ptr<Segment> basa3 = std::make_shared<Segment>(AIS_shapes,gp_Pnt(25,0,855),Mode::RotY, 2);

    _data.push_back(basa3);
    for (auto& shap : AIS_shapes)
        _view.Append(shap);

    AIS_shapes = ReadFile("C:/Qtprogect/OccDiploma/robot/kuka_joint_4.step");

    std::shared_ptr<Segment> basa4 = std::make_shared<Segment>(AIS_shapes,gp_Pnt(25, 0, 890),Mode::RotX, 3);

    _data.push_back(basa4);
    for (auto& shap : AIS_shapes)
        _view.Append(shap);

    AIS_shapes = ReadFile("C:/Qtprogect/OccDiploma/robot/kuka_joint_5.step");

    std::shared_ptr<Segment> basa5 = std::make_shared<Segment>(AIS_shapes,gp_Pnt(445,0,890),Mode::RotY, 4);

    _data.push_back(basa5);
    for (auto& shap : AIS_shapes)
        _view.Append(shap);

    AIS_shapes = ReadFile("C:/Qtprogect/OccDiploma/robot/kuka_joint_6.step");

    std::shared_ptr<Segment> basa6 = std::make_shared<Segment>(AIS_shapes,gp_Pnt(525,0,890),Mode::tool, 5);

    _data.push_back(basa6);
    for (auto& shap : AIS_shapes)
        _view.Append(shap);

    SetActualPosition();
}

std::vector<Handle (AIS_InteractiveObject)> CollectionShapes::ReadFile(const std::string& theStepName)
    {
        std::vector<Handle (AIS_InteractiveObject)> res;

        STEPCAFControl_Controller::Init();

        Handle(TDocStd_Document) aDoc;
        Handle(XCAFApp_Application) anApp = XCAFApp_Application::GetApplication();
        anApp->NewDocument("MDTV-XCAF", aDoc);

        STEPCAFControl_Reader aStepReader;
        aStepReader.SetColorMode(true);
        aStepReader.SetNameMode(true);
        aStepReader.SetLayerMode(true);
        aStepReader.SetPropsMode(true);

        aStepReader.ReadFile (theStepName.c_str());
        aStepReader.Transfer (aDoc);

        TDF_Label aRootLabel = aDoc->Main();

        aShapeTool = XCAFDoc_DocumentTool::ShapeTool(aRootLabel);
        aColorTool = XCAFDoc_DocumentTool::ColorTool(aRootLabel);

        Visit(aRootLabel, res);

        return res;
    }

    void CollectionShapes::Visit(const TDF_Label& theLabel, std::vector<Handle (AIS_InteractiveObject)>& outVec)
    {
        Quantity_Color aColor;
        TopoDS_Shape aShape;

        bool skip=0;
        if (aShapeTool->IsShape(theLabel))
        {
            //TopoDS_Shape aShape;
            //if a shape can be made from the current label, look for the color of that label:
            if(aShapeTool->GetShape(theLabel, aShape)){
                //std::cout<<"made a shape"<<std::endl;

                if(aColorTool->GetColor(aShape,XCAFDoc_ColorSurf,aColor)){
                    skip=0;
                } else { skip=1;}

            }

            if(skip==0)
            {
                outVec.push_back(new AIS_Shape(aShape));
                outVec.back()->SetColor(aColor);
                outVec.back()->SetDisplayMode(AIS_Shaded);
                outVec.back()->Attributes()->SetFaceBoundaryDraw(true);
                outVec.back()->Attributes()->SetFaceBoundaryAspect(
                    new Prs3d_LineAspect(Quantity_NOC_BLACK, Aspect_TOL_SOLID, 1.));
                outVec.back()->Attributes()->SetIsoOnTriangulation(true);
            }
        }

        for (TDF_ChildIterator c(theLabel); c.More(); c.Next())
            Visit(c.Value(), outVec);
    }

    std::shared_ptr<Segment> CollectionShapes::GetSegment(Handle(AIS_InteractiveObject) obj)
    {
        for (auto& seg : _data)
        {
            if (seg->IsPart(obj))
                return seg;
        }

        return std::shared_ptr<Segment>();
    }

    void CollectionShapes::MoveTCP(gp_Trsf pos)
    {
        KDL::Frame newTCP = App::Tools::TrsfToFrame(pos);
        _kinematic.MoveTCP(newTCP);
        SetActualPosition();
    }

    void CollectionShapes::MoveSegment(int numberSegment, double newPosition)
    {
        _kinematic.MoveSegment(numberSegment,newPosition);
        SetActualPosition();
    }

    bool CollectionShapes::IsEmpty()
    {
        return _data.empty();
    }

    void CollectionShapes::SetActualPosition()
    {
        gp_Trsf local = _basePosition;

        _data[0]->SetTransform(local);

        auto actualPos = _kinematic.GetPosition();
        for(int i = 1; i < _data.size(); i++)
        {
            gp_Trsf temp;
            temp.SetRotation(_data[i]->GetAxis(), actualPos(i - 1) * toDegrees);
            local.Multiply(temp);
            _data[i]->SetTransform(local);
        }
    }

}
