#ifndef GUIFRAME_H
#define GUIFRAME_H

#include "fx.h"

/** Modela un frame contenedor
*/
class Frame :  public FXVerticalFrame {
  FXDECLARE(Frame)
protected:
  Frame(){};
public:
  Frame(FXComposite *p,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=DEFAULT_SPACING,FXint pr=DEFAULT_SPACING,FXint pt=DEFAULT_SPACING,FXint pb=DEFAULT_SPACING,FXint hs=DEFAULT_SPACING,FXint vs=DEFAULT_SPACING);
  virtual ~Frame(void);
public:
  long onPaint(FXObject*, FXSelector, void*);
};

FXDEFMAP(Frame) FrameMap[]={
  FXMAPFUNC(SEL_PAINT,0,Frame::onPaint),
  };


FXIMPLEMENT(Frame,FXVerticalFrame,FrameMap,ARRAYNUMBER(FrameMap))

Frame::Frame(FXComposite *p,FXuint opts,FXint x,FXint y,FXint w,FXint h,FXint pl,FXint pr,FXint pt,FXint pb,FXint hs,FXint vs)
:FXVerticalFrame(p,opts,x,y,w,h,pl,pr,pt,pb,hs,vs)
{
}

Frame::~Frame(void)
{
}

long Frame::onPaint(FXObject* obj, FXSelector sel, void* ptr) {
  setBackColor(FXRGB(0,0,0));
  FXVerticalFrame::onPaint(obj, sel, ptr);
  return 1;
}


#endif