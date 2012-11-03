#ifndef SPACE_H
#define SPACE_H

// Inclusiones especificas
#include <string>
using std::string;

#include "base.h"

CGL_BEGIN

/** CGSpace modela un espacio geom�trico de cualquier tipo.
 *	Un espacio geom�trico puede modelarse por medio de herencia.
 */
class CGSpace {
public:
	/** Consructor por omisi�n, por omisi�n el nombre es "Espacio General"
	 */
	CGSpace() : m_strName("Espacio General") {		
	}

	/** Constructor del nombre del espacio
	 *	@param strName Nombre del espacio
	 */
	CGSpace(string strName) : m_strName(strName) {
	}

	/** Retorna el nombre del espacio
	 *	@return Retorna el nombre del espacio
	 */	
	string getName() {
		return m_strName;  	
	}

public:
	/** Fija el nombre de un espacio
	 *	@param strName Nombre del espacio
	 */
	void setName(string strName) {
		m_strName=strName;
	}

private:
	/** Nombre del espacio a manipular
	 */
	string m_strName;	
};

CGL_END

#endif