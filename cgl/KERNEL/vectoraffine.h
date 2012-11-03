#ifndef VECTORAFFINE_H
#define VECTORAFFINE_H

// inclusiones especificas
#include "base.h"
#include "vectorvectorial.h"

CGL_BEGIN

/** Define un vector afin.
 *	En realidad es un vector de un espacio euclidiano de dimension d+1
 */
template <class TAffineSpace>
class VectorAffine : public TAffineSpace::TVectorialSpaceSelf::TVector {
public:			
	/** Tipo del espacio vectorial al cual pertenece
	 */
	typedef typename TAffineSpace::TVectorialSpaceSelf TVectorialSpace;
	/**	Tipo de dato del vector
	 */
	typedef typename TAffineSpace::TVectorialSpaceSelf::TData TDataType;
	/**
	 */
	typedef typename TAffineSpace::TVectorialSpaceSelf::TVector TVectorBase;

public:
	/** Constructor por omisión
	 */
	VectorAffine() : TVectorBase() {				
    (*this)[TAffineSpace::dim]=0;
	}
	/** Constructor de copia
	 *	@param v Vector a partir del cual se construye
	 */
	VectorAffine(const VectorAffine& v) : TVectorBase()	{	
		copy(v);
    (*this)[TAffineSpace::dim]=0;
	}

public:
	/** Operador de asignación
	 *  @param x lado derecho en la expresión
	 */
  VectorAffine<TAffineSpace>& operator=(const VectorAssignable<TDataType>& x) {
		x.assign(*this);
		return *this;
	}			

  void setData(const TDataType &val){
    TVectorBase::set(val);
    (*this)[TAffineSpace::dim]=0; 
  }

public:
	/** Retorna la dimensión del espacio afin que lo contiene
	 *	@return Dimension del espacio afín que lo contiene
	 */
	unsigned int getDim() {
  	  	return VectorVectorial<TVectorialSpace>::getDim()-1;
	}
};

CGL_END

#endif
