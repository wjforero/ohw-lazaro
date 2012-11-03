#ifndef CALLBAKUPDATE_H
#define CALLBAKUPDATE_H

#include "base.h"

CGL_BEGIN


/** Modela una interface de callback para la actualización de variables
*/
class CallBackUpdate{
  public:
		CallBackUpdate():m_ini(0),m_last(100){
		
		}
		void setIniVal(const float& ini){
			m_ini=ini;
		}

		void setLastVal(const float& last){
			m_last=last;
		}

		virtual void updateVal(const float& val)=0;
		virtual void increVal()=0;
protected:
		float m_ini;
		float m_last;
  
};

CGL_END

#endif