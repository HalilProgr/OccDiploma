#ifndef MYAISOWNER_H
#define MYAISOWNER_H

#include <QADraw.hxx>

#include <ViewerTest.hxx>
#include <ViewerTest_EventManager.hxx>

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

#include "AIS_Shape.hxx"

#include <SelectMgr_EntityOwner.hxx>

class MyAisOwner : public SelectMgr_EntityOwner
{
    DEFINE_STANDARD_RTTI_INLINE(MyAisOwner, SelectMgr_EntityOwner)
public:
    MyAisOwner (const Handle(AIS_Shape)& theObj, int thePriority = 0)
        : SelectMgr_EntityOwner (theObj, thePriority) {}

    virtual void HilightWithColor (const Handle(PrsMgr_PresentationManager)& thePrsMgr,
                                  const Handle(Prs3d_Drawer)& theStyle,
                                  const Standard_Integer theMode) override
    { base_type::HilightWithColor (thePrsMgr, theStyle, theMode); }

    virtual void Unhilight (const Handle(PrsMgr_PresentationManager)& thePrsMgr,
                           const Standard_Integer theMode) override
    { base_type::Unhilight  (thePrsMgr, theMode); }

    void SetAnimation (const Handle(AIS_Animation)& theAnim)
    { myAnim = theAnim; }

    virtual bool HandleMouseClick (const Graphic3d_Vec2i& thePoint,
                                      Aspect_VKeyMouse theButton,
                                      Aspect_VKeyFlags theModifiers,
                                      bool theIsDoubleClick);

protected:
    Handle(Prs3d_Presentation) myPrs;
    Handle(AIS_Animation) myAnim;
};

#endif // MYAISOWNER_H
