#pragma once
#include "JuceHeader.h"
#include "NamableObject.h"

class EditableSkinComp;
class SkinPlacementRect;
class EditableSkin;

/**
    a property box to set the selected controls properties
 **/
class SkinCompPropsGUI :
public Component,
public Slider::Listener,
public ChangeListener,
public TextEditor::Listener,
public ComboBox::Listener,
public Button::Listener,
public Label::Listener
{
public:
	SkinCompPropsGUI(EditableSkin* comp = nullptr);
	~SkinCompPropsGUI();
    
    juce_UseDebuggingNewOperator
    
	void changeListenerCallback(ChangeBroadcaster* obj)override;

	void resized()override;
    void paintOverChildren(Graphics& g)override;
    
    void comboBoxChanged(ComboBox* box)override;
    void labelTextChanged(Label* lab)override;
    void buttonClicked(Button* b)override;
	void sliderValueChanged(Slider* slider)override;
    void textEditorTextChanged (TextEditor&)override;
    
    EditableSkinComp* getComp()const;
    
private:
	Slider firstFrame;
	Slider lastFrame;
    
	Slider sensitivity;
    
	ToggleButton interpolate;
	ToggleButton useControllArea;
    ToggleButton useFullRange;

	Slider frame;
	
    TextEditor helpText;
    TextEditor title;

	Label name;
    
    ComboBox clipMode;
    ComboBox mode;
    
    WeakReference<EditableSkin> skin;
};