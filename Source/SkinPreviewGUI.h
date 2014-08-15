#pragma once
#include "SkinGUI.h"

class EditableSkin;

class SkinPreviewGUI :
public SkinGUI
{
public:
    SkinPreviewGUI(EditableSkin* skin);
    ~SkinPreviewGUI();
    
    void paint(Graphics& g)override;
    
    SkinCompGUI* createCompGUI(SkinComp*) override;
    
private:
    WeakReference<EditableSkin> skin;
};