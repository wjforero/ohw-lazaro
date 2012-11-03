#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#ifndef FXTEXTFIELD_H
#include "FXTextField.h"
#endif

/** Modela un campo de texto
*/
class TextField : public FXTextField , public SkinInterface{
  FXDECLARE(TextField)
public:
  TextField(FXComposite* p,FXint ncols,FXObject* tgt=NULL,FXSelector sel=0,FXuint opts=TEXTFIELD_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=DEFAULT_PAD,FXint pr=DEFAULT_PAD,FXint pt=DEFAULT_PAD,FXint pb=DEFAULT_PAD);
public:
  long onPaint(FXObject*,FXSelector,void*);
  long onFocusIn(FXObject*,FXSelector,void*);
  long onFocusOut(FXObject*,FXSelector,void*);

	virtual void setSkin(string key,Skin* skin);
protected:
  TextField(){} 
private:
  TextField(const TextField&);
  TextField& operator=(const TextField&);
  int m_valFocus;
	FXIcon* m_icon;
};

FXDEFMAP(TextField) TextFieldMap[]={
  FXMAPFUNC(SEL_PAINT,0,TextField::onPaint),
  FXMAPFUNC(SEL_FOCUSIN,0,TextField::onFocusIn),
  FXMAPFUNC(SEL_FOCUSOUT,0,TextField::onFocusOut)
};


FXIMPLEMENT(TextField,FXTextField,TextFieldMap,ARRAYNUMBER(TextFieldMap))

TextField::TextField(FXComposite* p,FXint ncols,FXObject* tgt,FXSelector sel,FXuint opts,FXint x,FXint y,FXint w,FXint h,FXint pl,FXint pr,FXint pt,FXint pb)
:FXTextField(p,ncols,tgt,sel,opts,x,y,w,h,pl,pr,pt,pb){
  m_valFocus=0;
	m_icon=0;
}


long TextField::onFocusIn(FXObject* sender,FXSelector sel,void* ptr){   
  FXFrame::onFocusIn(sender,sel,ptr);
  m_valFocus=1;  
  if(isEditable()){
    if(!blinker) 
      blinker=getApp()->addTimeout(getApp()->getBlinkSpeed(),this,ID_BLINK);
    drawCursor(FLAG_CARET);
    update();
  }
  if(hasSelection()){
    update(border,border,width-(border<<1),height-(border<<1));
    }
  return 1;
}

long TextField::onFocusOut(FXObject* sender,FXSelector sel,void* ptr){
  
  FXFrame::onFocusOut(sender,sel,ptr);
  m_valFocus=0;  
  if(blinker){    
    getApp()->removeTimeout(blinker);blinker=NULL;    
  }
  drawCursor(0);  
  update();
  if(hasSelection()){    
    update(border,border,width-(border<<1),height-(border<<1));
  }
  return 1;
}


long TextField::onPaint(FXObject*,FXSelector,void* ptr){
  FXEvent *ev=(FXEvent*)ptr;
  FXDCWindow dc(this,ev);
  
  if(m_valFocus==1){
    drawFrame(dc,0,0,width,height);
  }else{
    dc.fillRectangle(0,0,width,height);    
  }

  if(isEnabled())
    dc.setForeground(backColor);
  else
    dc.setForeground(baseColor);

  dc.fillRectangle(border,border,width-(border<<1),height-(border<<1));

  dc.setClipRectangle(border,border,width-(border<<1),height-(border<<1));
  drawTextRange(dc,0,contents.length());  

  if(flags&FLAG_CARET){
    int xx=coord(cursor)-1;
    dc.setForeground(textColor);
    dc.fillRectangle(xx,padtop+border,1,height-padbottom-padtop-(border<<1));
    dc.fillRectangle(xx-2,padtop+border,5,1);
    dc.fillRectangle(xx-2,height-border-padbottom-1,5,1);
  }  

  if(m_valFocus==0 && m_icon){
		dc.drawIcon(m_icon,0,0);
  }
  return 1;
}

void TextField::setSkin(string key,Skin* skin){
	m_skin=skin;
	m_icon=m_skin->getIcon(key);	
}

#endif