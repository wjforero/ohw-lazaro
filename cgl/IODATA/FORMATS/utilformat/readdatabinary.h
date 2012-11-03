#ifndef READDATABINARY_H
#define READDATABINARY_H
// inclusiones especificas
#include "base.h"

CGL_BEGIN

/** Clase para modelar lecturas binarias sobre datos
*/
class ReadDataBinary
{
	public:
		
		/** Lee datos binarios
		*/
		virtual void read(void* dest, const int& len)=0;
		
		/** Se posiciona en los datos binarios
		*/
		virtual void setpos(const unsigned long& p)=0;
		
		/** Retorna la posici�n en los datos
		*/
		virtual unsigned long getpos(void)=0;

		/** Verifica si la posici�n es valida
		*/
		virtual bool ready()=0;
};

CGL_END

#endif