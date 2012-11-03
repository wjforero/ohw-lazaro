#ifndef ALPHABALLCONTROL_H
#define ALPHABALLCONTROL_H

#ifndef FXFRAME_H
#include "FXFrame.h"
#endif

class FXImage;

/** Control para el radio de la bola alpha
*/
class AlphaBallControl : public FXFrame {
  FXDECLARE(AlphaBallControl)
protected:
  /** Imagen
  */
  FXImage *dial;
  /** Valor del alpha-shape
  */
  FXfloat alpha;
  FXfloat alphaxy[2];   
  /** Localizaci�n se�alada x
  */
  FXint spotx;
  /** Localizaci�n se�alada y
  */
  FXint spoty;
  /** Localizaci�n en el dial x
  */
  FXint dialx;
  /** Localizaci�n en el dial y
  */
  FXint dialy;
  /** Valor para el tooltip
  */
  FXString tip;
  /** Valor de ayuda
  */
  FXString help;
protected:
  /** Constructor
  */
  AlphaBallControl();
  /** Funci�n para la actualizaci�n del dial
  */
  void updatedial();
  /** Mueve el inticador
  */
  void movespot(FXint x,FXint y);
  /** Realiza la conversi�n para la geometria
  */
  //FXbool spottoxy(FXint& x,FXint& y,FXint valphax,FXint valphay) const;
  FXbool spottoxy(FXint& x,FXint& y,FXfloat valphax,FXfloat valphay) const;
  /** Realiza la conversi�n para el valor de alpha
  */
  //FXbool xytospot(FXint& valphax,FXint& valphay,FXint x,FXint y) const;
  FXbool xytospot(FXfloat& valphax,FXfloat& valphay,FXint x,FXint y) const;

  void getMaxBound(FXint& vmax,int spotx,int spoty);
  /** Recalcula el layout
  */
  virtual void layout();
private:
  /** Construtor de copia
  */
  AlphaBallControl(const AlphaBallControl&);
  /** Operador de igualdad
  */
  AlphaBallControl &operator=(const AlphaBallControl&);
public:
  /** Metodo de pintado
  */
  long onPaint(FXObject*,FXSelector,void*);
  /** Funci�n para el button press
  */
  long onLeftBtnPress(FXObject*,FXSelector,void*);
  /** Funci�n para el release
  */
  long onLeftBtnRelease(FXObject*,FXSelector,void*);
  /** Funci�n de movimiento
  */
  long onMotion(FXObject*,FXSelector,void*);
  /** Petici�n de la ayuda
  */
  long onQueryHelp(FXObject*,FXSelector,void*);
  /** Petici�n del tip
  */
  long onQueryTip(FXObject*,FXSelector,void*);
public:

  /** Construtor
  */
  AlphaBallControl(FXComposite* p,FXObject* tgt=NULL,FXSelector sel=0,FXuint opts=FRAME_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=DEFAULT_PAD,FXint pr=DEFAULT_PAD,FXint pt=DEFAULT_PAD,FXint pb=DEFAULT_PAD);

  /** Crea los recursos
  */
  virtual void create();

  /** Libera los recursos
  */
  virtual void detach();

  /** Ancho por omisi�n
  */
  virtual FXint getDefaultWidth();

  /** Alto por omisi�n
  */
  virtual FXint getDefaultHeight();

  /** Fija el valor del alpha
  */
  void setAlpha(FXfloat h);

  /** Retorna el valor del alpha
  */
  FXfloat getAlpha() const { return alpha; }
  
  /** Fija la l�nea de ayuda para este control
  */
  void setHelpText(const FXString& text);

  /** Retorna el valor de la ayuda
  */
  FXString getHelpText() const { return help; }

  /** Fija el texto para el tip
  */
  void setTipText(const FXString& text);

  /** Retorna el texto para el tip
  */
  FXString getTipText() const { return tip; }

  /** Salva el valor
  */
  virtual void save(FXStream& store) const;

  /** Carga el valor
  */
  virtual void load(FXStream& store);

  /** Destructor
  */
  virtual ~AlphaBallControl();
};

#endif