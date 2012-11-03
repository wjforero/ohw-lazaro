#include "FXImageButton.h"

// mapa
FXDEFMAP(FXImageButton) FXImageButtonMap[]={
  FXMAPFUNC(SEL_PAINT,0,FXImageButton::onPaint),
  FXMAPFUNC(SEL_UPDATE,0,FXImageButton::onUpdate),
  FXMAPFUNC(SEL_ENTER,0,FXImageButton::onEnter),
  FXMAPFUNC(SEL_LEAVE,0,FXImageButton::onLeave),
	FXMAPFUNC(SEL_MOTION,0,FXImageButton::onMotion),
};


// Implementacion del objeto
FXIMPLEMENT(FXImageButton,FXButton,FXImageButtonMap,ARRAYNUMBER(FXImageButtonMap))


FXImageButton::FXImageButton(){
  state=STATE_UP;
}


FXImageButton::FXImageButton(FXComposite* p,const FXString& text,FXIcon* icon,FXObject* tgt, FXSelector sel, FXuint opts, FXint x, FXint y, FXint w, FXint h):
  FXButton(p,text,icon,tgt,sel,opts,x,y,w,h){
	state=STATE_UP;
	overicon=icon; downicon=icon;	hiticon=icon; disableicon=icon;
	setPadLeft(0); setPadRight(0);
	setPadBottom(0); setPadTop(0);
	m_regionActive=0;	
}


// Boton entra
long FXImageButton::onEnter(FXObject* sender,FXSelector sel,void* ptr){
  FXLabel::onEnter(sender,sel,ptr);
  if(isEnabled()){
    if((flags&FLAG_PRESSED) && (state!=STATE_ENGAGED)) setState(STATE_DOWN);
    //if(options&BUTTON_TOOLBAR) 
			update();
    }
  return 1;
  }


// Boton izquierdo
long FXImageButton::onLeave(FXObject* sender,FXSelector sel,void* ptr){
  FXLabel::onLeave(sender,sel,ptr);
  if(isEnabled()){
    if((flags&FLAG_PRESSED) && (state!=STATE_ENGAGED)) setState(STATE_UP);
    //if(options&BUTTON_TOOLBAR) 
			update();
    }
  return 1;
  }

long FXImageButton::onPaint(FXObject*,FXSelector,void* ptr){
	setBackColor(getParent()->getBackColor());
  FXint tw=0,th=0,iw=0,ih=0,tx,ty,ix,iy;
  FXEvent *ev=(FXEvent*)ptr;	
  FXDCWindow dc(this,ev);
	
	if(isEnabled() && (state==STATE_ENGAGED)){
		dc.setForeground(hiliteColor);
		dc.fillRectangle(0,0,width,height);
	}
	else{
		dc.setForeground(backColor);
		dc.fillRectangle(0,0,width,height);
	}
	
	if(!label.empty()){
		tw=labelWidth(label);
		th=labelHeight(label);
	}
	if(icon){
		iw=icon->getWidth();
		ih=icon->getHeight();
	}
	just_x(tx,ix,tw,iw);
	just_y(ty,iy,th,ih);

	if(state && (options&(FRAME_RAISED|FRAME_SUNKEN))){ 
		++tx; ++ty; ++ix; ++iy; 
	}

	if(icon){		
    if(isEnabled()) {      
		  if(underCursor() && (state==STATE_UP)){			  
				if(m_isOnMask)
			    dc.drawIcon(overicon,ix,iy);
        else
          dc.drawIcon(icon,ix,iy);
		  }		  
		  else if(underCursor() && (state==STATE_DOWN)){
			  dc.drawIcon(downicon,ix,iy);
		  }		  
		  else if(!underCursor()){
			  dc.drawIcon(icon,ix,iy);
		  }		  
		  else{
			  dc.drawIcon(overicon,ix,iy);
		  }
    } 
    else {
      dc.drawIcon(disableicon,ix,iy);
    }		
	}

	if(!label.empty() && m_skin!=0 && m_skin->getHaveText(m_key)){
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

void FXImageButton::setOverIcon(FXIcon* over){
	overicon=over;
	overicon->create();
}

FXIcon* FXImageButton::getOverIcon(){
	return overicon;
}

void FXImageButton::setDownIcon(FXIcon* down){
	downicon=down;
	downicon->create();
}

FXIcon* FXImageButton::getDownIcon(){
	return downicon;
}

void FXImageButton::setHitIcon(FXIcon* hit){
	hiticon=hit;
	hiticon->create();
}

FXIcon* FXImageButton::getHitIcon(){
	return hiticon;
}

void FXImageButton::setDisableIcon(FXIcon* disable){
	disableicon=disable;
	disableicon->create();
}

FXIcon* FXImageButton::getDisableIcon(){
	return disableicon;
}

void FXImageButton::setRegionFromImage(const FXString& fileMask){
  FXFileStream stream;
  FXImage* img=new FXGIFImage(getApp(),NULL,IMAGE_KEEP|IMAGE_SHMI|IMAGE_SHMP);
  if(stream.open(fileMask,FXStreamLoad)){
    img->loadPixels(stream);
    stream.close();
    img->create();
    m_regionActive=new FXRegion();
    for(int i=0;i<img->getWidth();i++){
      for(int j=0;j<img->getHeight();j++){
        FXColor cc=img->getPixel(i,j);    
        FXuchar uvr=FXREDVAL(cc);
        if(uvr==0){
          FXRegion regActiveV(i+1,j+1,1,1);
          (*m_regionActive)+=regActiveV;          
        }
      }
    }
  }  
	m_isOnMask=false;
}

long FXImageButton::onUpdate(FXObject* sender,FXSelector sel,void* ptr){
  if(!FXLabel::onUpdate(sender,sel,ptr)){
    if(options&BUTTON_AUTOHIDE){if(shown()){hide();recalc();}}
    if(options&BUTTON_AUTOGRAY){disable();}
    }
  return 1;
}

long FXImageButton::onMotion(FXObject* sender,FXSelector sel,void* ptr){
  FXLabel::onEnter(sender,sel,ptr);
  FXEvent *ev=(FXEvent*)ptr;
  if(isEnabled()){
    if(m_regionActive!=0){
      if(!m_regionActive->contains(ev->win_x,ev->win_y)){
        m_isOnMask=false;
        update();
      }else{
				m_isOnMask=true;        
        update();
      }   
    }
  }  
  return 1;
}

void FXImageButton::setSkin(string key,Skin* skin){
	m_skin=skin;

	icon=m_skin->getIcon(key+"norm");
	overicon=m_skin->getIcon(key+"over");
	downicon=m_skin->getIcon(key+"down");
	hiticon=m_skin->getIcon(key+"hit");
	disableicon=m_skin->getIcon(key+"disable");
}