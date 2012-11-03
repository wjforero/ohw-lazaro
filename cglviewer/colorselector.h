#ifndef COLOR_SELECTOR_H
#define COLOR_SELECTOR_H

#ifndef FXPACKER_H
#include "FXPacker.h"
#endif

#include "fximagebutton.h"


class FXColorBar;
class FXColorWell;
class FXColorWheel;
class FXImageButton;

/** Control para la selección del color
*/
class ColorSelector : public FXPacker {
  FXDECLARE(ColorSelector)
protected:
  FXColorWell  *well;                 
  FXColorBar   *bar;                  
  FXColorWheel *wheel;                
  FXfloat       rgba[4];              
  FXfloat       hsva[4];              
  FXImageButton* accept;
  FXImageButton* cancel;
public:
  enum {    
    ID_DIAL_WHEEL=FXPacker::ID_LAST,
    ID_COLOR_BAR,
    ID_COLOR,
    ID_WELL_CHANGED
    };
protected:
  ColorSelector(){}
  void updateWell();
private:
  ColorSelector(const ColorSelector&);
  ColorSelector &operator=(const ColorSelector&);
public:  
  long onCmdWheel(FXObject*,FXSelector,void*);
  long onUpdWheel(FXObject*,FXSelector,void*);
  long onCmdBar(FXObject*,FXSelector,void*);
  long onUpdBar(FXObject*,FXSelector,void*);  
  long onCmdWell(FXObject*,FXSelector,void*);  
  long onChgWell(FXObject*,FXSelector,void*); 
  long onCmdColor(FXObject*,FXSelector,void*);
  long onChgColor(FXObject*,FXSelector,void*);
  
public:
  
  ColorSelector(FXComposite *p,FXObject* tgt=NULL,FXSelector sel=0,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0,Skin* skin=0);  

  FXImageButton* acceptButton();
  FXImageButton* cancelButton();

  void setRGBA(FXColor clr);
  
  FXColor getRGBA() const;
  FXbool isOpaqueOnly() const;
  void setOpaqueOnly(FXbool opaque);

  long onMouseDown(FXObject*,FXSelector,void*);
  long onMouseUp(FXObject*,FXSelector,void*);
	long onMouseMove(FXObject*,FXSelector,void*);

private:
  int mdflag;
  int vx;
  int vy;
};

FXDEFMAP(ColorSelector) ColorSelectorMap[]={    
  FXMAPFUNC(SEL_CHANGED,ColorSelector::ID_COLOR,ColorSelector::onChgColor),
  FXMAPFUNC(SEL_COMMAND,ColorSelector::ID_COLOR,ColorSelector::onCmdColor),
  FXMAPFUNC(SEL_COMMAND,ColorSelector::ID_DIAL_WHEEL,ColorSelector::onCmdWheel),
  FXMAPFUNC(SEL_CHANGED,ColorSelector::ID_DIAL_WHEEL,ColorSelector::onCmdWheel),
  FXMAPFUNC(SEL_UPDATE,ColorSelector::ID_DIAL_WHEEL,ColorSelector::onUpdWheel),
  FXMAPFUNC(SEL_COMMAND,ColorSelector::ID_COLOR_BAR,ColorSelector::onCmdBar),
  FXMAPFUNC(SEL_CHANGED,ColorSelector::ID_COLOR_BAR,ColorSelector::onCmdBar),
  FXMAPFUNC(SEL_UPDATE,ColorSelector::ID_COLOR_BAR,ColorSelector::onUpdBar),  
  FXMAPFUNC(SEL_COMMAND,ColorSelector::ID_WELL_CHANGED,ColorSelector::onCmdWell),
  FXMAPFUNC(SEL_CHANGED,ColorSelector::ID_WELL_CHANGED,ColorSelector::onChgWell),
  FXMAPFUNC(SEL_LEFTBUTTONPRESS,   0, ColorSelector::onMouseDown),
	FXMAPFUNC(SEL_LEFTBUTTONRELEASE, 0, ColorSelector::onMouseUp),
	FXMAPFUNC(SEL_MOTION,            0, ColorSelector::onMouseMove)
};


