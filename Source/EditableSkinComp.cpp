#include "EditableSkinComp.h"
#include "EditableSkin.h"

EditableSkinComp::EditableSkinComp(EditableSkin* skin):
SkinComp(skin)
{
    this->skin = skin;
    
    this->compType = SkinComp::slider;
    
    this->useFullFrameRange = false;
    frames.setLength(skin->getSourceImages().size()-1);
}

EditableSkinComp::~EditableSkinComp()
{
    masterReference.clear();
}

EditableSkin* EditableSkinComp::getSkin()const
{
    return this->skin;
}

double EditableSkinComp::getGradient()const
{
    return gradient.getValue();
}

void EditableSkinComp::setGradient(double g)
{
    this->gradient = g;
    this->sendChangeMessage();
}

XmlElement* EditableSkinComp::createXml()
{
	XmlElement* e2 = new XmlElement("comp");
    
	e2->setAttribute("interpolation",(bool)interpolation.getValue());
	e2->setAttribute("usecontrollarea",(bool)useControllArea.getValue());
	e2->setAttribute("id",getName());
	e2->setAttribute("image",getStripFile( String(skin->getComps().items.indexOf(this)+10) ).getFileName());
	e2->setAttribute("start",getRange().getStart());
	e2->setAttribute("end",getRange().getEnd());
	e2->setAttribute("graphicarea",graphicArea.toString());
	e2->setAttribute("controllarea",controllArea.toString());
	e2->setAttribute("type",compType);
	e2->setAttribute("clip",cliptype);
	e2->setAttribute("sensitivity",(double)sensitivity.getValue());
    e2->setAttribute("help", this->helpText);
    e2->setAttribute("title", this->title);
    e2->setAttribute("fullRange",(bool)this->useFullFrameRange.getValue());
    e2->setAttribute("parameterIndex", this->getParameterIndex());
    e2->setAttribute("gradient", this->getGradient());

	return e2;
}

void EditableSkinComp::setFromXml(XmlElement* el)
{
    this->useFullFrameRange = el->getBoolAttribute("fullRange",false);
    SkinComp::setFromXml(el);
}

void EditableSkinComp::setFirtFrame(int s)
{
    this->frames.setStart(s);
}

void EditableSkinComp::setLastFrame(int e)
{
    this->frames.setEnd(e);
}

void EditableSkinComp::setRange(int start,int end)
{
    this->frames.setStart(start);
    this->frames.setEnd(end);
}

Range<int> EditableSkinComp::getRange()const
{
    return this->useFullFrameRange.getValue() ? Range<int>(0,skin->getSourceImages().size()-1) : SkinComp::getRange();
}