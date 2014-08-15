
#include "RenderSkinApplication.h"

	void RenderSkinApplication::initialise(const String& commandLineParameters)
	{
		this->window = new RenderSkinMainWindow();
	}
	void RenderSkinApplication::shutdown()
	{
	}
	const String RenderSkinApplication::getApplicationName()
	{
		return "RenderSkin";
	}
	const String RenderSkinApplication::getApplicationVersion()
	{
		return "1";
	}