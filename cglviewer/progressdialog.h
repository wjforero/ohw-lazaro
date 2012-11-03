#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include "fx.h"
#include "iodata/callbakupdate.h"

/** Modela una barra de progreso
*/
class ProgressDialog : public FXProgressDialog, ohw::cgl::CallBackUpdate
{
  FXDECLARE(ProgressDialog)
protected:
	ProgressDialog(){
	
	}
public:	
	ProgressDialog(FXWindow* owner,const FXString& caption,const FXString& label,FXuint opts=PROGRESSDIALOG_NORMAL,FXint x=0,FXint y=0,FXint w=0,FXint h=0);

	void updateVal(const float& val);
	void increVal();
	int hola;
};

FXIMPLEMENT(ProgressDialog,FXProgressDialog,0,0)

ProgressDialog::ProgressDialog(FXWindow* owner,const FXString& caption,const FXString& label,FXuint opts,FXint x,FXint y,FXint w,FXint h)
:FXProgressDialog(owner,caption,label,x,y,w,h){
	hola=0;
}

void ProgressDialog::updateVal(const float& val){
	hola++;
	//setProgress((100/(m_last-m_ini))*val);
	setProgress(50);
	//setProgress(hola);
	update();
}

void ProgressDialog::increVal(){
	//setProgress((m_last-m_ini)/100);
	hola++;	
	setProgress(hola);
	update();
}


#endif