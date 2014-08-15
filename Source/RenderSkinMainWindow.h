#pragma once
#include "JuceHeader.h"
#include "RenderSkin.h"
#include "RenderSkinMainComponent.h"

class RenderSkinMainWindow :
public DocumentWindow
{
public:
	RenderSkinMainWindow();
    ~RenderSkinMainWindow();
    
    void closeButtonPressed()override;
    
private:
	RenderSkin skin;
	RenderSkinMainComponent comp;
};
