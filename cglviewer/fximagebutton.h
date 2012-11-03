#ifndef FXIMAGEBUTTON_H
#define FXIMAGEBUTTON_H

#include "fx.h"
#include "skininterface.h"

/** Se provee una clase boton con imagenenes para diferentes eventos
*/
class FXImageButton : public FXButton, public SkinInterface{
  FXDECLARE(FXImageButton)
protected:
  FXImageButton();
public:
  long onPaint(FXObject*,FXSelector,void*);
  long onUpdate(FXObject*,FXSelector,void*);
  long onEnter(FXObject*,FXSelector,void*);
  long onLeave(FXObject*,FXSelector,void*);  
public:

  /** Construye un boton con un texto
  */
  FXImageButton(FXComposite* p,const FXString& text,FXIcon* ic=NULL,FXObject* tgt=NULL, FXSelector sel=0, FXuint opts=LAYOUT_EXPLICIT, FXint x=0, FXint y=0, FXint w=0, FXint h=0);
private:

	FXIcon* overicon;
	FXIcon* downicon;
	FXIcon* hiticon;
  FXIcon* disableicon;

	bool m_isOnMask;
  FXRegion* m_regionActive;

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

	long onMotion(FXObject* sender,FXSelector sel,void* ptr);

	virtual void setSkin(string key,Skin* skin);
};


#endif
