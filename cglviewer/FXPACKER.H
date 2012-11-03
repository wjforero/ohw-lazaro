#ifndef COLORWELL_H
#define COLORWELL_H

#ifndef FXCOLORWELL_H
#include "FXColorWell.h"
#endif

#ifndef COLORSELECTOR_H
#include "colordialog.h"
#endif

class ColorWell : public FXColorWell {
  FXDECLARE(ColorWell)
protected:
  ColorWell();
private:
  ColorWell(const ColorWell&);
  ColorWell &operator=(const ColorWell&);
public:
  long onDoubleClicked(FXObject*,FXSelector,void*);  
  long onLeave(FXObject*,FXSelector,void*);
	long onQueryHelp(FXObject* sender,FXSelector,void*);	
	void setHelpText(const FXString& text);
public: 
  ColorWell(FXComposite* p,FXColor clr=0,FXObject* tgt=NULL,FXSelector sel=0,FXuint opts=COLORWELL_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=DEFAULT_PAD,FXint pr=DEFAULT_PAD,FXint pt=DEFAULT_PAD,FXint pb=DEFAULT_PAD);
private:
	FXString text;
};

FXDEFMAP(ColorWell) ColorWellMap[]={  
  FXMAPFUNC(SEL_RIGHTBUTTONPRESS,0,ColorWell::onDoubleClicked),
  FXMAPFUNC(SEL_DOUBLECLICKED,0,ColorWell::onDoubleClicked),  
	FXMAPFUNC(SEL_UPDATE,FXWindow::ID_QUERY_HELP,ColorWell::onQueryHelp),
};

// Change help text
void ColorWell::setHelpText(const FXString& text){
  help=text;
}

long ColorWell::onQueryHelp(FXObject* sender,FXSelector,void*){
  if(!help.empty() && (flags&FLAG_HELP)){
    sender->handle(this,MKUINT(ID_SETSTRINGVALUE,SEL_COMMAND),(void*)&help);
    return 1;
    }
  return 0;
  }



// Object implementation
FXIMPLEMENT(ColorWell,FXColorWell,ColorWellMap,ARRAYNUMBER(ColorWellMap))



ColorWell::ColorWell(){
  flags|=FLAG_ENABLED|FLAG_DROPTARGET;
  rgba=0;
  oldrgba=0;
  wellColor[0]=0;
  wellColor[1]=0;
}



ColorWell::ColorWell(FXComposite* p,FXColor clr,FXObject* tgt,FXSelector sel,FXuint opts,FXint x,FXint y,FXint w,FXint h,FXint pl,FXint pr,FXint pt,FXint pb)
  :FXColorWell(p,clr,tgt,sel,opts,x,y,w,h,pl,pr,pt,pb){
  flags|=FLAG_ENABLED|FLAG_DROPTARGET;
  target=tgt;
  message=sel;
  rgba=clr;
  oldrgba=clr;
  wellColor[0]=rgbaoverwhite(rgba);
  wellColor[1]=rgbaoverblack(rgba);
}

long ColorWell::onDoubleClicked(FXObject*,FXSelector,void*){
  if(target && target->handle(this,MKUINT(message,SEL_DOUBLECLICKED),(void*)rgba)) return 1;  
  if(options&COLORWELL_SOURCEONLY) return 1;  
  
  ColorDialog colordialog(this,"Colores",0);
  FXColor oldcolor=getRGBA();
  colordialog.setTarget(this);
  colordialog.setSelector(ID_COLORDIALOG);
  colordialog.setRGBA(oldcolor);
  colordialog.setOpaqueOnly(isOpaqueOnly());  
  


  // We canceled, so restore the old color
  if(!colordialog.execute()){
    handle(this,MKUINT(0,SEL_CHANGED),(void*)oldcolor);
    handle(this,MKUINT(0,SEL_COMMAND),(void*)oldcolor);
  }
  return 1;
}

#endif