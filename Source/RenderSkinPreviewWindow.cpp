
#include "RenderSkinPreviewWindow.h"
#include "SkinGUI.h"

RenderSkinPreviewWindow::RenderSkinPreviewWindow(SkinGUI* gui,ScopedPointer<RenderSkinPreviewWindow>& pointer):
DialogWindow("preview",Colours::white,true)
{
    this->pointer = &pointer;
    this->setContentOwned(gui, true);
    this->gui = gui;
}
void RenderSkinPreviewWindow::closeButtonPressed()
{
    (*this->pointer) = nullptr;
}