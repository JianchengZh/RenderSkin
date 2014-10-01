#include "RenderSkin.h"
#include "EditableSkin.h"

RenderSkin::RenderSkin()
{
    PropertiesFile::Options opts;
    this->currentSkin = nullptr;
    
    opts.commonToAllUsers = false;
    opts.applicationName = "RenderSkin";
    opts.storageFormat = PropertiesFile::storeAsXML;
    opts.osxLibrarySubFolder = "Application Support";
    props.setStorageParameters(opts);
    MenuBarModel::setApplicationCommandManagerToWatch(this);
    
    ScopedPointer<XmlElement> el = props.getUserSettings()->getXmlValue("workspace");
    if(el)
    {
        XmlElement* skinXml = el->getFirstChildElement();
        while(skinXml)
        {
            File skinFile(skinXml->getStringAttribute("path"));
            if(skinFile.existsAsFile())
            {
                EditableSkin* skin = new EditableSkin(this);
                addSkin(skin);
                skin->loadFromFile(skinFile);
            }
            skinXml = skinXml->getNextElement();
        }
    }
    
}

RenderSkin::~RenderSkin()
{
    ScopedPointer<XmlElement> el = this->getKeyMappings()->createXml(true);
    props.getUserSettings()->setValue("keymap", el);
    

    masterReference.clear();
}

Array<EditableSkin*> RenderSkin::getOpenSkins()const
{
    Array<EditableSkin*> arr;
    for(int i = 0 ; i < skins.size() ; ++i)
    {
        arr.add(skins.getUnchecked(i));
    }
    return arr;
}
EditableSkin* RenderSkin::getCurrentSkin()const
{
    return this->currentSkin;
}

void RenderSkin::setCurrentSkin(EditableSkin* skin)
{
    this->currentSkin = skin;
}

bool RenderSkin::attemptToClose()
{
    XmlElement el("workspace");
    
    bool close = true;
    
    for(int i = 0 ; i < skins.size() ; i++)
    {
        EditableSkin* skin = skins.getUnchecked(i);
        close = skin->attemptToClose();
        File f = skin->getFile();
        
        XmlElement* doc = new XmlElement("document");
        doc->setAttribute("path", f.getFullPathName());
        el.addChildElement(doc);
    }
    
    props.getUserSettings()->setValue("workspace",&el);
    
    return close;
}

StringArray RenderSkin::getMenuBarNames()
{
    return ApplicationCommandManager::getCommandCategories();
}

PopupMenu RenderSkin::getMenuForIndex (int topLevelMenuIndex, const String& menuName)
{
    Array<int> cmds = ApplicationCommandManager::getCommandsInCategory(menuName);
    PopupMenu menu;
    for(int i = 0 ; i < cmds.size(); i++)
    {
        
        menu.addCommandItem(this,cmds.getUnchecked(i));
    }
    return menu;
}

void RenderSkin::menuItemSelected (int menuItemID, int topLevelMenuIndex)
{
}

//safely removes and deletes a skin
void RenderSkin::removeSkin(EditableSkin* skin)
{
    Skin* skinToDelete = this->currentSkin;
    this->skins.removeObject(this->currentSkin,false);
    //skin->sendSynchronousChangeMessage(); // neede to inform the ui
    delete skinToDelete;
    this->sendChangeMessage();
}

D3CKHistory* RenderSkin::getHistory()const
{
    return &const_cast<RenderSkin*>(this)->history;
}

void RenderSkin::addSkin(EditableSkin* skin)
{
    skins.add(skin);
//    skin->getComps().setListHistory(&this->history);
//    skin->addChangeListener(this);
    sendChangeMessage();
}

void RenderSkin::addSkin(const File& file)
{
    EditableSkin* skin = new EditableSkin(this);
    addSkin(skin);
    skin->loadFromFile(file);
}

void RenderSkin::changeListenerCallback(ChangeBroadcaster* obj)
{
//    sendChangeMessage();
}