#pragma once
#include "JuceHeader.h"
#include "RenderSkin.h"
#include "RenderSkinMainGUI.h"

class RenderSkinMainComponent:
public Component,
public ApplicationCommandTarget,
public ChangeListener
{
public:
	RenderSkinMainComponent(RenderSkin* skin);
    
	juce_UseDebuggingNewOperator
    
	void changeListenerCallback(ChangeBroadcaster* obj)override;
	void resized()override;
    ApplicationCommandTarget* getNextCommandTarget()override;
    void getAllCommands (Array <CommandID>& commands)override;
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)override;
    bool perform (const InvocationInfo& info)override;
    
private:
    RenderSkin* skin;
	MenuBarComponent menu;
	TabbedComponent tabs;
};