#pragma once
#include "JuceHeader.h"
#include "RenderSkinMainWindow.h"

/**
    JUCE application for RenderSkin
 **/
class RenderSkinApplication :
	public JUCEApplication
{
	void initialise(const String& commandLineParameters);
	void shutdown();
	const String getApplicationName();
	const String getApplicationVersion();
    
private:
	ScopedPointer<RenderSkinMainWindow>  window;
    ScopedPointer<RenderSkin> app;
};