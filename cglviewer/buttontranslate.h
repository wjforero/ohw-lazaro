#ifndef BUTTONTRANSLATE_H
#define BUTTONTRANSLATE_H

#include "fx.h"
#include "fx3d.h"
#include "fximagebutton.h"

class ButtonTranslate : public FXImageButton{
  FXDECLARE(ButtonTranslate)
protected:
  ButtonTranslate();
public:
	enum{
    TRANSLATING=1,
		TRANSLATINGX,
		TRANSLATINGY,
		TRANSLATINGXY,
		TRANSLATINGZ
  };
public:	
	long onLeftBtnPress(FXObject*,FXSelector,void*);
  long onLeftBtnRelease(FXObject*,FXSelector,void*);
	virtual long onTranslate(FXObject*,FXSelector,void*);
	
public:  
  ButtonTranslate(FXGLViewer* viewer,FXComposite* p, const FXString& text,FXIcon* icon, FXObject* tgt=NULL, FXSelector sel=0,FXuint opts=LAYOUT_EXPLICIT, FXint x=0, FXint y=0, FXint w=0, FXint h=0);
	void setOptionTranslate(const unsigned int& optionTranslate);	
protected:
	int m_option;
	FXGLViewer* m_viewer;
	int m_curAntx;
  int m_curAnty;
	unsigned int m_optionTranslate;
};


#endif