FXIMPLEMENT(ColorSelector,FXPacker,ColorSelectorMap,ARRAYNUMBER(ColorSelectorMap))

ColorSelector::ColorSelector(FXComposite *p,FXObject* tgt,FXSelector sel,FXuint opts,FXint x,FXint y,FXint w,FXint h,Skin* skin)
:FXPacker(p,opts,x,y,w,h){  
  FXHorizontalFrame * frameWell=new FXHorizontalFrame(this,LAYOUT_SIDE_TOP|LAYOUT_CENTER_X,0,0,0,0, 0,0,0,0);
  well=new FXColorWell(frameWell,FXRGBA(255,255,255,255),this,ID_WELL_CHANGED,COLORWELL_SOURCEONLY|LAYOUT_FIX_HEIGHT|LAYOUT_FIX_WIDTH,0,0,0,0);

  FXHorizontalFrame * frameWhellBar=new FXHorizontalFrame(this,LAYOUT_CENTER_X,0,0,0,0, 0,0,0,0);
  wheel=new FXColorWheel(frameWhellBar,this,ID_DIAL_WHEEL,LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT|LAYOUT_LEFT,0,0,50,50);
  bar=new FXColorBar(frameWhellBar,this,ID_COLOR_BAR,COLORBAR_VERTICAL|LAYOUT_FIX_HEIGHT|LAYOUT_FIX_WIDTH|LAYOUT_RIGHT,0,0,10,50,1,1,1,1);
	wheel->setBackColor(skin->getBackColorDialog());
	bar->setBackColor(skin->getBackColorDialog());
	frameWhellBar->setBackColor(skin->getBackColorDialog());

  FXHorizontalFrame *buttons=new FXHorizontalFrame(this,LAYOUT_CENTER_X,0,0,0,0, 0,0,0,0);

  accept=new FXImageButton(buttons,"\tCOLOR\tColor\nAceptar",0,this,LAYOUT_LEFT,0,0,20,20);
	accept->setKeySkin("butokcolorselector");
	accept->setValSkin(skin);	
  cancel=new FXImageButton(buttons,"\tCOLOR\tColor\nCancelar", 0 ,this,0,LAYOUT_RIGHT,0,0,20,20);
	cancel->setKeySkin("butcancelcolorselector");
	cancel->setValSkin(skin);	
	cancel->setBackColor(skin->getBackColorDialog());
	buttons->setBackColor(skin->getBackColorDialog());
	setBackColor(skin->getBackColorDialog());
	
  rgba[0]=0.0;
  rgba[1]=0.0;
  rgba[2]=0.0;
  rgba[3]=1.0;
  
  hsva[0]=360.0;
  hsva[1]=0.0;
  hsva[2]=0.0;
  hsva[3]=1.0;

  updateWell();
}

long ColorSelector::onCmdWheel(FXObject*,FXSelector sel,void*){
  hsva[0]=wheel->getHue();
  hsva[1]=wheel->getSat();
  fxhsv_to_rgb(rgba[0],rgba[1],rgba[2],hsva[0],hsva[1],hsva[2]);
  updateWell();
  handle(this,MKUINT(ID_COLOR,SELTYPE(sel)),(void*)well->getRGBA());
  return 1;
}

long ColorSelector::onUpdWheel(FXObject*,FXSelector,void*){
  wheel->setHue(hsva[0]);
  wheel->setSat(hsva[1]);
  return 1;
}

