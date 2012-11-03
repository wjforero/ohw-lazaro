#ifndef BUTTONROTATE_H
#define BUTTONROTATE_H

#include "fx.h"
#include "fx3d.h"
#include "fximagebutton.h"

/** Boton para rotación
*/
class ButtonRotate : public FXImageButton{
  FXDECLARE(ButtonRotate)
protected:
  ButtonRotate();	
public:
	enum{
		ROTATING=1
	};
public:
	long onLeftBtnPress(FXObject*,FXSelector,void*);
  long onLeftBtnRelease(FXObject*,FXSelector,void*);
	long onRotate(FXObject*,FXSelector,void*);
	
public:  
  ButtonRotate(FXGLViewer* viewer,FXComposite* p, const FXString& text,FXIcon* icon, FXObject* tgt=NULL, FXSelector sel=0,FXuint opts=LAYOUT_EXPLICIT, FXint x=0, FXint y=0, FXint w=0, FXint h=0);	
protected:	
	FXGLViewer* m_viewer;
	int m_curAntx;
  int m_curAnty;
	unsigned int m_option;
};

#endif