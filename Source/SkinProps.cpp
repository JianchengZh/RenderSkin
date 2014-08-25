#include "SkinProps.h"
#include "Skin.h"
#include "RenderSkinPreviewWindow.h"
#include "SkinGUI.h"
#include "SkinPreviewGUI.h"
#include "EditableSkin.h"
#include "AudioProcessorSkin.h"

SkinProps::SkinProps(EditableSkin* skin):
name(skin),
imagePath("sourceimagepath",skin->getSourceImagePath(),false,true,false,"*","","select image source folder"),
targetPath("targetPath",skin->getTargetPath(),false,true,true,"*","","select folder to save skin to")
{
    this->skin = skin;
    skin->addChangeListener(this);
    
    addAndMakeVisible(name);
	addAndMakeVisible(fitBoundsToImage);
	addAndMakeVisible(test);
	addAndMakeVisible(exportFile);
    
    addAndMakeVisible(loadPlugin);
    loadPlugin.addListener(this);
    loadPlugin.setButtonText("load plugin");
    
    addAndMakeVisible(showPlugin);
    showPlugin.addListener(this);
    showPlugin.setButtonText("show plugin");
    
    addAndMakeVisible(maskOpacity);
    maskOpacity.addListener(this);
    maskOpacity.setRange(0, 1);
    
    addAndMakeVisible(screenRatio);
    screenRatio.addListener(this);
    screenRatio.setRange(0.1, 1);
    
    addAndMakeVisible(quality);
    this->quality.setValue(skin->getQuality());//(skin->getQualtiy());
    this->quality.setRange(0, 1);
    quality.setSliderStyle(Slider::SliderStyle::Rotary);
    quality.addListener(this);
    
    addAndMakeVisible(preview);
    preview.setButtonText("preview");//
    preview.addListener(this);
    
	fitBoundsToImage.addListener(this);
	fitBoundsToImage.setButtonText("fit bounds");
    
	test.addListener(this);
	test.setButtonText("test");
    
    addAndMakeVisible(imagePath);
	imagePath.addListener(this);
    
    addAndMakeVisible(targetPath);
	targetPath.addListener(this);
    
	exportFile.addListener(this);
	exportFile.setButtonText("export");
    
	addAndMakeVisible(&bgFrame);
	bgFrame.setSliderStyle(Slider::IncDecButtons);
    bgFrame.addListener(this);
    
    addAndMakeVisible(format);
	format.addItem("jpeg",3);
    format.addItem("png",2);
    format.setSelectedId(3);
    format.setEnabled(false);
    
    pluginFormats.addDefaultFormats();
    
    changeListenerCallback(skin);
}

SkinProps::~SkinProps()
{
    if(skin)
    {
        skin->removeChangeListener(this);
    }
    this->testWindow = nullptr; //close the window ahead of time to prevent clash with skin;
    this->testInstance = nullptr;
    
    pluginUI.deleteAndZero();
    plugin = nullptr;
}

void SkinProps::resized()
{
    int w = getWidth();
    int w2 = w/2;
    int y = 0;
    int ch = 20;
	int x = 0;
    
    name.setBounds(x, y, w, ch);
    y+= name.getHeight();
    
    imagePath.setBounds(x,y,w,ch);
	y += ch;
    targetPath.setBounds(x,y,w,ch);
	y += ch;
    
    screenRatio.setBounds(x, y, w, ch);
    y += screenRatio.getHeight();
    
    preview.setBounds(x, y, w, ch);
    y+=ch;
    
    maskOpacity.setBounds(x, y, w, ch);
    y += ch;
    
    bgFrame.setBounds(x,y,w2,ch);
	fitBoundsToImage.setBounds(w2,y,w2,ch);
	y += ch;
    
    format.setBounds(x,y,w/2,ch);
    quality.setBounds(format.getRight(), y,w/2, ch);
    y += ch;
    
    exportFile.setBounds(x,y,w/2,ch);
    test.setBounds(exportFile.getRight(), y, w/2, ch);
    y += ch;
    
    loadPlugin.setBounds(x, y, getWidth(), ch);
    
    y+= ch;
    showPlugin.setBounds(x, y, getWidth(), ch);
}

void SkinProps::filenameComponentChanged (FilenameComponent* fileComponentThatHasChanged)
{
	if(fileComponentThatHasChanged == &imagePath && skin->getSourceImagePath() != fileComponentThatHasChanged->getCurrentFile())
	{
		skin->sourceImagePath = fileComponentThatHasChanged->getCurrentFile();
		skin->loadImages();
	}
	else if(fileComponentThatHasChanged == &targetPath)
	{
		skin->targetPath = fileComponentThatHasChanged->getCurrentFile();
    
	}
}

void SkinProps::changeListenerCallback(ChangeBroadcaster* obj)
{
    screenRatio.setValue(skin->getScreenRatio());
    bgFrame.setRange(0,skin->getSourceImages().size()-1,1);
    bgFrame.setValue(skin->backgroundframe.getValue(),NotificationType::dontSendNotification);
    maskOpacity.setValue(skin->maskOpacity.getValue(),NotificationType::dontSendNotification);
}

double SkinProps::getScale()const
{
    const double scale = -1;//this->previewSize.getValue() / this->skin->graphicArea.getWidth();
    return scale;
}

void SkinProps::comboBoxChanged(ComboBox* box)
{
    
}

