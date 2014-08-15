#include "SkinPreviewGUI.h"
#include "EditableSkin.h"
#include "SkinPreviewCompGUI.h"
#include "EditableSkinComp.h"

SkinPreviewGUI::SkinPreviewGUI(EditableSkin* skin):
SkinGUI(skin)
{
    this->skin = skin;
    this->changeListenerCallback(&skin->getComps());
}

SkinPreviewGUI::~SkinPreviewGUI()
{
    
}

void SkinPreviewGUI::paint(Graphics& g)
{
    Rectangle<int> c = skin->graphicArea;
    Image img = skin->getSourceImages()[skin->backgroundframe.getValue()];
    g.drawImage(img,0,0,getWidth(),getHeight(),c.getX(),c.getY(),c.getWidth(),c.getHeight());
}

SkinCompGUI* SkinPreviewGUI::createCompGUI(SkinComp* comp)
{
    EditableSkinComp* c = dynamic_cast<EditableSkinComp*>(comp);
    jassert(c)//previewer takes only editable comps
    return new SkinPreviewCompGUI(c, this);
}
