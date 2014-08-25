#pragma once
#include "Skin.h"

/**
 a subclass of skin adding methods to edit and save a skin
 **/
class EditableSkin:
public Skin,
public Value::Listener
{
public:
    EditableSkin();
    ~EditableSkin();
    
    juce_UseDebuggingNewOperator
    
    virtual SkinComp* createComp() override;
    
    virtual int getWidth()const override;
    virtual int getHeight()const override;
    
    virtual void valueChanged (Value& value) override;

    
    void loadFromXml(XmlElement* el);
    XmlElement* createXml()const;
    
    void loadImages();
    bool save();
    bool isSaved() const;
    
    bool attemptToClose();
    
    bool isExported() const;
    bool isFileEqualToState(const File& file) const;
    
    void produce();

    void createSkin(const File& file, ImageFileFormat& format);
    
    const Array<Image>& getSourceImages() const;
    
    File getTargetPath() const;
    File getSourceImagePath()const;
    
    double getQuality()const;
    void setQuality(double quality);
    
    /**
     the index form the image stack to use as background
     **/
    Value backgroundframe;
    
    /**
     sets the opacity for the clipping masks overlay in the editor
     **/
    Value maskOpacity;
    
private:
    /**
     the path to the source images
     **/
    File sourceImagePath;
    
    /**
     the path to save the skin to
     **/
	File targetPath;
    
    /**
     the quaility of the exported images if applicapbe (eg. jpeg quality)
     **/
    Value quality;
    
    /**
     source images
     **/
    Array<Image> sourceImages;

    
    friend class SkinProps;
    WeakReference<EditableSkin>::Master masterReference;
    friend class WeakReference<EditableSkin>;
    
};