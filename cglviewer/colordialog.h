#ifndef COLORDIALOG_H
#define COLORDIALOG_H

#ifndef FXDIALOGBOX_H
#include "FXDialogBox.h"
#endif

#ifndef COLORSELECTOR_H
#include "colorselector.h"
#endif

/** Modela un dialogo para el color
*/
class ColorDialog : public FXDialogBox {
  FXDECLARE(ColorDialog)
protected:
  ColorSelector *colorbox;
protected:
  ColorDialog(){}
private:
  ColorDialog(const ColorDialog&);
  ColorDialog &operator=(const ColorDialog&);
public:
  long onChgColor(FXObject*,FXSelector,void*);
  long onCmdColor(FXObject*,FXSelector,void*);
public:
  enum {
    ID_COLORSELECTOR=FXDialogBox::ID_LAST,
    ID_LAST
    };
public:
  ColorDialog(FXWindow* owner,const FXString& name,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0,Skin* skin=0);
  void setRGBA(FXColor clr);
  FXColor getRGBA() const;
  FXbool isOpaqueOnly() const;
  void setOpaqueOnly(FXbool forceopaque);
  long onMouseDown(FXObject*,FXSelector,void*);
  long onMouseUp(FXObject*,FXSelector,void*);
	long onMouseMove(FXObject*,FXSelector,void*);
  virtual ~ColorDialog();
private:
  int mdflag;
  int vx;
  int vy;  
};

/** Mapa de eventos
*/
FXDEFMAP(ColorDialog) ColorDialogMap[]={
  FXMAPFUNC(SEL_CHANGED,ColorDialog::ID_COLORSELECTOR,ColorDialog::onChgColor),
  FXMAPFUNC(SEL_COMMAND,ColorDialog::ID_COLORSELECTOR,ColorDialog::onCmdColor),
  FXMAPFUNC(SEL_LEFTBUTTONPRESS,   0, ColorDialog::onMouseDown),
	FXMAPFUNC(SEL_LEFTBUTTONRELEASE, 0, ColorDialog::onMouseUp),
	FXMAPFUNC(SEL_MOTION,            0, ColorDialog::onMouseMove)
};

/** implementa el dialogo
*/
FXIMPLEMENT(ColorDialog,FXDialogBox,ColorDialogMap,ARRAYNUMBER(ColorDialogMap))

/** Construye el dialogo
*/
ColorDialog::ColorDialog(FXWindow* owner,const FXString& name,FXuint opts,FXint x,FXint y,FXint w,FXint h,Skin* skin):
  FXDialogBox(owner,name,opts|DECOR_CLOSE|DECOR_MINIMIZE,x,y,w,h,0,0,0,0,4,4){
  colorbox=new ColorSelector(this,this,ID_COLORSELECTOR,LAYOUT_FILL_X|LAYOUT_FILL_Y,0,0,0,0,skin);
  colorbox->setTarget(this);
  colorbox->setSelector(ID_COLORSELECTOR);
  colorbox->acceptButton()->setTarget(this);
  colorbox->acceptButton()->setSelector(FXDialogBox::ID_ACCEPT);
  colorbox->cancelButton()->setTarget(this);
  colorbox->cancelButton()->setSelector(FXDialogBox::ID_CANCEL);
  mdflag=0;
}

/** Fija el RBG
*/
void ColorDialog::setRGBA(FXColor clr){
  colorbox->setRGBA(clr);
}

/** Obtiene el RBG
*/
FXColor ColorDialog::getRGBA() const {
  return colorbox->getRGBA();
}

/** Maneja el cambio de color
*/
long ColorDialog::onChgColor(FXObject*,FXSelector,void* ptr){
  if(target) target->handle(this,MKUINT(message,SEL_CHANGED),ptr);
  return 1;
}

/** Maneja el aceptar de color
*/
long ColorDialog::onCmdColor(FXObject*,FXSelector,void* ptr){
  if(target) target->handle(this,MKUINT(message,SEL_COMMAND),ptr);
    return 1;
 }

/** Retorna true si es opaca el color box
*/
FXbool ColorDialog::isOpaqueOnly() const {
  return colorbox->isOpaqueOnly();
}

/** Fija el valor de opacidad
*/
void ColorDialog::setOpaqueOnly(FXbool forceopaque){
  colorbox->setOpaqueOnly(forceopaque);
}

/** Destructor
*/
ColorDialog::~ColorDialog(){
  colorbox=(ColorSelector*)-1;
}

/** Manejador de presión del botón
*/
long ColorDialog::onMouseDown(FXObject*,FXSelector,void*ptr){
	FXEvent *ev=(FXEvent*)ptr;  	
	grab();

	mdflag=1;
	vx=ev->win_x;
	vy=ev->win_y;	
  
	return 1;
}

/** Manejador de movimiento del boton
*/
long ColorDialog::onMouseMove(FXObject* sender, FXSelector sel, void* ptr){
  FXEvent *ev=(FXEvent*)ptr;  
  if(mdflag && (ev->last_x!=ev->win_x) && (ev->last_y!=ev->win_y)){
	  move(getX() +(ev->win_x)-vx,getY()+(ev->win_y)-vy);	
  }
  return 1;
}

/** Manejador del up del boton
*/
long ColorDialog::onMouseUp(FXObject*,FXSelector,void* ptr){	
  FXEvent *ev=(FXEvent*) ptr;
  if(mdflag){	  	
    mdflag=0;
  }
  ungrab();
  return 1;
}


#endif