void SkinProps::buttonClicked(Button* button)
{
	if(button == &fitBoundsToImage && skin->getSourceImages().size())
	{
		Image img =  skin->getSourceImages().getFirst();
		skin->graphicArea = img.getBounds();
	}
	else if(button == &test)
	{
        if(targetPath.getCurrentFile().exists())
        {
            if(this->testWindow)
            {
                this->testWindow->closeButtonPressed();
            }
            
            File f = targetPath.getCurrentFile().getChildFile("skin.d3ckskin");
            
            if( ! this->skin->isFileEqualToState(f) )
            {
                ScopedPointer<JPEGImageFormat> iformat = new JPEGImageFormat();
                iformat->setQuality(this->quality.getValue());
                skin->createSkin(this->targetPath.getCurrentFile(),*iformat);
            }
            
            this->testInstance = this->plugin ? new AudioProcessorSkin(this->plugin) : new Skin();
            
            ImageCache::releaseUnusedImages();
            
            this->testInstance->loadFromFile(f);
            
            this->testInstance->scaleToScreen();
            
            SkinGUI* g = new SkinGUI(this->testInstance);
            this->testWindow = new RenderSkinPreviewWindow(g,this->testWindow);
            testWindow->setName("test");
            this->testWindow->setVisible(true);
            this->testWindow->toFront(true);
        }
        else
        {
            AlertWindow::showMessageBox(AlertWindow::InfoIcon, "select export path", "select eport path");
        }
	}
	else if(button == &exportFile)
	{
        ScopedPointer<JPEGImageFormat> iformat = new JPEGImageFormat();
        iformat->setQuality(this->quality.getValue());
		skin->createSkin(this->targetPath.getCurrentFile(),*iformat);
	}
    else if(button == &preview)
    {
        if(this->previewWindow == nullptr)
        {
            SkinGUI* g = new SkinPreviewGUI(this->skin);
            this->previewWindow = new RenderSkinPreviewWindow(g,this->previewWindow);
        }
        this->previewWindow->setVisible(true);
        this->previewWindow->toFront(true);
    }
    else if(button == &this->loadPlugin)
    {
        PopupMenu menu;
        menu.addItem(1, "open...");
        this->knownPlugins.addToMenu(menu, KnownPluginList::sortByManufacturer);
        int res = menu.show();
        if(res == 1)
        {
            WildcardFileFilter wildcardFilter ("*.vst", String::empty, "vst files");
            FileBrowserComponent browser (FileBrowserComponent::canSelectDirectories | FileBrowserComponent::openMode,
                                          File("~/Library/Audio"),
                                          &wildcardFilter,
                                          nullptr);
            FileChooserDialogBox dialogBox ("Open vst file",
                                            "Please choose some vst you want to open...",
                                            browser,
                                            false,
                                            Colours::lightgrey);
            if (dialogBox.show())
            {
                File selectedFile = browser.getSelectedFile(0);
                StringArray files;
                files.add(selectedFile.getFullPathName());
                OwnedArray<PluginDescription> types;
                this->knownPlugins.scanAndAddDragAndDroppedFiles(this->pluginFormats, files, types);
                if(types.size() == 1)
                {
                    this->createPlugin(types.getFirst());
                }
            }
        }
        else
        {
            int index = this->knownPlugins.getIndexChosenByMenu(res);
            PluginDescription* desc = this->knownPlugins.getType(index);
            if(desc)
            {
                this->createPlugin(desc);
            }
        }
    }
    else
    {
        this->openPluginEditor();
    }
}

void SkinProps::clearPlugin()
{
    if(this->plugin)
    {
        this->plugin->releaseResources();
        this->plugin = nullptr;
    }
}

void SkinProps::openPluginEditor()
{
    if(this->plugin)
    {
        if(!this->pluginUI)
        {
            this->pluginUI = new PluginWindow(this->plugin->createEditorIfNeeded());
        }
        this->pluginUI->setVisible(true);
        this->pluginUI->toFront(true);
    }
}

void SkinProps::createPlugin(PluginDescription* desc)
{
    String errors;
    
    this->clearPlugin();
    
    double sampleRate = 44100;
    int numChannels = 2;
    int bufferSize = 128;
    
    this->plugin = this->pluginFormats.createPluginInstance(*desc, sampleRate, bufferSize, errors);
    if(this->plugin)
    {
        this->plugin->setPlayConfigDetails(numChannels, numChannels, sampleRate, bufferSize);
        this->plugin->prepareToPlay(sampleRate, bufferSize);
        
        this->openPluginEditor();
    }
}

void SkinProps::sliderValueChanged(Slider*slider)
{
	if(slider == &bgFrame)
	{
		skin->backgroundframe = bgFrame.getValue();
		skin->sendChangeMessage();
	}
    else if(slider == &quality)
    {
        this->skin->setQuality(slider->getValue());
    }
    else if(slider == &this->screenRatio)
    {
        const double ration = slider->getValue();
        this->skin->setScreenRatio(ration);
        this->skin->scaleToScreen();
    }
    else if(slider == &this->maskOpacity )
    {
        this->skin->maskOpacity = slider->getValue();
    }
}

PluginWindow::PluginWindow(Component* ed,bool owned):
DocumentWindow(ProjectInfo::projectName,Colours::white,DocumentWindow::allButtons)
{
    if(owned)
    {
        this->setContentOwned(ed, true);
    }
    else
    {
        this->setContentNonOwned(ed,true);
    }
}
PluginWindow::~PluginWindow()
{
}

void PluginWindow::closeButtonPressed()
{
    delete this;
}