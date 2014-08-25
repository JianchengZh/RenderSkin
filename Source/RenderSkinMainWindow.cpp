#include "RenderSkinMainWindow.h"

RenderSkinMainWindow::RenderSkinMainWindow(RenderSkin* app):
DocumentWindow("RenderSkin",Colours::white,DocumentWindow::closeButton,true),
comp(app)
{
    this->app = app;
    
    this->setResizable(true, false);
    comp.setSize(this->app->props.getUserSettings()->getIntValue("width",800), this->app->props.getUserSettings()->getIntValue("height",800));
    this->setContentNonOwned(&comp,true);
    this->setVisible(true);
}

RenderSkinMainWindow::~RenderSkinMainWindow()
{
    this->app->props.getUserSettings()->setValue("width",comp.getWidth());
    this->app->props.getUserSettings()->setValue("height",comp.getHeight());
}

void RenderSkinMainWindow::closeButtonPressed()
{
    if(this->app->attemptToClose())
    {
        JUCEApplication::quit();
    }
}