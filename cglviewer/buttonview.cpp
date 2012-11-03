#include "buttonview.h"

// mapa
FXDEFMAP(ButtonView) ButtonViewMap[]={
  FXMAPFUNC(SEL_PAINT,0,ButtonView::onPaint),
	FXMAPFUNC(SEL_LEFTBUTTONPRESS,0,ButtonView::onChangeState),
};


// Implementacion del objeto
FXIMPLEMENT(ButtonView,FXButton,ButtonViewMap,ARRAYNUMBER(ButtonViewMap))


// Deserialization
ButtonView::ButtonView()
{
	m_state=ID_FRONT;
}


// Construct and init
ButtonView::ButtonView(FXIcon** icons,FXGLViewer* viewer,FXComposite* p,const FXString& text, FXObject* tgt, FXSelector sel, FXuint opts, FXint x, FXint y, FXint w, FXint h):
  FXButton(p,text, 0, tgt, sel, opts, x, y, w, h){
	unsigned int i=0;
	state=STATE_UP;
	overicon=icon; downicon=icon;	hiticon=icon; disableicon=icon;
	setPadLeft(0); setPadRight(0);
	setPadBottom(0); setPadTop(0);
	m_regionActive = 0;	
	m_icons = new FXIcon*[6];
	if(icons!=0){
		for(i=0;i<6;i++){
			m_icons[i]=icons[i];
		}
	}
		
	m_state=0;
	m_viewer=viewer;	
}

void ButtonView::setSkin(string key,Skin* skin){
	m_skin=skin;
	char iconsView[50];
	for(int i=0;i<6;i++){
		sprintf(iconsView,"viewicon%d",i);
		m_icons[i]=skin->getIcon(iconsView);
	}	
}

long ButtonView::onPaint(FXObject*,FXSelector,void* ptr)
{	
	setBackColor(getParent()->getBackColor());
  FXint tw=0,th=0,iw=0,ih=0,tx,ty,ix,iy;
  FXEvent *ev=(FXEvent*)ptr;
	// Start drawing
  FXDCWindow dc(this,ev);
	
	if(isEnabled() && (state==STATE_ENGAGED)){
		dc.setForeground(hiliteColor);
		dc.fillRectangle(0,0,width,height);
	}else{
		dc.setForeground(backColor);
		dc.fillRectangle(0,0,width,height);
	}

	// Place text & icon
	if(!label.empty()){
		tw=labelWidth(label);
		th=labelHeight(label);
	}

	icon=m_icons[m_state]; 
	icon->create();
	if(icon){
		iw=icon->getWidth();
		ih=icon->getHeight();
	}
	just_x(tx,ix,tw,iw);
	just_y(ty,iy,th,ih);

	// Shift a bit when pressed
	if(state && (options&(FRAME_RAISED|FRAME_SUNKEN))){ 
		++tx; ++ty; ++ix; ++iy; 
	}

	// Draw the icon
	if(icon){		
    if(isEnabled()) {			
      // Enabled and cursor inside, and up
		  if(underCursor() && (state==STATE_UP)){				
          dc.drawIcon(icon,ix,iy);
		  }
		  // Enabled and cursor inside and down
		  else if(underCursor() && (state==STATE_DOWN)){
			  dc.drawIcon(icon,ix,iy);
		  }
		  // Enabled and checked
		  else if(!underCursor()){
				dc.drawIcon(icon,ix,iy); 
		  }
		  // unchecked or not under cursor
		  else{
			  dc.drawIcon(icon,ix,iy);
		  }
    } //Disabled
    else {
      dc.drawIcon(icon,ix,iy);
    }
	}		

	
	/*sprintf(dd,"%d",m_state);
	FXString ssss(dd);
	label=ssss;*/
	// Draw the text
	if(!label.empty()&& m_skin!=0 && m_skin->getHaveText(m_key)){
		dc.setTextFont(font);
		if(isEnabled()){
			dc.setForeground(textColor);
			drawLabel(dc,label,hotoff,tx,ty,tw,th);
		}
		else{
			dc.setForeground(hiliteColor);
			drawLabel(dc,label,hotoff,tx+1,ty+1,tw,th);
			dc.setForeground(shadowColor);
			drawLabel(dc,label,hotoff,tx,ty,tw,th);
		}
	}
  return 1; 
}

long ButtonView::onChangeState(FXObject* sender,FXSelector sel,void* ptr){	
	m_state = (m_state<=4)? (m_state+1):0;
	update();	
	if(m_viewer){
		switch(m_state){
			case ID_FRONT: m_viewer->handle(this,MKUINT(FXGLViewer::ID_FRONT,SEL_COMMAND),0); break;
			case ID_BACK: m_viewer->handle(this,MKUINT(FXGLViewer::ID_BACK,SEL_COMMAND),0); break;
			case ID_LEFT: m_viewer->handle(this,MKUINT(FXGLViewer::ID_LEFT,SEL_COMMAND),0); break;
			case ID_RIGHT: m_viewer->handle(this,MKUINT(FXGLViewer::ID_RIGHT,SEL_COMMAND),0); break;
			case ID_TOP: m_viewer->handle(this,MKUINT(FXGLViewer::ID_TOP,SEL_COMMAND),0); break;
			case ID_BOTTOM: m_viewer->handle(this,MKUINT(FXGLViewer::ID_BOTTOM,SEL_COMMAND),0); break;
		}								
	}
	return 1;
}
