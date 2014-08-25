#include "SkinPlacementRect.h"
#include "EditableSkinComp.h"
#include "EditableSkin.h"
#include "SkinCompPropsGUI.h"
#include "RenderSkinEditor.h"

SkinPlacementRect::SkinPlacementRect(EditableSkinComp* comp,BoundsDriver* rect,RenderSkinEditor* editor ):
PlacementRect(rect)
{
    this->editor = editor;
	this->comp = comp;
    this->editor->getSkin()->getComps().selectedItems.addChangeListener(this);
    
    comp->addChangeListener(this);
}

SkinPlacementRect::~SkinPlacementRect()
{
    if(this->editor->getSkin())
    {
        this->editor->getSkin()->getComps().selectedItems.removeChangeListener(this);
    }
    if(this->comp)
    {
        this->comp->removeChangeListener(this);
    }
};

void SkinPlacementRect::changeListenerCallback(ChangeBroadcaster*obj)
{
	BoundDrivenComponent::changeListenerCallback(obj);
	repaint();
}

const Colour& SkinPlacementRect::getColour() const
{
    return this->isSelected() ? Colours::red : this->colour;
}

void SkinPlacementRect::paint(Graphics& g)
{
    if(this->comp)
    {
        PlacementRect::paint(g,this);
        g.setOpacity(this->comp->getSkin()->maskOpacity.getValue());
        const Image mask = this->comp->getMask(this->getBounds());
        g.drawImageAt(mask, 0, 0);
    }
}

bool SkinPlacementRect::isSelected() const
{
    if(this->editor->getSkin())
    {
        return this->editor->getSkin()->getComps().selectedItems.isSelected(this->comp);
    }
    return false;
}

void SkinPlacementRect::mouseDown(const MouseEvent& e)
{
    this->editor->getSkin()->getComps().selectedItems.selectOnly(this->comp);
    PlacementRect::mouseDown(e);
}

void SkinPlacementRect::mouseUp(const MouseEvent& e)
{
	PlacementRect::mouseUp(e);
    if(e.mods.isMiddleButtonDown())
    {
        AlertWindow win(
                        "name",
                        "name",
                        AlertWindow::NoIcon,
                        this);
        win.addTextEditor("name", comp->getName());
        win.addButton("ok", 1);
        if(win.runModalLoop())
        {
            comp->setName(win.getTextEditorContents("name"));
        }
    }
	if(e.mods.isPopupMenu())
	{
		PopupMenu menu;
		menu.addItem(1,"remove");
        
		PopupMenu indexSelect;
		int index = comp->getSkin()->getComps().items.indexOf(comp);
        
		for(int i = 0 ; i < comp->getSkin()->getComps().items.size(); i++)
        {
			indexSelect.addItem(1000+i,String(i),true, index == i);
        }
        
		menu.addSubMenu("index",indexSelect);
        
		PopupMenu compType;
//		compType.addItem(10000,"graphic",true,comp->getCompType() == 0);
//		compType.addItem(10001,"slider",true,comp->getCompType() == 1);
//		compType.addItem(10002,"toggleButton",true,comp->getCompType() == 2);
		menu.addSubMenu("mode",compType);
        
		PopupMenu clipType;
		clipType.addItem(20000,"rect",true,comp->getClipType() == SkinComp::rect);
		clipType.addItem(20001,"ellipse",true,comp->getClipType() == SkinComp::ellipse);
		clipType.addItem(20002,"smooth rect",true,comp->getClipType() == SkinComp::smoothrect);
		menu.addSubMenu("clip",clipType);
        
        EditableSkin* skin = dynamic_cast<EditableSkin*>(this->comp->getSkin());
        jassert(skin);
		SkinCompPropsGUI props(skin);
		menu.addCustomItem(99999999,&props,150,160,false);
        
		int res = menu.show();
        
		if(res >= 20000)
		{
			comp->setClipType(  (SkinComp::ClipType)(res - 20000));
			comp->sendChangeMessage();
		}
		else if(res >= 10000)
		{
			comp->setType((SkinComp::CompType)(res - 10000));
		}
		else if(res >= 1000)
		{
			comp->setList(&comp->getSkin()->getComps(),res-1000,true);
		}
		else if(res == 1)
		{
			comp->moveToTrash(true);
		}
	}
}