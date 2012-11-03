#include "buttonrotate.h"

// mapa
FXDEFMAP(ButtonRotate) ButtonRotateMap[]={		 
  FXMAPFUNC(SEL_LEFTBUTTONPRESS,0,ButtonRotate::onLeftBtnPress),
  FXMAPFUNC(SEL_LEFTBUTTONRELEASE,0,ButtonRotate::onLeftBtnRelease),
	FXMAPFUNC(SEL_MOTION,0,ButtonRotate::onRotate),
};


// Implementacion del objeto
FXIMPLEMENT(ButtonRotate,FXImageButton,ButtonRotateMap,ARRAYNUMBER(ButtonRotateMap))


// Deserialization
ButtonRotate::ButtonRotate(){	
}

// Construct and init
ButtonRotate::ButtonRotate(FXGLViewer* viewer,FXComposite* p,const FXString& text,FXIcon* icon, FXObject* tgt, FXSelector sel, FXuint opts, FXint x, FXint y, FXint w, FXint h):
  FXImageButton(p,text, icon, tgt, sel, opts, x, y, w, h){
	m_viewer=viewer;
	m_option=0;	
}

// Handle de repaint
long ButtonRotate::onLeftBtnPress(FXObject*,FXSelector,void* ptr){
	FXEvent* event=(FXEvent*)ptr;	
  if(isEnabled()){
    grab();    
    m_option=ROTATING;
    m_curAntx = event->last_x;
    m_curAnty = event->last_y;
  }
  return 1; 
}

long ButtonRotate::onLeftBtnRelease(FXObject* sender,FXSelector sel,void* ptr){
	FXEvent* event=(FXEvent*)ptr;
  if(isEnabled()){
    ungrab();    
    m_option=0;    
  }
  return 1;
}

long ButtonRotate::onRotate(FXObject* sender,FXSelector sel,void* ptr){	
	FXEvent* ev=(FXEvent*)ptr;
	if(m_viewer){
		if(m_option==ROTATING){
				const FXVec xaxis(1.0f,0.0f,0.0f);
        const FXVec yaxis(0.0f,1.0f,0.0f);
        const FXVec zaxis(0.0f,0.0f,1.0f);        
        FXfloat angx,angy;
        FXQuat qx,qy;  
        angx=(FXfloat)DTOR*(ev->win_x-ev->last_x);
        angy=(FXfloat)DTOR*(ev->win_y-ev->last_y);
        qx=FXQuat(yaxis,angx);
        qy=FXQuat(xaxis,angy);
        m_viewer->setOrientation(qx*m_viewer->getOrientation());
        m_viewer->setOrientation(qy*m_viewer->getOrientation());
    }
  }
	return 1;
}

