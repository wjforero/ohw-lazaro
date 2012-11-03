/**
 * $Revision: 1.1 $
 * $Date: 2003/03/11 20:55:39 $
 */

#ifndef DIRECTION2D_H
#define DIRECTION2D_H

#include "base.h"

CGL_BEGIN

/** Representa un circulo en un espacio afin 2d.
 *  Un circulo esta representado por un centro y un radio.
 */
template<class TAffineSpace>
class Direction2d 
{
public:
	/** Tipo del espacio vectorial al cual pertenece
	 */
	typedef typename TAffineSpace::TVectorialSpaceSelft TVectorialSpace;
	/**	Tipo de dato del vector
	 */
	typedef typename TAffineSpace::TVectorialSpaceSelft::typevector TDataType;
	/**
	 */
	typedef typename TAffineSpace::TVectorialSpaceSelft::Vector TVectorBase;

public:
	/** Constructor por omisi�n
	 */
	Direction2d() : TVectorBase() {
		(*this)[0]=0;
	}
	/** Constructor de copia
	 *	@param d Direccion a partir del cual se construye
	 */
	Direction2d(const Direction2d& v) : TVectorBase()	{	
		copy(v);				
	}
	/** Construye una direcci�n a partir de un vector
	 *	@param v Vector a partir del cual se construye
	 */
	Direction2d(const Vector2d& v) : TVectorBase()	{	
		copy(v);				
	}

public:
	/** Operador de asignaci�n
	 *  @param x lado derecho en la expresi�n
	 */
  Vector2d<TAffineSpace>& operator=(const VectorAssignable<TDataType>& x)	{
		x.assign(*this);
		return *this;
	}

public:
	/** Retorna la dimensi�n del espacio afin que lo contiene
	 *	@return Dimension del espacio af�n que lo contiene
	 */
	unsigned int getDim() {
  	return VectorVectorial<TVectorialSpace>::getDim()-1;
	}
};

CGL_END

#endif