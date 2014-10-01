#include "RenderSkinMainComponent.h"
#include "EditableSkin.h"

RenderSkinMainComponent::RenderSkinMainComponent(RenderSkin* app):
menu(app),
tabs(TabbedButtonBar::TabsAtTop)
{
    this->app = app;
    this->addKeyListener(app->getKeyMappings());
    this->app->registerAllCommandsForTarget(this);
    
    const int cmd = ModifierKeys::commandModifier;
    const int del = KeyPress::backspaceKey;
    app->getKeyMappings()->addKeyPress(RenderSkin::undo, KeyPress('z',cmd,'z'));
    app->getKeyMappings()->addKeyPress(RenderSkin::redo, KeyPress('z',cmd | ModifierKeys::shiftModifier,'Z'));
    app->getKeyMappings()->addKeyPress(RenderSkin::removeItem, KeyPress(del,ModifierKeys(),0));
    app->getKeyMappings()->addKeyPress(RenderSkin::open, KeyPress('o',cmd,0));
    app->getKeyMappings()->addKeyPress(RenderSkin::newskin, KeyPress('n',cmd,0));
    app->getKeyMappings()->addKeyPress(RenderSkin::newskin, KeyPress('n',cmd,0));
    app->getKeyMappings()->addKeyPress(RenderSkin::save, KeyPress('s',cmd,0));
    app->getKeyMappings()->addKeyPress(RenderSkin::quit, KeyPress('q',cmd,0));
    
    ScopedPointer<XmlElement> el = app->props.getUserSettings()->getXmlValue("keymap");
    if(el)
    {
        app->getKeyMappings()->restoreFromXml(*el);
    }
    
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
    commands.add(RenderSkin::quit);
    commands.add(RenderSkin::addComp);
    
    commands.add(RenderSkin::undo);
    commands.add(RenderSkin::redo);
    commands.add(RenderSkin::removeItem);
    
    commands.add(RenderSkin::editKeyMappings);
    
    
    
}

void RenderSkinMainComponent::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{
    const String skinName = this->app->getCurrentSkin() ? this->app->getCurrentSkin()->getName() : "skin";
    result.setActive(this->app->getCurrentSkin());
    switch(commandID)
    {
        case RenderSkin::editKeyMappings:
        {
            result.setInfo("keyMap", "edit key mappings", "config", 0);
            break;
        }
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
            
        case RenderSkin::quit:
        {
            result.setInfo("quit","quit RenderRkin","file",0);
            break;
        }
        case RenderSkin::closeSkin:
        {
            result.setInfo("close skin","close "+skinName,"file",0);
            result.setActive(this->app->getCurrentSkin());
            break;
        }
        case RenderSkin::addComp:
        {
            result.setInfo("add comp","adds a comp to "+skinName,"comps",0);
            result.setActive(this->app->getCurrentSkin());
            break;
        }
        case RenderSkin::undo:
        {
            result.setInfo("undo","undo: " + this->app->getHistory()->getUndoDescription(),"edit",0);
            result.setActive(this->app->getHistory()->canUndo());
            break;
        }
        case RenderSkin::redo:
        {
            result.setInfo("redo","redo: " + this->app->getHistory()->getRedoDescription(),"edit",0);
            result.setActive(this->app->getHistory()->canRedo());
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
        case RenderSkin::undo:
        {
            this->app->getHistory()->undo();
            break;
        }
        case RenderSkin::redo:
        {
            this->app->getHistory()->redo();
            break;
        }
        case RenderSkin::editKeyMappings:
        {
            KeyMappingEditorComponent editor(*this->app->getKeyMappings(),false);
            editor.centreWithSize(500, 500);
            DialogWindow::showModalDialog("key map", &editor, this, Colours::white, true);
            break;
        }
        case RenderSkin::removeItem:
        {
            D3CKHistory::beginNewTransaction("delete selected comps", this);
            app->getCurrentSkin()->deleteSelectedComps(D3CKHistory::find(this));
            break;
        }
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
            if(AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon,"close "+app->getCurrentSkin()->getName()+"?","close "+app->getCurrentSkin()->getName()+"?"))
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
        case RenderSkin::quit:
        {
            app->attemptToClose();
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
