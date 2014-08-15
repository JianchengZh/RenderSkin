#pragma once
#include "JuceHeader.h"
#include "NamableObjectLabel.h"

class Skin;
class EditableSkin;
class RenderSkinPreviewWindow;

class SkinProps:
public Slider::Listener,
public Component,
public ComboBox::Listener,
public Button::Listener,
public FilenameComponentListener,
public ChangeListener

{
public:
    SkinProps(EditableSkin* skin);
    ~SkinProps();
    
    void resized()override;
    
    void filenameComponentChanged (FilenameComponent* fileComponentThatHasChanged)override;
	void sliderValueChanged(Slider*slider)override;
	void comboBoxChanged(ComboBox* box)override;
    void changeListenerCallback(ChangeBroadcaster* obj)override;
    void buttonClicked(Button* button)override;

    double getScale()const;

private:
    WeakReference<EditableSkin> skin;
    NamableObjectLabel name;
    
	TextButton fitBoundsToImage;
	TextButton test;
	TextButton exportFile;
    TextButton preview;
    
	FilenameComponent imagePath;
	FilenameComponent targetPath;
    
	ComboBox format;
	Slider quality;
    Slider screenRatio;
	Slider bgFrame;
    Slider maskOpacity;
    
    ScopedPointer<RenderSkinPreviewWindow> previewWindow;
    ScopedPointer<RenderSkinPreviewWindow> testWindow;

    ScopedPointer<Skin> testInstance;
};