#ifndef STATUSLINE_H
#define STATUSLINE_H

#ifndef FXSTATUSLINE_H
#include "FXStatusline.h"
#endif

/** Modela una linea de estatus
*/
class Statusline : public FXStatusline {
  FXDECLARE(Statusline)
public:
  Statusline(FXComposite* p,FXObject* tgt=NULL,FXSelector sel=0);
  virtual long onPaint(FXObject*,FXSelector,void* ptr);
  void setFontTitle(FXFont* fnt);
	void setTextColor(FXColor clr);
protected:
  Statusline(){}
  FXFont   *m_fontTitle;               // Font
private:
  Statusline(const Statusline&);
  Statusline& operator=(const Statusline&);
  
};

FXDEFMAP(Statusline) StatuslineMap[]={
  FXMAPFUNC(SEL_PAINT,0,Statusline::onPaint)
};

FXIMPLEMENT(Statusline,FXStatusline,StatuslineMap,ARRAYNUMBER(StatuslineMap))

Statusline::Statusline(FXComposite* p,FXObject* tgt,FXSelector sel):
  FXStatusline(p,tgt,sel){
  status="Listo.";
  normal="Listo.";
  m_fontTitle=0;
}

long Statusline::onPaint(FXObject*,FXSelector,void* ptr){  	
  FXEvent *ev=(FXEvent*)ptr;
  FXDCWindow dc(this,ev);
  FXint ty=padtop+(height-padtop-padbottom-font->getFontHeight())/2;  
  FXint pos,len;
  dc.setForeground(backColor);	
  FXint wt=0;
  dc.fillRectangle(ev->rect.x,ev->rect.y,ev->rect.w,ev->rect.h);
  if(!status.empty()){
    dc.setTextFont(font);
    pos=status.findf('\n');
    len=status.length();
    if(pos>=0){			
      if(m_fontTitle!=0)
        dc.setTextFont(m_fontTitle);
      dc.setForeground(textColor);
      dc.drawText(padleft,ty+font->getFontAscent(),status.text(),pos);
      wt=font->getFontAscent();
      dc.setTextFont(font);
      dc.setForeground(textColor);
      dc.drawText(padleft,ty+3*wt,status.text()+pos+1,len-pos-1);
    }
    else{
      dc.setForeground(textColor);
      dc.drawText(padleft,ty+font->getFontAscent(),status.text(),len);
      }
    }
  drawFrame(dc,0,0,width,height);
  return 1;
}

void Statusline::setFontTitle(FXFont* fnt){
  m_fontTitle=fnt;
  m_fontTitle->create();
}


void Statusline::setTextColor(FXColor clr){
	textColor=clr;
	textHighlightColor=clr;
}


#endif

