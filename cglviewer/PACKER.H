#ifndef PACKER_H
#define PACKER_H

/** Modela un packer
*/
class Packer : public FXPacker, public SkinInterface{
  FXDECLARE(Packer)
protected:
	Packer(){
	
	}  
private:
	Packer(const FXPacker&){
	
	}
	Packer &operator=(const FXPacker&){
	
	}
public:
  long onPaint(FXObject*,FXSelector,void*);
	long onQueryHelp(FXObject*,FXSelector,void*);	
	void setSkin(string key,Skin* skin);
	void setHelpText(const FXString& text);
public:
  
  Packer(FXComposite *p,FXuint opts=0,FXint x=0,FXint y=0,FXint w=0,FXint h=0,FXint pl=DEFAULT_SPACING,FXint pr=DEFAULT_SPACING,FXint pt=DEFAULT_SPACING,FXint pb=DEFAULT_SPACING,FXint hs=DEFAULT_SPACING,FXint vs=DEFAULT_SPACING);  
private:
	FXIcon* m_iconBack;			//Mouse over icon
	FXString help;
};


#endif
// Map
FXDEFMAP(Packer) PackerMap[]={
  FXMAPFUNC(SEL_PAINT,0,Packer::onPaint),
	FXMAPFUNC(SEL_UPDATE,FXWindow::ID_QUERY_HELP,Packer::onQueryHelp),
};


FXIMPLEMENT(Packer,FXPacker,PackerMap,ARRAYNUMBER(PackerMap))

Packer::Packer(FXComposite* p,FXuint opts,FXint x,FXint y,FXint w,FXint h,FXint pl,FXint pr,FXint pt,FXint pb,FXint hs,FXint vs):
  FXPacker(p,opts,x,y,w,h){  
	m_iconBack=0;
}

long Packer::onPaint(FXObject*,FXSelector,void* ptr){
	setBackColor(getParent()->getBackColor());
  FXEvent *ev=(FXEvent*)ptr;
  FXDCWindow dc(this,ev);
  dc.setForeground(backColor);  
	dc.fillRectangle(ev->rect.x,ev->rect.y,ev->rect.w,ev->rect.h);	
	if(m_iconBack!=0)
		dc.drawIcon(m_iconBack,0,0);
  drawFrame(dc,0,0,width,height);	
  return 1;	
}

void Packer::setSkin(string key,Skin* skin){
	m_skin=skin;
	m_iconBack=skin->getIcon(key);	
}

long Packer::onQueryHelp(FXObject* sender,FXSelector,void*){
  if(!help.empty() && (flags&FLAG_HELP)){
    sender->handle(this,MKUINT(ID_SETSTRINGVALUE,SEL_COMMAND),(void*)&help);
    return 1;
    }
  return 0;
  }

void Packer::setHelpText(const FXString& text){
  help=text;
}