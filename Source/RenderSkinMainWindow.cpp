#include "RenderSkinMainWindow.h"

RenderSkinMainWindow::RenderSkinMainWindow():
DocumentWindow("RenderSkin",Colours::white,DocumentWindow::closeButton,true),
comp(&skin)
{
    this->setResizable(true, false);
    comp.setSize(this->skin.props.getUserSettings()->getIntValue("width",800), this->skin.props.getUserSettings()->getIntValue("height",800));
    this->setContentNonOwned(&comp,true);
    this->setVisible(true);
}

RenderSkinMainWindow::~RenderSkinMainWindow()
{
    this->skin.props.getUserSettings()->setValue("width",comp.getWidth());
    this->skin.props.getUserSettings()->setValue("height",comp.getHeight());
}

void RenderSkinMainWindow::closeButtonPressed()
{
    if(this->skin.attemptToClose())
    {
        JUCEApplication::quit();
    }
}