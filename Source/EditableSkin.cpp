#include "EditableSkin.h"
#include "EditableSkinComp.h"
#include "RenderSkin.h"

EditableSkin::EditableSkin(RenderSkin* app)
{
    this->app = app;
    setName("new skin");
    backgroundframe = 0;
    maskOpacity = 0;
    
    maskOpacity.addListener(this);
    //    backgroundframe.addListener(this);
}

EditableSkin::~EditableSkin()
{
    masterReference.clear();
}

void EditableSkin::loadFromXml(XmlElement *el)
{
    backgroundframe = el->getIntAttribute("backgroundframe",(int)backgroundframe.getValue());
    sourceImagePath = el->getStringAttribute("sourceimagepath");
    targetPath = el->getStringAttribute("targetpath");
    quality = el->getDoubleAttribute("quality",0.7);
    
    loadImages();
    
    Skin::loadFromXml(el);
}

RenderSkin* EditableSkin::getApp()const
{
    return this->app;
}

void EditableSkin::deleteSelectedComps(D3CKHistory* history)
{
    for(int i = this->comps.selectedItems.getNumSelected() ; -- i >= 0 ; )
    {
        ListItem* item = this->comps.selectedItems.getSelectedItem(i);
        item->moveToTrash(history,history);
    }
}

void EditableSkin::valueChanged (Value& value)
{
    for(int i = 0 ; i < comps.getNumRows() ; ++i)
    {
        comps.items.getUnchecked(i)->sendChangeMessage();
    }
    this->sendChangeMessage();
}

void EditableSkin::setQuality(double quality)
{
    if(this->quality != quality)
    {
        this->quality = quality;
        this->sendChangeMessage();
    }
}

double EditableSkin::getQuality()const
{
    return this->quality.getValue();
}

File EditableSkin::getSourceImagePath()const
{
    return this->sourceImagePath;
}

File EditableSkin::getTargetPath() const
{
    return this->targetPath;
}

void EditableSkin::loadImages()
{
	sourceImages.clear();
	DirectoryIterator it(sourceImagePath,false);
	while(it.next() )
	{
        File file = it.getFile();
        {
            Image img = Skin::getFromFileOrMemory(file);
            if(img.isValid())
            {
                sourceImages.add(img);
            }
        }
	}
	sendChangeMessage();
}

SkinComp* EditableSkin::createComp()
{
    return new EditableSkinComp(this);
}

int EditableSkin::getWidth()const
{
    return this->graphicArea.getWidth();
}

int EditableSkin::getHeight()const
{
    return this->graphicArea.getHeight();
}

XmlElement* EditableSkin::createXml()const
{
	XmlElement* el = new XmlElement("skin");
	if(getComps().items.size())
	{
		XmlElement* e1  = new XmlElement("comps");
		el->addChildElement(e1);
		for(int i = 0 ; i  < getComps().items.size() ; i++)
		{
            EditableSkinComp* c = dynamic_cast<EditableSkinComp*>(getComps().items.getUnchecked(i));
            jassert(c);
			e1->addChildElement(c->createXml());
		}
	}
	el->setAttribute("bounds",graphicArea.toString());
	el->setAttribute("name",getName());
	el->setAttribute("sourceimagepath",sourceImagePath.getFullPathName());
	el->setAttribute("targetpath",targetPath.getFullPathName());
	el->setAttribute("backgroundframe",(int)backgroundframe.getValue());
    el->setAttribute("quality",getQuality());
    
    el->setAttribute("screenRatio", (double)this->screenRatio.getValue());
    
	return el;
}

bool EditableSkin::isExported() const
{
    jassertfalse;
    return false;//isFileEqualToState(this->getFile());
}

bool EditableSkin::isSaved() const
{
    return isFileEqualToState(this->getFile());
}

bool EditableSkin::save()
{
    if(!this->file.existsAsFile())
    {
        FileChooser fc("save skin",File::nonexistent,"*.d3ckskin");
        if(fc.browseForFileToSave(true))
        {
            this->file = fc.getResult().withFileExtension("d3ckskin");
            this->file.create();
        }
        else
        {
            return false;
        }
    }
    if(this->file.existsAsFile())
    {
        ScopedPointer<XmlElement> el = this->createXml();
        el->writeToFile(this->file,"");
        return true;
    }
    
    return false;
}

bool EditableSkin::isFileEqualToState(const File& file)const
{
    if(file.existsAsFile())
    {
        ScopedPointer<XmlElement> el = XmlDocument::parse(file);
        ScopedPointer<XmlElement> el2 = this->createXml();
        
        return el->createDocument("") == el2->createDocument("");
    }
    else
    {
        return false;
    }
}

bool EditableSkin::attemptToClose()
{
    if(!this->isSaved())
    {
        int res = AlertWindow::showYesNoCancelBox(AlertWindow::QuestionIcon, "save changes to "+this->getName()+"?", "save changes to "+ this->getName() +"?");
        if(res == 1)
        {
            return this->save();
        }
        else
        {
            return res == 2;// no, do not save
        }
    }
    else
    {
        return true;
    }
}

const Array<Image>& EditableSkin::getSourceImages() const
{
    return this->sourceImages;
}

void EditableSkin::produce()
{
    JPEGImageFormat f;
    f.setQuality(this->getQuality());
    this->createSkin(this->targetPath,f );
}


void EditableSkin::createSkin(const File& targetDir,ImageFileFormat& format)
{
    
	if(targetPath.create());
	{
        const String skinFileName = "skin.d3ckskin";//this->getFile().getFileName();
        const File skinFile = targetPath.getChildFile(skinFileName);
        
		File file(targetPath.getChildFile("background."+ Skin::getImageFormatEnding(&format)));
		file.deleteFile();
		FileOutputStream fo(file);
        
		Image bg(Image::RGB,graphicArea.getWidth(),graphicArea.getHeight(),false);
		Graphics g(bg);
		g.fillAll(Colours::black);
		g.drawImage(sourceImages[backgroundframe.getValue()],0,0,graphicArea.getWidth(),graphicArea.getHeight(),graphicArea.getX(),graphicArea.getY(),graphicArea.getWidth(),graphicArea.getHeight());
		format.writeImageToStream(bg,fo);
        
		fo.flush();
        
		XmlElement* el = createXml();
		el->writeToFile(skinFile,"");
		delete el;
        
		for(int i = getComps().items.size() ; -- i >= 0 ;)
		{
            
			EditableSkinComp* c = dynamic_cast<EditableSkinComp*>(getComps().items.getUnchecked(i));
            jassert(c) //an editable skin should only hold editbale skin comps
			
            if(!c->graphicArea.isEmpty() && c->getName().isNotEmpty())
            {
                int x = c->graphicArea.getX();
                int y = c->graphicArea.getY();
                int h = c->graphicArea.getHeight();
                int w = c->graphicArea.getWidth();
                int len = c->getRange().getLength()+1;
                Image img(Image::RGB, w, h* len ,true);
                Graphics g(img);
                
                int len2 = c->getRange().getLength();
                for(int im = 0 ; im <= len2 ; im++)
                {
                    Image si = sourceImages.getUnchecked(jlimit(0, sourceImages.size()-1,im+c->getRange().getStart()));
                    g.drawImage(si,0,im*h,w,h,x,y,w,h);
                }
                File file = c->getStripFile(String((i+10)));
                file.deleteFile();
                FileOutputStream fo(file);
                format.writeImageToStream(img,fo);
                
            }
            
		}
	}
}