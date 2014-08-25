
#include "RenderSkinApplication.h"
#include "EditableSkin.h"

void RenderSkinApplication::initialise(const String& commandLineParameters)
{
    app = new RenderSkin();
    StringArray parms;
    parms.addTokens(commandLineParameters, " ");
    
    
    if(parms.contains("-produce"))
    {
        File inputfile(parms[parms.size()-1]);

        if(inputfile.existsAsFile())
        {
            EditableSkin skin;
            skin.loadFromFile(inputfile);
            DBG("producing:" + skin.getName());
            skin.produce();
            DBG("done producing:" + skin.getName());
            this->quit();
        }
    }
    else
    {
        this->window = new RenderSkinMainWindow(app);
    }
}
void RenderSkinApplication::shutdown()
{
    this->window = nullptr;
    this->app = nullptr;
}
const String RenderSkinApplication::getApplicationName()
{
    return "RenderSkin";
}
const String RenderSkinApplication::getApplicationVersion()
{
    return "0.9";
}