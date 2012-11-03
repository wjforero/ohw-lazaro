/**
 * $Revision: 1.1 $
 * $Date: 2003/03/11 20:55:39 $
 */

#ifndef POINT2D_H
#define POINT2D_H

#include "base.h"

CGL_BEGIN

/** Define un punto en un espacio afin 2d.
 *	En realidad es un vector de un espacio vectorial de dimension 3
 */
template <class TAffineSpace>
class Point2d : public TAffineSpace::TVectorialSpaceSelf::TVector {

public:			
	/** Define el tipo de dato del espacio vectorial del cual hereda.
	 */
	typedef typename TAffineSpace::TVectorialSpaceSelf TVectorialSpace;
	/** Define el tipo de dato del espacio vectorial es decir de sus vectores.
	 */
	typedef typename TAffineSpace::TVectorialSpaceSelf::TData TData;		
	/**
	 */
	typedef typename TAffineSpace::TVectorialSpaceSelf::TVector TVectorBase;

public:
	/** Constructor por omisión
	 */
	Point2d() : TVectorBase() {
		(*this)[0]=0;
		(*this)[1]=0;
		(*this)[TAffineSpace::dim]=1;
	}
	/** Constructor de copia
	 */
	Point2d(const Point2d& p) : TVectorBase() {				
		(*this).copy(p);			
		(*this)[TAffineSpace::dim]=1;
	}
	/** Construye un punto con coordenadas (hx,hy,1)
	 */
	Point2d(const TData& hx, const TData& hy) : TVectorBase() {
    (*this)[0]=hx;
		(*this)[1]=hy;
		(*this)[TAffineSpace::dim]=1;
	}
	/** Construye un punto con coordenadas (hx,hy,hw)
	 */
	Point2d(const TData& hx, const TData& hy, const TData& hw) : TVectorBase() {
		if(hw >= 0){
			(*this)[0]=hx;
			(*this)[1]=hy;
			(*this)[TAffineSpace::dim]=hw;
		}else{
			(*this)[0]=-hx;
			(*this)[1]=-hy;
			(*this)[TAffineSpace::dim]=-hw;
		}
	}
	
public:
	/** Operador de asignación
	 */
	Point2d<TAffineSpace>& operator=(const VectorAssignable<TData>& x) {
		x.assign(*this);
		return *this;
	}
	/** Operador de igualdad
	 */
	bool operator==(const Point2d& p) const{
		return ( (gethx()*p.gethw() == p.gethx()*gethw()) && 
			(gethy()*p.gethw()==p.gethy()*gethw()) );
	};
	/** Operador de desigualdad
	 */
	bool operator!=(const Point2d& p) const{
		return !((*this)==p);
	};

public:
	/** Retorna la coordenada x del punto (cartesiano)
	 */
	const TData getx() const {
		return (gethx()/gethw());
	};
	/** Retorna la coordenada y del punto (cartesiano)
	 */
	const TData gety() const {
		return (gethy()/gethw());
	};
	/** Retorna la coordenada x del punto (homogeneo)
	 */
	const TData gethx() const {
		return this->getData(0);
	};
	/** Retorna la coordenada y del punto (homogeneo)
	 */
	const TData gethy() const {
		return this->getData(1);
	};
	/** Retorna la coordenada homogenea del punto
	 */
	const TData gethw() const {
		return this->getData(2);
	};
	/** Retorna la i-esima coordenada cartesiana del punto
	 */
	const TData getCartesian(const int& i) const{
		switch(i) {
			case 0: return getx();
			case 1: return gety();
		}
		return 0;
	}
	/** Retorna la i-esima coordenada homogenea del punto
	 */
	const TData getHomogeneous(const int& i) const{
		switch(i) {
			case 0: return gethx();
			case 1: return gethy();
			case 2: return gethw();
		}
		return 0;
	}
	/** Retorna la dimensión del punto
	 */
	unsigned int getDim()	{
    	return VectorVectorial<TVectorialSpace>::getDim()-1;
	}

private:
	/**
	 */
	VectorExpr<BinaryOperation< TData* , TData*,DApAdd<TData>,TData>,TData>
	operator+(const Point2d<TAffineSpace>& a)	{
		return *this;
	}
	/**
	 */
	VectorExpr<BinaryOperation<ExprLiteral<TData>,TData*,DApMultiply<TData>,TData>,TData>		
	operator*(const TData& a) {
  		return *this;
	}
};

CGL_END

#endif