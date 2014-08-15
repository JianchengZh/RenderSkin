#pragma once 
#include "SkinPlacementRect.h"

class SkinCompControllPlacementRect:
public SkinPlacementRect
{
public:
    SkinCompControllPlacementRect(EditableSkinComp* comp , BoundsDriver* rect, RenderSkinEditor* editor );

    void changeListenerCallback(ChangeBroadcaster*obj) override;
    void paint(Graphics& g)override;
};