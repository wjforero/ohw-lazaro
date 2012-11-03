#ifndef SKININTERFACE_H
#define SKININTERFACE_H

#include "skin.h"

/** Provee una interface para soportar skins
*/
class SkinInterface{  
public:
	SkinInterface():m_skin(0),m_haveText(true){
		
	}
	virtual void setValSkin(Skin* skin){
		setSkin(m_key,skin);
	}
	/** Fija el skin
	*/
	virtual void setSkin(string key,Skin* skin)=0;	
	
	/** Retorna el skin
	*/
	virtual Skin* getSkin(){
		return m_skin;
	}
	/**
	*/
	virtual void setKeySkin(string key){
		m_key=key;
	}
	void setHaveText(const bool& haveText){
		m_haveText=haveText;
	}	
protected:
	Skin* m_skin;
	string m_key;
	bool m_haveText;
};

#endif