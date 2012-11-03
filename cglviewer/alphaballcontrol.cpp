#include "xincs.h"
#include "fxver.h"
#include "fxdefs.h"
#include "fxkeys.h"
#include "FXStream.h"
#include "FXString.h"
#include "FXSize.h"
#include "FXPoint.h"
#include "FXRectangle.h"
#include "FXSettings.h"
#include "FXRegistry.h"
#include "FXApp.h"
#include "FXDCWindow.h"
#include "FXImage.h"
#include "alphaballcontrol.h"


#define ALPHADIAMETER  60   // Diametro por omisión del control

/*******************************************************************************/


// Map
FXDEFMAP(AlphaBallControl) FXColorWheelMap[]={
  FXMAPFUNC(SEL_PAINT,0,AlphaBallControl::onPaint),
  FXMAPFUNC(SEL_MOTION,0,AlphaBallControl::onMotion),
  FXMAPFUNC(SEL_LEFTBUTTONPRESS,0,AlphaBallControl::onLeftBtnPress),
  FXMAPFUNC(SEL_LEFTBUTTONRELEASE,0,AlphaBallControl::onLeftBtnRelease),
  FXMAPFUNC(SEL_UPDATE,FXWindow::ID_QUERY_TIP,AlphaBallControl::onQueryTip),
  FXMAPFUNC(SEL_UPDATE,FXWindow::ID_QUERY_HELP,AlphaBallControl::onQueryHelp),
  };


// Object implementation
FXIMPLEMENT(AlphaBallControl,FXFrame,FXColorWheelMap,ARRAYNUMBER(FXColorWheelMap))



/*******************************************************************************/


// Init
AlphaBallControl::AlphaBallControl(){
  flags|=FLAG_ENABLED;
  alpha=0.01f;
  spotx=0;
  spoty=0;
  dialx=0;
  dialy=0;
  alphaxy[0]=0;
  alphaxy[1]=0;
}


AlphaBallControl::AlphaBallControl(FXComposite* p,FXObject* tgt,FXSelector sel,FXuint opts,FXint x,FXint y,FXint w,FXint h,FXint pl,FXint pr,FXint pt,FXint pb):
  FXFrame(p,opts,x,y,w,h,pl,pr,pt,pb){
  flags|=FLAG_ENABLED;
  target=tgt;
  message=sel;
  dial=new FXImage(getApp(),NULL,IMAGE_ALPHA|IMAGE_DITHER|IMAGE_KEEP|IMAGE_OWNED|IMAGE_SHMI|IMAGE_SHMP,ALPHADIAMETER,ALPHADIAMETER);
  alpha=0.01f;
  spotx=ALPHADIAMETER/2;
  spoty=ALPHADIAMETER/2;
  dialx=0;
  dialy=0;
}


void AlphaBallControl::create(){
  FXFrame::create();
  updatedial();
  dial->create();
}

void AlphaBallControl::detach(){
  FXFrame::detach();
  dial->detach();
}

FXint AlphaBallControl::getDefaultWidth(){
  return ALPHADIAMETER+padleft+padright+(border<<1);
}

FXint AlphaBallControl::getDefaultHeight(){
  return ALPHADIAMETER+padtop+padbottom+(border<<1);
}

void AlphaBallControl::layout(){
  register FXint ww,hh,ss;
  ww=width-padleft-padright-(border<<1);
  hh=height-padtop-padbottom-(border<<1);
  ss=FXMAX(3,FXMIN(ww,hh));
  if(dial->getWidth()!=ss){
    dial->resize(ss,ss);
    updatedial();
    dial->render();
  }
  dialx=border+padleft+(ww-ss)/2;
  dialy=border+padtop+(hh-ss)/2;
  spottoxy(spotx,spoty,alphaxy[0],alphaxy[1]);
  flags&=~FLAG_DIRTY;
}

FXbool AlphaBallControl::spottoxy(FXint& x,FXint& y,FXfloat h,FXfloat s) const {
  register FXdouble r=dial->getWidth()*0.5;
  register FXdouble a=(h-180.0)*DTOR;
  x=(FXint)(s*r*cos(a)+r+0.5);
  y=(FXint)(s*r*sin(a)+r+0.5);
  return TRUE;

}

FXbool AlphaBallControl::xytospot(FXfloat& h,FXfloat& s,FXint x,FXint y) const {
  register FXdouble r=dial->getWidth()*0.5;
  register FXdouble rx=x-r;
  register FXdouble ry=y-r;
  register FXdouble v=sqrt(rx*rx+ry*ry);
  h=0.0f;
  s=0.0f;
  if(0.0<v){
    h=atan2(ry,rx)*RTOD+180.0;
    if(v<r){
      s=v/r;
      return TRUE;
      }
    s=1.0f;
    }
  return FALSE;

  }


void AlphaBallControl::updatedial(){
  register FXColor *colors=(FXColor*)dial->getData();
  float vx,vy;
  for(register FXint y=0; y<dial->getHeight(); y++){
    for(register FXint x=0; x<dial->getWidth(); x++){
      if(xytospot(vx,vy,x,y)){        
        colors[y*dial->getWidth()+x]=FXRGB(0.0f,0.0f,255.0f);
      }else{
        colors[y*dial->getWidth()+x]=backColor;
        }
      }
    }
}

void AlphaBallControl::movespot(FXint x,FXint y){
  if(spotx!=x || spoty!=y){    
    xytospot(alphaxy[0],alphaxy[1],x,y);    
    spottoxy(spotx,spoty,alphaxy[0],alphaxy[1]);    
    update();
  }
}


