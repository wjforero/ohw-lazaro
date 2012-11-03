#ifndef ALPHABALLDIALOG_H
#define ALPHABALLDIALOG_H

#include "fx.h"
#include "textfield.h"

#include "base.h"
#include "alphaballcontrol.h"

/** Dialog del alpha shape
*/
class AlphaBallDialog : public FXDialogBox {
  FXDECLARE(AlphaBallDialog)

public:
  // Constructor
  AlphaBallDialog(FXApp* a);    
  
  // Destructor
  virtual ~AlphaBallDialog();

protected:
  AlphaBallDialog(){};

public:
  AlphaBallDialog(FXWindow* owner,const FXString& name,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0);  
  enum{
    ID_VALUEALPHA=FXDialogBox::ID_LAST
  };

  
  long onCmdAlpha(FXObject*,FXSelector,void*);
  long onMouseDown(FXObject*,FXSelector,void*);
  long onMouseUp(FXObject*,FXSelector,void*);
	long onMouseMove(FXObject*,FXSelector,void*);


private:
  TextField* tfalpha;

  int mdflag;
  int vx;
  int vy;  
};

FXDEFMAP(AlphaBallDialog) AlphaBallDialogMap[]={    
  FXMAPFUNC(SEL_CHANGED,AlphaBallDialog::ID_VALUEALPHA,AlphaBallDialog::onCmdAlpha),
  FXMAPFUNC(SEL_LEFTBUTTONPRESS,   0, AlphaBallDialog::onMouseDown),
	FXMAPFUNC(SEL_LEFTBUTTONRELEASE, 0, AlphaBallDialog::onMouseUp),
	FXMAPFUNC(SEL_MOTION,            0, AlphaBallDialog::onMouseMove)
};

FXIMPLEMENT(AlphaBallDialog,FXDialogBox,AlphaBallDialogMap,ARRAYNUMBER(AlphaBallDialogMap))

AlphaBallDialog::AlphaBallDialog(FXWindow* owner,const FXString& name,FXuint opts,FXint x,FXint y,FXint w,FXint h)
  :FXDialogBox(owner,name,opts|DECOR_CLOSE|DECOR_MINIMIZE,x,y,w,h,0,0,0,0,4,4){

  // frame de operaciones de vista sobre toda la scena
  FXVerticalFrame * frameViewStateOperation=new FXVerticalFrame(this,LAYOUT_SIDE_LEFT|LAYOUT_CENTER_Y|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);  

  tfalpha=new TextField(frameViewStateOperation,6);  
  new AlphaBallControl(frameViewStateOperation,this,ID_VALUEALPHA,LAYOUT_FIX_WIDTH|LAYOUT_FIX_HEIGHT,0,0,100,100);
  
  new FXButton(this,"&Accept",NULL,this,ID_ACCEPT,FRAME_RAISED|FRAME_THICK|LAYOUT_RIGHT|LAYOUT_CENTER_Y);

  new FXButton(this,"&Cancel",NULL,this,ID_CANCEL,FRAME_RAISED|FRAME_THICK|LAYOUT_RIGHT|LAYOUT_CENTER_Y);

  mdflag=0;
}

AlphaBallDialog::~AlphaBallDialog(){
  
}

long AlphaBallDialog::onCmdAlpha(FXObject*,FXSelector,void*ptr){  
  float* arr=(float*)ptr;
  tfalpha->setText(FXStringVal(arr[0]));
  return 1;
}

long AlphaBallDialog::onMouseDown(FXObject*,FXSelector,void*ptr){
	FXEvent *ev=(FXEvent*)ptr;  	
	grab();

	mdflag=1;
	vx=ev->win_x;
	vy=ev->win_y;	
  
	return 1;
}



long AlphaBallDialog::onMouseMove(FXObject* sender, FXSelector sel, void* ptr){
  FXEvent *ev=(FXEvent*)ptr;  
  if(mdflag && (ev->last_x!=ev->win_x) && (ev->last_y!=ev->win_y)){
	  move(getX() +(ev->win_x)-vx,getY()+(ev->win_y)-vy);	
  }
  return 1;
}


long AlphaBallDialog::onMouseUp(FXObject*,FXSelector,void* ptr){	
  FXEvent *ev=(FXEvent*) ptr;
  if(mdflag){	  	
    mdflag=0;
  }
  ungrab();
  return 1;
}


#endif