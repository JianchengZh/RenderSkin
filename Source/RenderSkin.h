#pragma once
#include "JuceHeader.h"
#include "SkinViewComp.h"
#include "SkinComp.h"

#include "List.h"
#include "D3CKHistory.h"

class EditableSkin;

/**
 this is the main application opject for the render skin editor
 **/
class RenderSkin :
public ApplicationCommandManager,
public MenuBarModel,
public ChangeBroadcaster,
public ChangeListener
{
public:
    enum Commands
    {
        open = 1,
        save,
        closeSkin,
        newskin,
        undo,
        redo,
        addComp,
        removeItem
    };
    
	RenderSkin();
	~RenderSkin();
    
    juce_UseDebuggingNewOperator
    
	StringArray getMenuBarNames()override;
	PopupMenu getMenuForIndex (int topLevelMenuIndex, const String& menuName)override;
    void menuItemSelected (int menuItemID, int topLevelMenuIndex)override;
    
    void changeListenerCallback(ChangeBroadcaster* obj)override;

    //safely removes and deletes a skin
    void removeSkin(EditableSkin* skin);
	void addSkin(EditableSkin* skin);
	void addSkin(const File& file = File::nonexistent);
    bool attemptToClose();
    


    
    Array<EditableSkin*> getOpenSkins()const;
    EditableSkin* getCurrentSkin()const;
    
    void setCurrentSkin(EditableSkin* );
    
    ApplicationProperties props;
    
private:
    
	CriticalSection lock;
    D3CKHistory history;
	EditableSkin* currentSkin;
	OwnedArray<EditableSkin> skins;
    
    WeakReference<RenderSkin>::Master masterReference;
    friend class WeakReference<RenderSkin>;
};