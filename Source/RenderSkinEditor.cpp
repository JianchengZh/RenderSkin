#include "RenderSkinEditor.h"
#include "EditableSkin.h"
#include "SkinPlacementRect.h"
#include "EditableSkinComp.h"
#include "SkinCompControllPlacementRect.h"
#include "D3CKHistory.h"

RenderSkinEditor::RenderSkinEditor(EditableSkin* skin):
SkinViewComp(skin),
trim(&skin->graphicArea)
{
    this->skin = skin;
	colour = Colours::red;
    skin->getComps().addChangeListener(this);
	skin->addChangeListener(this);
	changeListenerCallback(skin);
	addAndMakeVisible(&trim);
	trim.setName("trim");
	trim.colour = colour;
}

RenderSkinEditor::~RenderSkinEditor()
{
    if(skin)
    {
        skin->getComps().removeChangeListener(this);
        skin->removeChangeListener(this);
    }
}

RenderSkin* RenderSkinEditor::getApp()const
{
    return this->skin->getApp();
}

bool RenderSkinEditor::keyPressed(const KeyPress& k)
{
    if(k.isKeyCode(KeyPress::deleteKey) || k.isKeyCode(KeyPress::backspaceKey))
    {
        for(int i =  skin->getComps().selectedItems.getNumSelected() ; -- i >= 0 ;)
        {
            ListItem* comp = skin->getComps().selectedItems.getSelectedItem(i);
            comp->moveToTrash(true,D3CKHistory::beginNewTransaction("remove comps", this));
        }
        return true;
    }
    return false;
}


void RenderSkinEditor::findLassoItemsInArea (Array <ListItem*>& itemsFound, const Rectangle<int>& area)
{
    for(int i = this->boundings.size() ; -- i >= 0 ; )
    {
        PlacementRect* d = this->boundings.getUnchecked(i);
        if( area.intersects(d->getBounds()))
        {
            for(int j = skin->getComps().getNumRows() ; -- j >= 0  ; )
            {
                SkinComp* c = dynamic_cast<SkinComp*>(skin->getComps().getItem(j));
                if(&c->graphicArea == d->getDriver())
                {
                    itemsFound.add(c) ;
                }
            }
        }
    }
}

void RenderSkinEditor::mouseDown(const MouseEvent& e)
{
    if(e.mods.isCommandDown())
    {
        placer.setTopLeftPosition(e.getPosition());
        placer.setSize(0,0);
        addAndMakeVisible(placer);
    }
    else
    {
        addAndMakeVisible(this->lasso);
        this->lasso.beginLasso(e, this);
    }
}
void RenderSkinEditor::mouseUp(const MouseEvent& e)
{
    this->lasso.endLasso();
    this->removeChildComponent(&this->placer);
    if(e.mods.isCommandDown())
    {
        SkinComp* sc = skin->createComp();
        sc->graphicArea = this->placer.getBounds();
        sc->setList(&skin->getComps());
        skin->getComps().selectedItems.selectOnly(sc);
    }
}

void RenderSkinEditor::mouseDrag(const MouseEvent& e)
{
    this->lasso.dragLasso(e);
    placer.setSize(e.getDistanceFromDragStartX(), e.getDistanceFromDragStartY());
}

SelectedItemSet<ListItem*>& RenderSkinEditor::getLassoSelection()
{
    return this->skin->getComps().selectedItems;
}

void RenderSkinEditor::changeListenerCallback(ChangeBroadcaster* obj)
{
	if(obj == skin || obj == &skin->getComps())
	{
		boundings.clear();
		controlls.clear();
        
		for(int i = skin->getComps().getNumRows() ; -- i >= 0  ; )
		{
            EditableSkinComp* c = dynamic_cast<EditableSkinComp*>(skin->getComps().items.getUnchecked(i));
			SkinPlacementRect* comp = new SkinPlacementRect(c,&c->graphicArea,this);
			comp->colour = Colours::white;
			comp->setName(  String(i) +  ":graphic");
			boundings.add(comp);
			addAndMakeVisible(comp);
            
			SkinPlacementRect* comp2 = new SkinCompControllPlacementRect(c,&c->controllArea,this);
			comp2->colour = colour;
			comp2->setName( String(i) +  ":controll" );
			controlls.add(comp2);
			addChildComponent(comp2);
			comp2->changeListenerCallback(skin->getComps().items.getUnchecked(i));
		}
        
	}
	repaint();
}

void RenderSkinEditor::paint(Graphics& g)
{
    if(skin)
    {
        if(skin->getSourceImages().size()) // draw in edit mode
        {
            Rectangle<int> c = skin->graphicArea;
            Image img = skin->getSourceImages()[skin->backgroundframe.getValue()];
            g.drawImage(img,0,0,img.getWidth(),img.getHeight(),0,0,img.getWidth(),img.getHeight());
        }
        else
        {
            g.drawText("background image not set properly", 0, 0, getWidth(), getHeight(), Justification::centred, true);
        }
        
        
        g.setColour(colour);
        g.setOpacity(0.5);
        
        for(int i = skin->getComps().getNumRows() ; -- i >= 0 ; )
        {
            SkinComp* comp = skin->getComps().items.getUnchecked(i);
            if(comp->useControllArea == true)
            {
                Rectangle<int> rect = comp->graphicArea.getUnion(comp->controllArea);
                g.drawRect(rect);
            }
        }
    }
}

void RenderSkinEditor::Placer::paint(Graphics &g)
{
    PlacementRect::paint(g, this);
}