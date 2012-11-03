#ifndef SKIN_H
#define SKIN_H

#include "fx.h"

#include <vector>
#include <string>
using std::vector;
using std::string;

/** Provee para soportar skins
*/
class Skin{  
public:
	Skin(){
		m_pathSkin = new char[50];
		sprintf(m_pathSkin,"");
	}
	~Skin(){
		delete m_pathSkin;
	}
public:
	enum{
    ID_CHANGESTATE=FXWindow::ID_LAST
  };

	enum{
    ID_FRONTVIEW=0,
		ID_FRONT,
		ID_BACK,
		ID_LEFT,
		ID_RIGHT,
		ID_TOP,
		ID_BOTTOM
  };
public:
  long onPaint(FXObject*,FXSelector,void*);  
	//long onChangeState(FXObject*,FXSelector,void*);
	long onChangeState(FXObject* sender,FXSelector sel,void* ptr);

public:

	/** Inserta un valor clave y su valor
	*/
	void setValueIcon(const string& name,FXIcon* icon,const bool& haveText=true){
		m_names.push_back(name);
		m_icons.push_back(icon);
		m_haveText.push_back(haveText);
	}

	/** Retorna el icono asociado con el valor clave
	*/
	FXIcon* getIcon(const string& name){
		unsigned int i=0;
		for(i=0;i<m_names.size();i++){
			if(name==m_names[i]){
				return m_icons[i];
			}
		}
		return 0;
	}

	/** Retorna el icono asociado con el valor clave
	*/
	bool getHaveText(const string& name){
		unsigned int i=0;
		for(i=0;i<m_names.size();i++){
			if(name==m_names[i]){
				return m_haveText[i];
			}
		}
		return true;
	}

	void loadIconsResource(const string& key,char* nameIcon){
		char valorButtonFile[50],defaultFile[50];
		FXGIFIcon* icc=0;
		sprintf(valorButtonFile,"%s%s.gif",m_pathSkin,nameIcon);
		sprintf(defaultFile,"%sdefault.gif",m_pathSkin);
		FXFileStream fs;
		icc= new FXGIFIcon(m_app);									
		bool haveText=false;
		if(fs.open(valorButtonFile,FXStreamLoad) && m_app!=0){
			icc->loadPixels(fs);
		}else if(fs.open(defaultFile,FXStreamLoad)){
			icc->loadPixels(fs);
			haveText=true;
		}
		icc->create();
		setValueIcon(key,icc,haveText);
		fs.close();
	}

	void loadIconsButton(char* namebutton){		
		char* arrVal[]={"norm","over","down","hit","disable"};
		char valorButton[50],valorButtonFile[50],defaultFile[50];
		unsigned int i=0;
		FXGIFIcon* icc=0;		
		sprintf(defaultFile,"%sdefault.gif",m_pathSkin);
		bool haveText=false;
		for(i=0;i<5;i++){
			sprintf(valorButton,"%s%s",namebutton,arrVal[i]);
			sprintf(valorButtonFile,"%s%s%s.gif",m_pathSkin,namebutton,arrVal[i]);
			FXFileStream fs;
			icc= new FXGIFIcon(m_app);									
			
			if(fs.open(valorButtonFile,FXStreamLoad) && m_app!=0){				
				icc->loadPixels(fs);
			}else if(fs.open(defaultFile,FXStreamLoad)){				
				icc->loadPixels(fs);
				haveText=true;
			}
			
			icc->create();
			FXColor cb=FXRGB(0,0,0);	
			icc->setTransparentColor(cb);
			setValueIcon(valorButton,icc,haveText);
			fs.close();
		}		
	}

	void setApp(FXApp* app){
		m_app=app;
	}

	void setPathSkin(char *patSkin){		
		sprintf(m_pathSkin,"%s",patSkin);
	}

	void setBackColorDialog(FXColor& backColor){
		m_backColor=backColor;
	}

	FXColor getBackColorDialog(){
		return m_backColor;
	}
	
private:	
	vector<string> m_names;
	vector<FXIcon*> m_icons;	
	vector<bool> m_haveText;
	FXApp* m_app;	
	char* m_pathSkin;
	FXColor m_backColor;
};

#endif