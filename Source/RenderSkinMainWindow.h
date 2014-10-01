#pragma once
#include "JuceHeader.h"
#include "RenderSkin.h"
#include "RenderSkinMainComponent.h"
#include "HistorySource.h"

class RenderSkinMainWindow :
public DocumentWindow,
public HistorySource
{
public:
	RenderSkinMainWindow(RenderSkin* app);
    ~RenderSkinMainWindow();
    
    virtual D3CKHistory* getHistory()const;

    void closeButtonPressed()override;
    
private:
	WeakReference<RenderSkin> app;
	RenderSkinMainComponent comp;
};
