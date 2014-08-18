#pragma once
#include "JuceHeader.h"
#include "RenderSkinEditor.h"
#include "SkinCompPropsGUI.h"
#include "SkinProps.h"

class SkinComp;
class RenderSkinPreviewWindow;
class RenderSkinMainGUI:
public Component

{
public:
	RenderSkinMainGUI(EditableSkin* skin);
	~RenderSkinMainGUI();
    
    juce_UseDebuggingNewOperator
    
	void resized()override;
    
private:
    SkinCompPropsGUI properties;
    SkinProps skinProperties;
    
	RenderSkinEditor editor;
	WeakReference<EditableSkin> skin;
};
