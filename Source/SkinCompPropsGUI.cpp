#include "SkinCompPropsGUI.h"
#include "EditableSkinComp.h"
#include "EditableSkin.h"
#include "SkinPlacementRect.h"
//#include "ShottyLook.h"
#include "EditableSkinComp.h"

SkinCompPropsGUI::SkinCompPropsGUI(EditableSkin* skin)
{
    this->skin = skin;
    
	addAndMakeVisible(&firstFrame);
	addAndMakeVisible(&lastFrame);
	addAndMakeVisible(&frame);
    
    addAndMakeVisible(gradient);
    gradient.addListener(this);
    gradient.setRange(0, 1);
    
    addAndMakeVisible(&name);
    name.setEditable(true);
    name.addListener(this);
    
    addAndMakeVisible(&helpText);
    addAndMakeVisible(&title);
    
    addAndMakeVisible(clipMode);
    clipMode.addListener(this);
    
    addAndMakeVisible(mode);
    mode.addListener(this);
    
    addAndMakeVisible(interpolate);
    interpolate.setButtonText("interpolate");
    interpolate.addListener(this);
    
	addAndMakeVisible(useControllArea);
    useControllArea.setButtonText("userControllArea");
    useControllArea.addListener(this);
    
    addAndMakeVisible(useFullRange);
    useFullRange.setButtonText("userFullRange");
    useFullRange.addListener(this);
    
    this->clipMode.addItem("rect", SkinComp::ClipType::rect+1);
    this->clipMode.addItem("ellipse", SkinComp::ClipType::ellipse+1);
//    this->clipMode.addItem("gradient rect", SkinComp::ClipType::smoothrect+1);
    this->clipMode.addListener(this);
    
    this->mode.addItem("image", SkinComp::CompType::none+1);
    this->mode.addItem("slider", SkinComp::CompType::slider+1);
    this->mode.addItem("button", SkinComp::CompType::togglebutton+1);
    this->mode.addListener(this);
    
    this->helpText.addListener(this);
    this->title.addListener(this);

	firstFrame.setSliderStyle(Slider::IncDecButtons);
	firstFrame.addListener(this);
	lastFrame.setSliderStyle(Slider::IncDecButtons);
	lastFrame.addListener(this);

	sensitivity.addListener(this);
	sensitivity.setRange(10,2000);

	frame.setRange(0,1);
	frame.setSliderStyle(Slider::Rotary);
	frame.addListener(this);
    
    helpText.setTextToShowWhenEmpty("help text", Colours::lightgrey);
    helpText.setMultiLine(true);
    
    this->skin->getComps().selectedItems.addChangeListener(this);
}

SkinCompPropsGUI::~SkinCompPropsGUI()
{
    if(skin)
    {
        skin->getComps().selectedItems.removeChangeListener(this);
    }
}

void SkinCompPropsGUI::labelTextChanged(Label* lab)
{
    if(lab == &this->name)
    {
        if(this->getComp())
        {
            this->getComp()->setName(lab->getText());
        }
    }
}

void SkinCompPropsGUI::buttonClicked(Button* b)
{
    if(b == &this->useFullRange)
    {
        for(int i = this->skin->getComps().selectedItems.getNumSelected() ; -- i >= 0 ; )
        {
            EditableSkinComp* c =  dynamic_cast<EditableSkinComp*>(this->skin->getComps().selectedItems.getSelectedItem(i));
            if(c)
            {
                c->useFullFrameRange = b->getToggleState();
            }
        }
    }
    else if(b == &this->interpolate)
    {
        for(int i = this->skin->getComps().selectedItems.getNumSelected() ; -- i >= 0 ; )
        {
            SkinComp* c =  dynamic_cast<SkinComp*>(this->skin->getComps().selectedItems.getSelectedItem(i));
            if(c)
            {
                c->interpolation = b->getToggleState();
            }
        }

    }
    else if(b == &this->useControllArea)
    {
        for(int i = this->skin->getComps().selectedItems.getNumSelected() ; -- i >= 0 ; )
        {
            SkinComp* c =  dynamic_cast<SkinComp*>(this->skin->getComps().selectedItems.getSelectedItem(i));
            if(c)
            {
                c->setUseControllArea(b->getToggleState());
                c->sendChangeMessage();
            }
        }
    }
}

void SkinCompPropsGUI::comboBoxChanged(ComboBox* box)
{
    if(&mode == box)
    {
        for(int i = this->skin->getComps().selectedItems.getNumSelected() ; -- i >= 0 ; )
        {
            SkinComp* c =  dynamic_cast<SkinComp*>(this->skin->getComps().selectedItems.getSelectedItem(i));
            if(c)
            {
                c->setType((SkinComp::CompType)(box->getSelectedId() - 1));
            }
        }
    }
    else if(&clipMode == box)
    {
        for(int i = this->skin->getComps().selectedItems.getNumSelected() ; -- i >= 0 ; )
        {
            SkinComp* c =  dynamic_cast<SkinComp*>(this->skin->getComps().selectedItems.getSelectedItem(i));
            if(c)
            {
                c->setClipType((SkinComp::ClipType)(box->getSelectedId() - 1));
            }
        }
    }
        
}

