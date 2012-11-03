#ifndef VECTORNORMALAFFINE_H
#define VECTORNORMALAFFINE_H

// inclusiones especificas
#include "base.h"
#include "vectorvectorial.h"

CGL_BEGIN

/**
 */
template <class TAffineSpace>
class VectorNormalAffine : public TAffineSpace::TVectorialSpaceSelft::Vector {
public:			
	/**
	 */
	typedef typename TAffineSpace::TVectorialSpaceSelft TVectorialSpace;
	/**
	 */
	typedef typename TAffineSpace::AffineVector TAffineVector;
	/**
	 */
	typedef typename TAffineSpace::TVectorialSpaceSelft::typevector TDataType;

public:
	/**
	 */
	VectorNormalAffine() : TAffineSpace::TVectorialSpaceSelft::Vector() {				
		(*this)[0]=0;
	}
	/**
	 */
	VectorNormalAffine(const VectorNormalAffine& v) : TAffineSpace::TVectorialSpaceSelft::Vector() {
	}
	/**
	 */
	VectorNormalAffine(const TAffineVector& v) : TAffineSpace::TVectorialSpaceSelft::Vector() {
	}

public:
	/**
	 */
	VectorNormalAffine<TAffineSpace>& operator=(const VectorAssignable<TDataType>& x) {
		x.assign(*this);
		return *this;
	}			

public:
	/**
	 */
	unsigned int getDim() {
		return VectorVectorial<TVectorialSpace>::getDim()-1;
	}
	/** Normaliza este vector
	 */
	void normalize() {
	}
};

CGL_END

#endif