long AlphaBallControl::onQueryHelp(FXObject* sender,FXSelector,void*){
  if(!help.empty() && (flags&FLAG_HELP)){
    sender->handle(this,MKUINT(ID_SETSTRINGVALUE,SEL_COMMAND),(void*)&help);
    return 1;
    }
  return 0;
}


long AlphaBallControl::onQueryTip(FXObject* sender,FXSelector,void*){
  if(!tip.empty() && (flags&FLAG_TIP)){
    sender->handle(this,MKUINT(ID_SETSTRINGVALUE,SEL_COMMAND),(void*)&tip);
    return 1;
    }
  return 0;
  }


long AlphaBallControl::onPaint(FXObject*,FXSelector,void* ptr){
  FXEvent *event=(FXEvent*)ptr;
  FXDCWindow dc(this,event);
  FXint d=dial->getWidth();
  dc.setForeground(backColor);
  dc.fillRectangle(0,0,width,height);
 /* dc.fillRectangle(border,border,dialx-border,height-(border<<1));
  dc.fillRectangle(dialx+dial->getWidth(),border,width-border-dialx-dial->getWidth(),height-(border<<1));
  dc.fillRectangle(dialx,border,dial->getWidth(),dialy-border);
  dc.fillRectangle(dialx,dialy+dial->getHeight(),dial->getWidth(),height-border-dialy-dial->getHeight());  
  //dc.drawImage(dial,dialx,dialy);
  dc.setForeground(borderColor);
  dc.drawArc(dialx+1,dialy,d,d,90*64,45*64);
  dc.drawArc(dialx,dialy+1,d,d,135*64,45*64);
  dc.setForeground(baseColor);
  dc.drawArc(dialx-1,dialy,d,d,270*64,45*64);
  dc.drawArc(dialx,dialy-1,d,d,315*64,45*64);
  dc.setForeground(shadowColor);
  dc.drawArc(dialx,dialy,d,d,45*64,180*64);
  dc.setForeground(hiliteColor);
  dc.drawArc(dialx,dialy,d,d,225*64,180*64);
  dc.setForeground(FXRGB(255,255,255));*/
  // pinta el spot  
  dc.setForeground(FXRGB(0,0,0));
  dc.fillArc(dialx+spotx-2,dialy+spoty-2,5,5,0,360*64); 

  int vmax;
  getMaxBound(vmax,spotx,spoty);
  dc.drawArc(dialx+dial->getWidth()*0.5 - vmax,dialy+dial->getHeight()*0.5 - vmax,2*vmax,2*vmax,0,360*64);
  drawFrame(dc,0,0,width,height);
  return 1;
}


void AlphaBallControl::getMaxBound(FXint& vmax,int spotx,int spoty){
  int vx=dial->getWidth()*0.5 - spotx;
  int vy=dial->getHeight()*0.5 - spoty;
  vmax=sqrt(vx*vx+vy*vy);
}


long AlphaBallControl::onMotion(FXObject*,FXSelector,void* ptr){
  FXEvent* event=(FXEvent*)ptr;
  flags&=~FLAG_TIP;
  float* valphap = new float[1];    
  int vmax;
  if(flags&FLAG_PRESSED){
    movespot(event->win_x-dialx,event->win_y-dialy);
    getMaxBound(vmax,spotx,spoty);
    valphap[0]=vmax;
    if(target) 
      target->handle(this,MKUINT(message,SEL_CHANGED),(void*)valphap);
    flags|=FLAG_CHANGED;
    delete valphap;
    return 1;
  }
  return 0;
}


long AlphaBallControl::onLeftBtnPress(FXObject*,FXSelector,void* ptr){
  FXEvent* event=(FXEvent*)ptr;
  flags&=~FLAG_TIP;
  float* valphap = new float[1];  
  int vmax;
  if(isEnabled()){
    grab();
    if(target && target->handle(this,MKUINT(message,SEL_LEFTBUTTONPRESS),ptr)) return 1;
    movespot(event->win_x-dialx,event->win_y-dialy);
    getMaxBound(vmax,spotx,spoty);
    valphap[0]=vmax;
    if(target) 
      target->handle(this,MKUINT(message,SEL_CHANGED),(void*)valphap);
    flags|=FLAG_CHANGED;
    flags&=~FLAG_UPDATE;
    flags|=FLAG_PRESSED;
    delete valphap;
    }
  return 1;
  }



long AlphaBallControl::onLeftBtnRelease(FXObject*,FXSelector,void* ptr){
  FXuint flgs=flags;
  float* valphap = new float[1];  
  int vmax;
  if(isEnabled()){
    ungrab();
    flags|=FLAG_UPDATE;
    flags&=~FLAG_PRESSED;
    flags&=~FLAG_CHANGED;
    if(target && target->handle(this,MKUINT(message,SEL_LEFTBUTTONRELEASE),ptr)) return 1;
    if(flgs&FLAG_CHANGED){
      getMaxBound(vmax,spotx,spoty);
      valphap[0]=vmax;
      if(target) target->handle(this,MKUINT(message,SEL_COMMAND),(void*)valphap);
      delete valphap;
      }      
    return 1;
    }
  return 1;
  }


void AlphaBallControl::setAlpha(FXfloat valpha){
  alpha=valpha;  
}

void AlphaBallControl::setHelpText(const FXString& text){
  help=text;
}

void AlphaBallControl::setTipText(const FXString& text){
  tip=text;
}



void AlphaBallControl::save(FXStream& store) const {
}

void AlphaBallControl::load(FXStream& store){
}

AlphaBallControl::~AlphaBallControl(){
  delete dial;
  dial=(FXImage*)-1;
}