void SkinCompPropsGUI::paintOverChildren(Graphics& g)
{
    if(this->skin && this->skin->getComps().selectedItems.getNumSelected() > 1)
    {
        g.setColour(Colours::red.withAlpha(0.3f));
        g.fillRect(mode.getBounds());
        g.fillRect(clipMode.getBounds());
        g.fillRect(interpolate.getBounds());
        g.fillRect(useControllArea.getBounds());
        g.fillRect(useFullRange.getBounds());
    }
}

void SkinCompPropsGUI::changeListenerCallback(ChangeBroadcaster* obj)
{
    EditableSkinComp* comp = this->getComp();
    
    this->setAlpha(comp ? 1 : 0.5);
    this->setEnabled(comp);
    
    repaint();
    
    if(!comp)
    {
        return;
    }
    
    name.setText(comp->getName(), NotificationType::dontSendNotification);
	sensitivity.setValue(comp->getSensitivity(),NotificationType::dontSendNotification);
    
    firstFrame.setEnabled(!comp->useFullFrameRange.getValue());
	firstFrame.setRange(0,comp->getSkin()->getSourceImages().size()-1,1);
	firstFrame.setValue(comp->getRange().getStart(),NotificationType::dontSendNotification);
    
    lastFrame.setEnabled(!comp->useFullFrameRange.getValue());
	lastFrame.setRange(0,comp->getSkin()->getSourceImages().size()-1,1);
	lastFrame.setValue(comp->getRange().getEnd(),NotificationType::dontSendNotification);
    
    gradient.setValue(comp->getGradient());
    
    this->helpText.setText(comp->helpText);
    this->title.setText(comp->title);
    
    this->clipMode.setSelectedId(comp->getClipType()+1);
    this->mode.setSelectedId(comp->compType+1);
    
    this->interpolate.setToggleState(comp->interpolation.getValue(), NotificationType::dontSendNotification);
    this->useControllArea.setToggleState(comp->useControllArea.getValue(), NotificationType::dontSendNotification);
    useFullRange.setToggleState(comp->useFullFrameRange.getValue(), NotificationType::dontSendNotification);
}

EditableSkinComp* SkinCompPropsGUI::getComp()const
{
    return dynamic_cast<EditableSkinComp*>(this->skin->getComps().selectedItems.getSelectedItem(0));
}

void SkinCompPropsGUI::textEditorTextChanged (TextEditor& e)
{
    EditableSkinComp* comp = this->getComp();
    if(!comp)
    {
        return;
    }
    
    if(&e == &this->helpText)
    {
        comp->helpText = this->helpText.getText();
    }
    else if(&e == & title)
    {
        comp->title = this->title.getText();
    }
    
    comp->sendChangeMessage();
};

void SkinCompPropsGUI::sliderValueChanged(Slider* slider)
{
    EditableSkinComp* comp = this->getComp();
    if(!comp)
    {
        return;
    }

	if(slider == &firstFrame)
	{
		comp->setFirtFrame(slider->getValue());
	}
	else if(slider == &lastFrame)
	{
		comp->setLastFrame(slider->getValue());
	}
	else if(slider == &frame)
	{
		comp->setValue(slider->getValue());
	}
	else if(slider == &sensitivity)
	{
		comp->sensitivity = sensitivity.getValue();
	}
    else if(slider == &gradient)
	{
		comp->setGradient(gradient.getValue());
	}
    
    comp->sendChangeMessage();

}

void SkinCompPropsGUI::resized()
{
	int x = 0;
	int y = 0;
    
    int h= 20;
	name.setBounds(x,y,getWidth(),h);
	y+= h;
	int w = (getWidth())/2;
    
    mode.setBounds(x, y, getWidth(), h);
    y+=mode.getHeight();
    
    clipMode.setBounds(x, y, getWidth(), h);
    y+=clipMode.getHeight();
    
    gradient.setBounds(x, y, getWidth(), h);
    y+=y;
    
	firstFrame.setBounds(x,y,w,h);
	x=firstFrame.getRight();
	lastFrame.setBounds(x,y,w,h);
	y+=h;
    x=0;
    
    useFullRange.setBounds(x, y, getWidth(), h);
    y+=useFullRange.getHeight();
    
	frame.setBounds(x,y,getWidth(),h);
	y+=h;
	interpolate.setBounds(x,y,getWidth(),h);
	y+=h;
	useControllArea.setBounds(x,y,getWidth(),h);
	y+=h;

    
    title.setBounds(x,y,getWidth(),h);
    y+=title.getHeight();
    
    helpText.setBounds(x,y,getWidth(),h*4);
    y+=helpText.getHeight();
}