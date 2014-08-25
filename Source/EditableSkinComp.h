#pragma once
#include "SkinComp.h"
class EditableSkin;

class EditableSkinComp:
public SkinComp
{
public:
    EditableSkinComp(EditableSkin* skin);
    ~EditableSkinComp();
    
    XmlElement* createXml();
    void setFromXml(XmlElement* el);
    
    Range<int> getRange()const override;
    void setRange(int start,int end);
    void setFirtFrame(int);
    void setLastFrame(int);
    
    double getGradient()const;
    void setGradient(double g);

    EditableSkin* getSkin()const;
    
    Value useFullFrameRange;
    
private:
    WeakReference<EditableSkin> skin;
    
    WeakReference<EditableSkinComp>::Master masterReference;
    friend class WeakReference<EditableSkinComp>;
};