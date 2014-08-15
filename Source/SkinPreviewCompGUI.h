#pragma once
#include "SkinCompGUI.h"

class EditableSkinComp;

class SkinPreviewCompGUI :
public SkinCompGUI
{
    public:
    SkinPreviewCompGUI(EditableSkinComp*,SkinGUI*);
    ~SkinPreviewCompGUI();
    
    void paint(Graphics&g)override;
    
private:
    WeakReference<EditableSkinComp> comp;
    
};