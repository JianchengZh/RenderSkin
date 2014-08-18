#include "RenderSkinMainGUI.h"
#include "EditableSkin.h"

RenderSkinMainGUI::RenderSkinMainGUI(EditableSkin* skin):
skinProperties(skin),
properties(skin),
editor(skin)
{
	this->skin = skin;
    
	addAndMakeVisible(editor);
    addAndMakeVisible(skinProperties);
    addAndMakeVisible(properties);
}

RenderSkinMainGUI::~RenderSkinMainGUI()
{

}

void RenderSkinMainGUI::resized()
{
    int y = 0;
    int ch = 20;
    int th = this->getHeight() - ch * 15;
    
    properties.setBounds(0, y, 150, th);
    y =  th;
    
    skinProperties.setBounds(0, y, properties.getWidth(), getHeight() - y);
    editor.setBounds(properties.getWidth(),0,getWidth()-properties.getWidth(),getHeight());
    y = editor.getHeight();
}