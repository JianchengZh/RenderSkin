#pragma once
#include "SkinViewComp.h"
#include "PlacementRect.h"

class SkinPlacementRect;
class ListItem;
class EditableSkin;
class RenderSkin;

/**
    component to place the skon controls bounds on the background image of the skin
 **/
class RenderSkinEditor :
public SkinViewComp,
public LassoSource<ListItem*>
{
public:
	RenderSkinEditor(EditableSkin* skin);
	~RenderSkinEditor();
    
    juce_UseDebuggingNewOperator
    
	void changeListenerCallback(ChangeBroadcaster* ob)override;
    
	void paint(Graphics& g)override;
    void mouseDown(const MouseEvent& e)override;
    void mouseUp(const MouseEvent& e)override;
    void mouseDrag(const MouseEvent& e)override;
    
    virtual void findLassoItemsInArea (Array <ListItem*>& itemsFound, const Rectangle<int>& area) override;
    bool keyPressed(const KeyPress& k);

    virtual SelectedItemSet<ListItem*>& getLassoSelection() override;
    
    RenderSkin* getApp()const;
    
private:
    
    WeakReference<EditableSkin> skin;
	Colour colour;
    LassoComponent<ListItem*> lasso;
    
	PlacementRect trim;
	OwnedArray<PlacementRect> boundings;
	OwnedArray<PlacementRect> controlls;
    

    
    class Placer : public Component
    {
    public:
        void paint(Graphics&g) override;
    };
    
    Placer placer;
    
};
