#include "SkinCompControllPlacementRect.h"
#include "EditableSkinComp.h"

SkinCompControllPlacementRect::SkinCompControllPlacementRect(EditableSkinComp* comp , BoundsDriver* rect, RenderSkinEditor* editor ):
SkinPlacementRect(comp,rect,editor)
{
    
}

void SkinCompControllPlacementRect::changeListenerCallback(ChangeBroadcaster*obj)
{
	const bool useControll = comp->useControllArea.getValue();
	setVisible(useControll);
    
    SkinPlacementRect::changeListenerCallback(obj);
}

void SkinCompControllPlacementRect::paint(Graphics& g)
{
    PlacementRect::paint(g,this);
    
}
