#pragma once
#include "JuceHeader.h"

class SkinGUI;

class RenderSkinPreviewWindow :
public DialogWindow
{
public:
    RenderSkinPreviewWindow(SkinGUI* gui,ScopedPointer<RenderSkinPreviewWindow>& pointer);
    void closeButtonPressed()override;
    
private:
    SafePointer<SkinGUI> gui;
    ScopedPointer<RenderSkinPreviewWindow>* pointer;
};