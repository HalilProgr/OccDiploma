#ifndef MYAISSHAPE_H
#define MYAISSHAPE_H

#include <AIS_Animation.hxx>
#include <AIS_AnimationObject.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepBndLib.hxx>
#include <Graphic3d_ArrayOfPoints.hxx>
#include <Prs3d_Arrow.hxx>
#include <Prs3d_ArrowAspect.hxx>
#include <Prs3d_BndBox.hxx>
#include <Prs3d_ShadingAspect.hxx>
#include <Prs3d_ToolCylinder.hxx>
#include <Prs3d_ToolDisk.hxx>
#include <Prs3d_PresentationShadow.hxx>
#include <StdPrs_ShadedShape.hxx>
#include <StdPrs_WFShape.hxx>
#include <StdSelect_BRepSelectionTool.hxx>
#include <StdPrs_ToolTriangulatedShape.hxx>
#include <Select3D_SensitiveBox.hxx>
#include <Select3D_SensitivePrimitiveArray.hxx>
#include <V3d_Viewer.hxx>
#include <math_BullardGenerator.hxx>

#include <AIS_ViewController.hxx>
#include <AIS_Animation.hxx>

#include <AIS_Shape.hxx>
#include <Standard_Handle.hxx>

class MyAisShape : public AIS_Shape
{
public:
    MyAisShape(const TopoDS_Shape& shap) : AIS_Shape(shap) {}
    void SetAnimation (const Handle(AIS_Animation)& theAnim) { myAnim = theAnim; }
protected:
    Handle(AIS_Animation) myAnim;
    gp_Pnt myDragPntFrom;
};

#endif // MYAISSHAPE_H
