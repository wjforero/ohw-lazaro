/**
 * $Id: vectorvectorial.h,v 1.2 2003/02/15 17:27:57 Wilson Javier Exp $
 * $Log: vectorvectorial.h,v $
 * Revision 1.2  2003/02/15 17:27:57  Wilson Javier
 * *** empty log message ***
 *
 * Revision 1.1  2003/02/13 21:07:12  Wilson Javier
 * *** empty log message ***
 *
 */

#ifndef VECTORVECTORIAL_H
#define VECTORVECTORIAL_H

// Inclusiones especificas
#include "base.h"
#include "numericalvector.h"
using namespace ohw::numerical;


CGL_BEGIN

/** Esta clase define un vector de un espacio vectorial.
 */
template <class TVectorialSpace>
class VectorVectorial : public NumericalVector<typename TVectorialSpace::TData> {
public:			
	/** Define la dimensión de un espacio vectorial.
	 */
	enum{dim=TVectorialSpace::dim};
	/** Tipo de dato de los elementos del vector.
	 */
	typedef typename TVectorialSpace::TData TData;

public:
	/** Constructor por omisión.
	 *	Por omisión construye un vector de la dimensión del espacio que lo contiene.
	 */			
	VectorVectorial() : NumericalVector<TData>(dim){			
	}
	/** Constructor de copia.
	 */
	VectorVectorial(const VectorVectorial& v) : NumericalVector<TData>(dim) {
		(*this).copy(v);
	}
	/**	Operador de asignación.
	 *	@param x Lado derecho
	 */
	VectorVectorial<TVectorialSpace>& operator=(const VectorAssignable<TData>& x) {
		x.assign(*this);
		return *this;
	}			
	/**	Retorna la dimensión del espacio al cual pertenece el vector.
	 *	@return Dimension del espacio
	 */
	unsigned int getDim() const {
		return dim;
	}

private:
	// Espacio para ocultar los operadores indeseables.
};

CGL_END

#endif


