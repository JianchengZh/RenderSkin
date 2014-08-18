#pragma once
#include "JuceHeader.h"
#include "NamableObjectLabel.h"

class Skin;
class EditableSkin;
class RenderSkinPreviewWindow;
class PluginWindow;

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
    void createPlugin(PluginDescription*);
    double getScale()const;
    
    void openPluginEditor();
    void clearPlugin();
    
private:
    WeakReference<EditableSkin> skin;
    NamableObjectLabel name;
    
	TextButton fitBoundsToImage;
	TextButton test;
	TextButton exportFile;
    TextButton preview;
    
    TextButton loadPlugin;
    TextButton showPlugin;
    
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
    
    ScopedPointer<AudioPluginInstance> plugin;
    SafePointer<PluginWindow> pluginUI;
    
    AudioPluginFormatManager pluginFormats;
    KnownPluginList knownPlugins;
};

class PluginWindow :
public DocumentWindow,
public DeletedAtShutdown
{
public:
    PluginWindow(Component* ed,bool owned = true);
    ~PluginWindow();
    void closeButtonPressed();
};