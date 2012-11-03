#include "buttontranslate.h"

// mapa
FXDEFMAP(ButtonTranslate) ButtonTranslateMap[]={		 
  FXMAPFUNC(SEL_LEFTBUTTONPRESS,0,ButtonTranslate::onLeftBtnPress),
  FXMAPFUNC(SEL_LEFTBUTTONRELEASE,0,ButtonTranslate::onLeftBtnRelease),
	FXMAPFUNC(SEL_MOTION,0,ButtonTranslate::onTranslate),
};


// Implementacion del objeto
FXIMPLEMENT(ButtonTranslate,FXImageButton,ButtonTranslateMap,ARRAYNUMBER(ButtonTranslateMap))


// Deserialization
ButtonTranslate::ButtonTranslate(){	
}

// Construct and init
ButtonTranslate::ButtonTranslate(FXGLViewer* viewer,FXComposite* p,const FXString& text,FXIcon* icon, FXObject* tgt, FXSelector sel, FXuint opts, FXint x, FXint y, FXint w, FXint h):
  FXImageButton(p,text, icon, tgt, sel, opts, x, y, w, h){
	m_viewer=viewer;
	m_option=0;
	m_optionTranslate=TRANSLATINGXY;
}


// Handle de repaint
long ButtonTranslate::onLeftBtnPress(FXObject*,FXSelector,void* ptr){
	FXEvent* event=(FXEvent*)ptr;	
  if(isEnabled()){
    grab();    
    m_option=TRANSLATING;
    m_curAntx = event->last_x;
    m_curAnty = event->last_y;
  }
  return 1; 
}

long ButtonTranslate::onLeftBtnRelease(FXObject* sender,FXSelector sel,void* ptr){
	FXEvent* event=(FXEvent*)ptr;
  if(isEnabled()){
    ungrab();    
    m_option=0;
  }
  return 1;
}

long ButtonTranslate::onTranslate(FXObject* sender,FXSelector sel,void* ptr){
	FXEvent* event=(FXEvent*)ptr;
	FXEvent* ev=event;
	if(m_viewer){
		if(m_option==TRANSLATING){
      FXVec vec;
			if(m_optionTranslate==TRANSLATINGXY){
				vec=m_viewer->worldVector(ev->last_x,ev->last_y,ev->win_x,ev->win_y);
				m_viewer->translate(-vec);
			}else if(m_optionTranslate==TRANSLATINGX){
				vec=m_viewer->worldVector(ev->last_x,0,ev->win_x,0);
				m_viewer->translate(-vec);
			}else if(m_optionTranslate==TRANSLATINGY){
				vec=m_viewer->worldVector(0,event->last_y,0,event->win_y);
				m_viewer->translate(-vec);
			}else if(m_optionTranslate==TRANSLATINGZ){
				vec=vec=m_viewer->worldVector(0,event->last_y,0,event->win_y);
				FXfloat tmp=(FXfloat)(m_viewer->worldPix()*(event->win_y-event->last_y));
				vec=normalize(m_viewer->getEyeVector());
				m_viewer->translate(tmp*vec);				
			}
//      
      //
      /*
      m_viewer->translate(-vec);
      m_viewer->update();			*/
      
    }
  }
	return 1;
}

void ButtonTranslate::setOptionTranslate(const unsigned int& optionTranslate){
	m_optionTranslate=optionTranslate;
}