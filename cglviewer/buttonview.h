#ifndef BUTTONVIEW_H
#define BUTTONVIEW_H

#include "fx.h"
#include "fx3d.h"
#include "skininterface.h"

/** Se provee una clase boton con imagenenes para diferentes eventos
*/
class ButtonView : public FXButton, public SkinInterface{
  FXDECLARE(ButtonView)
protected:
  ButtonView();
public:
	enum{
    ID_CHANGESTATE=FXWindow::ID_LAST
  };

	enum{    
		ID_FRONT=0,
		ID_BACK,
		ID_LEFT,
		ID_RIGHT,
		ID_TOP,
		ID_BOTTOM
  };
public:
  long onPaint(FXObject*,FXSelector,void*);  	
	long onChangeState(FXObject* sender,FXSelector sel,void* ptr);

public:

  /** Construye un boton con un texto
  */
  ButtonView(FXIcon** icons,FXGLViewer* viewer,FXComposite* p, const FXString& text, FXObject* tgt=NULL, FXSelector sel=0,FXuint opts=LAYOUT_EXPLICIT, FXint x=0, FXint y=0, FXint w=0, FXint h=0);

	virtual void setSkin(string key,Skin* skin);
private:

	FXIcon* overicon;			
	FXIcon* downicon;			
	FXIcon* hiticon;			
  FXIcon* disableicon;	

	bool m_isOnMask;					//Flag de la mascara
  FXRegion* m_regionActive;	//Región Activa

public:

	void setOverIcon(FXIcon* over);
	FXIcon* getOverIcon();

	void setDownIcon(FXIcon* down);
	FXIcon* getDownIcon();

	void setHitIcon(FXIcon* hit);
	FXIcon* getHitIcon();

  void setDisableIcon(FXIcon* hit);
	FXIcon* getDisableIcon();

	void setRegionFromImage(const FXString& fileMask);	//Construye una región a partir de una imagén
	
private:
	unsigned int m_state;	
	FXIcon** m_icons;
	FXGLViewer* m_viewer;
};

#endif
