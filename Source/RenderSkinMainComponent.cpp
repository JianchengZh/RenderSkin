#include "RenderSkinMainComponent.h"
#include "EditableSkin.h"

RenderSkinMainComponent::RenderSkinMainComponent(RenderSkin* app):
menu(app),
tabs(TabbedButtonBar::TabsAtTop)
{
    this->app = app;
    this->addKeyListener(app->getKeyMappings());
    this->app->registerAllCommandsForTarget(this);
    this->app->addChangeListener(this);
    
    this->addAndMakeVisible(&this->menu);
    this->addAndMakeVisible(&this->tabs);
    this->tabs.getTabbedButtonBar().addChangeListener(this);
    
    this->changeListenerCallback(this->app);
}


void RenderSkinMainComponent::changeListenerCallback(ChangeBroadcaster* obj)
{
    if(obj == this->app)
    {
        int tab = jmax(0,this->tabs.getCurrentTabIndex());
        this->tabs.clearTabs();
        for(int i = 0 ; i < this->app->getOpenSkins().size(); i++)
        {
            EditableSkin* skin = this->app->getOpenSkins().getUnchecked(i);
            this->tabs.addTab(skin->getName(),Colours::white,new RenderSkinMainGUI(skin),true);
        }
        this->tabs.setCurrentTabIndex(tab);
    }
    else if(&tabs.getTabbedButtonBar() == obj)
    {
        app->setCurrentSkin(app->getOpenSkins()[tabs.getCurrentTabIndex()]);
    }
}

void RenderSkinMainComponent::resized()
{
    
    menu.setBounds(0,0,getWidth(),15);
    tabs.setBounds(0,15,getWidth(),getHeight()-15);
}

ApplicationCommandTarget* RenderSkinMainComponent::getNextCommandTarget()
{
    return 0;
}

void RenderSkinMainComponent::getAllCommands (Array <CommandID>& commands)
{
    commands.add(RenderSkin::newskin);
    commands.add(RenderSkin::open);
    commands.add(RenderSkin::save);
    commands.add(RenderSkin::closeSkin);
    commands.add(RenderSkin::addComp);
//    commands.add(RenderSkin::undo);
//    commands.add(RenderSkin::redo);
//    commands.add(RenderSkin::removeItem);
}

void RenderSkinMainComponent::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{
    result.setActive(this->app->getCurrentSkin());
    switch(commandID)
    {
        case RenderSkin::newskin:
        {
            result.setInfo("new","create new skin","file",0);
            result.setActive(true);
            
            break;
        }
        case RenderSkin::open:
        {
            result.setInfo("open","open skin","file",0);
            result.setActive(true);
            
            break;
        }
        case RenderSkin::save:
        {
            result.setInfo("save","save skin","file",0);
            result.setActive(this->app->getCurrentSkin() && !this->app->getCurrentSkin()->isSaved());
            break;
        }
        case RenderSkin::closeSkin:
        {
            result.setInfo("close skin","close skin","file",0);
            result.setActive(this->app->getCurrentSkin());
            break;
        }
        case RenderSkin::addComp:
        {
            result.setInfo("add comp","addsa a comp","comps",0);
            result.setActive(this->app->getCurrentSkin());
            break;
        }
        case RenderSkin::undo:
        {
            result.setInfo("undo","undo","edit",0);
            break;
        }
        case RenderSkin::redo:
        {
            result.setInfo("redo","redo","edit",0);
            break;
        }
        case RenderSkin::removeItem:
        {
            result.setInfo("remove","remove","edit",0);
            break;
        }
    }
}

bool RenderSkinMainComponent::perform (const InvocationInfo& info)
{
    switch(info.commandID)
    {
        case RenderSkin::newskin:
        {
            app->addSkin();
            break;
        }
        case RenderSkin::open:
        {
            FileChooser fc("open skin",File::nonexistent,"*.d3ckskin");
            if(fc.browseForFileToOpen())
            {
                app->addSkin(fc.getResult());
            }
            break;
        }
        case RenderSkin::closeSkin:
        {
            if(AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon,"close skin?","close skin?"))
            {
                if(app->getCurrentSkin()->attemptToClose())
                {
                    app->removeSkin(app->getCurrentSkin());
                }
            }
            break;
        }
        case RenderSkin::save:
        {
            app->getCurrentSkin()->save();
            break;
        }
        case RenderSkin::addComp:
        {
            Skin* s = app->getOpenSkins().getUnchecked(tabs.getCurrentTabIndex());
            SkinComp* sc = s->createComp();
            sc->setList(const_cast<OwnedList<SkinComp>*>(&s->getComps()));
            break;
        }
    }
    return true;
}
