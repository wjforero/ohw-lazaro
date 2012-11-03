#ifndef STATUSBAR_H
#define STATUSBAR_H

#ifndef FXHORIZONTALFRAME_H
#include "FXHorizontalFrame.h"
#endif


#ifndef STATUSLINE_H
#include "statusline.h"
#endif


/** Linea de status bar
*/
class Statusbar : public FXStatusbar {
  FXDECLARE(Statusbar)
protected:
  Statusbar(){}  
private:
  Statusbar(const Statusbar&);
  Statusbar& operator=(const Statusbar&);
  static const FXchar defaultMessageSt[];
public:  
  Statusbar(FXComposite* p,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=3,FXint pr=3,FXint pt=2,FXint pb=2,FXint hs=4,FXint vs=0);
public:
  virtual void setBackColor(FXColor clr);
	virtual void setTextColor(FXColor clr);
  void setFont(FXFont* fnt,FXFont* fnttitle);
};
 
FXDEFMAP(Statusbar) StatusbarMap[]={
  FXMAPFUNC(SEL_PAINT,0,Statusbar::onPaint)
};

const FXchar Statusbar::defaultMessageSt[]="Listo.";

FXIMPLEMENT(Statusbar,FXStatusbar,0,0)


// Make a status bar
Statusbar::Statusbar(FXComposite* p,FXuint opts,FXint x,FXint y,FXint w,FXint h,FXint pl,FXint pr,FXint pt,FXint pb,FXint hs,FXint vs):
  FXStatusbar(p,opts,x,y,w,h,pl,pr,pt,pb,hs,vs){
  corner=new FXDragCorner(this);
  status=new Statusline(this);
  status->setFrameStyle(FRAME_NONE);
}

void Statusbar::setBackColor(FXColor clr){
  FXStatusbar::setBackColor(clr);
  status->setBackColor(clr);
}

void Statusbar::setTextColor(FXColor clr){
  status->setTextColor(clr);  
}


void Statusbar::setFont(FXFont* fnt,FXFont* fnttitle){
  status->setFont(fnt);  
  ((Statusline*)status)->setFontTitle(fnttitle);
}




#endif