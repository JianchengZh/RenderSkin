#include "SkinPreviewCompGUI.h"
#include "EditableSkinComp.h"
#include "EditableSkin.h"

SkinPreviewCompGUI::SkinPreviewCompGUI(EditableSkinComp* c,SkinGUI* s):
SkinCompGUI(c,s)
{
    this->comp = c;
}
SkinPreviewCompGUI::~SkinPreviewCompGUI()
{
    
}
void SkinPreviewCompGUI::paint(Graphics&g)
{
    double frame = comp->getAbsolutFrame();
    
    int index0 = jlimit((int)0 , comp->getSkin()->getSourceImages().size()-1 ,(int)frame  );
    int index1 = jlimit((int)0 , comp->getSkin()->getSourceImages().size()-1 , index0+1 );
    float r2 = frame - index0;
    
    Rectangle<int> b = comp->graphicArea;
    Rectangle<int> bScaled = b * this->getScale();
    
    g.saveState();
    if(this->useMask())
    {
        g.reduceClipRegion(getMask(), AffineTransform::identity);
    }
    
    if(index0 == index1 || comp->interpolation != true)
    {
        Image img =comp->getSkin()->getSourceImages()[index0];
        g.drawImage(img,0,0,bScaled.getWidth(),bScaled.getHeight(),b.getX(),b.getY(),b.getWidth(),b.getHeight());
    }
    else
    {
        Image img = comp->getSkin()->getSourceImages()[index0];
        g.drawImage(img,0,0,bScaled.getWidth(),bScaled.getHeight(),b.getX(),b.getY(),b.getWidth(),b.getHeight());
        
        g.setOpacity(r2);
        Image img2 = comp->getSkin()->getSourceImages()[index1];
        g.drawImage(img2,0,0,bScaled.getWidth(),bScaled.getHeight(),b.getX(),b.getY(),b.getWidth(),b.getHeight());
    }
    
    g.restoreState();
    
    g.setOpacity(this->comp->getSkin()->maskOpacity.getValue());
    g.drawImageAt(this->getMask(), 0, 0);
}