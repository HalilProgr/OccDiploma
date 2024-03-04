#include "inc/myaisowner.h"

#include "inc/myaisshape.h"

bool MyAisOwner::HandleMouseClick (const Graphic3d_Vec2i& thePoint,
                                  Aspect_VKeyMouse theButton,
                                  Aspect_VKeyFlags theModifiers,
                                  bool theIsDoubleClick)
{
    static bool isFirst = true;
    isFirst = !isFirst;
    MyAisShape* anObj = dynamic_cast<MyAisShape*> (mySelectable);
    gp_Trsf aTrsfTo;
    aTrsfTo.SetRotation (gp_Ax1 (gp::Origin(), gp::DX()),
                        isFirst ? M_PI * 0.5 : -M_PI * 0.5);
    gp_Trsf aTrsfFrom = anObj->LocalTransformation();
    Handle(AIS_AnimationObject) anAnim =
        new AIS_AnimationObject ("MyAnim", anObj->InteractiveContext(),
                                anObj, aTrsfFrom, aTrsfTo);
    anAnim->SetOwnDuration (2.0);

    myAnim->Clear();
    myAnim->Add (anAnim);
    myAnim->StartTimer (0.0, 1.0, true);
    return true;
}