long ColorSelector::onCmdBar(FXObject*,FXSelector sel,void*){
  hsva[0]=bar->getHue();
  hsva[1]=bar->getSat();
  hsva[2]=bar->getVal();
  fxhsv_to_rgb(rgba[0],rgba[1],rgba[2],hsva[0],hsva[1],hsva[2]);
  updateWell();  
  handle(this,MKUINT(ID_COLOR,SELTYPE(sel)),(void*)well->getRGBA());
  return 1;
}

long ColorSelector::onUpdBar(FXObject*,FXSelector,void*){
  bar->setHue(hsva[0]);
  bar->setSat(hsva[1]);
  bar->setVal(hsva[2]);
  return 1;
}

long ColorSelector::onChgWell(FXObject*,FXSelector,void* ptr){
  FXColor color=(FXColor)(long)ptr;
  if(well->isOpaqueOnly()) color|=FXRGBA(0,0,0,255);
  rgba[0]=0.003921568627f*FXREDVAL(color);
  rgba[1]=0.003921568627f*FXGREENVAL(color);
  rgba[2]=0.003921568627f*FXBLUEVAL(color);
  rgba[3]=0.003921568627f*FXALPHAVAL(color);
  fxrgb_to_hsv(hsva[0],hsva[1],hsva[2],rgba[0],rgba[1],rgba[2]);
  hsva[3]=rgba[3];
  return 1;
}


long ColorSelector::onCmdWell(FXObject*,FXSelector,void*){
  handle(this,MKUINT(ID_COLOR,SEL_COMMAND),(void*)well->getRGBA());
  return 1;
}


void ColorSelector::updateWell(){
  well->setRGBA(FXRGBA((int)(rgba[0]*255.0),(int)(rgba[1]*255.0),(int)(rgba[2]*255.0),(int)(rgba[3]*255.0)));
}

FXImageButton* ColorSelector::acceptButton(){
  return accept;
}

FXImageButton* ColorSelector::cancelButton(){
  return cancel;
}

void ColorSelector::setRGBA(FXColor clr){
  if(clr!=well->getRGBA()){
    rgba[0]=0.003921568627f*FXREDVAL(clr);
    rgba[1]=0.003921568627f*FXGREENVAL(clr);
    rgba[2]=0.003921568627f*FXBLUEVAL(clr);
    rgba[3]=0.003921568627f*FXALPHAVAL(clr);
    fxrgb_to_hsv(hsva[0],hsva[1],hsva[2],rgba[0],rgba[1],rgba[2]);
    hsva[3]=rgba[3];
    well->setRGBA(clr);
    }
  }

FXColor ColorSelector::getRGBA() const {
  return well->getRGBA();
}

FXbool ColorSelector::isOpaqueOnly() const {
  return well->isOpaqueOnly();
  }


void ColorSelector::setOpaqueOnly(FXbool opaque){
  if(opaque){
    well->setOpaqueOnly(TRUE);
    setRGBA(well->getRGBA() | FXRGBA(0,0,0,255));
    }
  else{
    well->setOpaqueOnly(FALSE);
    }
  }

long ColorSelector::onCmdColor(FXObject*,FXSelector,void*){
  if(target)   
    target->handle(this,MKUINT(message,SEL_COMMAND),(void*)well->getRGBA());  
  return 1;
}

long ColorSelector::onChgColor(FXObject*,FXSelector,void*ptr){
  if(target) target->handle(this,MKUINT(message,SEL_CHANGED),(void*)well->getRGBA());
  return 1;
}

long ColorSelector::onMouseDown(FXObject* sender,FXSelector sel,void*ptr){
  if(getParent() && getParent()->handle(sender,sel,ptr)) return 1;   
	return 1;
}

long ColorSelector::onMouseMove(FXObject* sender, FXSelector sel, void* ptr){
  if(getParent() && getParent()->handle(sender,sel,ptr)) return 1; 
  return 1;
}


long ColorSelector::onMouseUp(FXObject* sender,FXSelector sel,void* ptr){	
  if(getParent() && getParent()->handle(sender,sel,ptr)) return 1; 
  return 1;
}

  
#endif