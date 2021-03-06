#pragma once
#include "PlacementRect.h"

class EditableSkinComp;
class RenderSkinEditor;

class SkinPlacementRect :
public PlacementRect
{
public:
	SkinPlacementRect(EditableSkinComp* comp , BoundsDriver* rect, RenderSkinEditor* editor );
    ~SkinPlacementRect();
    
    juce_UseDebuggingNewOperator
    
	void mouseUp(const MouseEvent& e) override;
    void mouseDown(const MouseEvent& e) override;
    
    virtual void changeListenerCallback(ChangeBroadcaster*obj) override;
    
    bool keyPressed(const KeyPress& k) override;
    
    void paint(Graphics& g)override;
    
    const Colour& getColour() const;
    bool isSelected() const;
    
protected:
    
    bool selectionBool;
	WeakReference<EditableSkinComp> comp;
    SafePointer<RenderSkinEditor> editor;
};