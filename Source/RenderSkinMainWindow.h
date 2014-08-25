#pragma once
#include "JuceHeader.h"
#include "RenderSkin.h"
#include "RenderSkinMainComponent.h"

class RenderSkinMainWindow :
public DocumentWindow
{
public:
	RenderSkinMainWindow(RenderSkin* app);
    ~RenderSkinMainWindow();
    
    void closeButtonPressed()override;
    
private:
	WeakReference<RenderSkin> app;
	RenderSkinMainComponent comp;
};
