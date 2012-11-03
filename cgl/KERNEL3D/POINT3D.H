/**
 * $Revision: 1.2 $
 * $Date: 2003/03/18 12:36:05 $
 */

#ifndef POINT3D_H
#define POINT3D_H

#include "base.h"

CGL_BEGIN

/** Define un punto en un espacio afin 2d.
 *	En realidad es un vector de un espacio vectorial de dimension 3
 */
template <class TAffineSpace>
class Point3d : public TAffineSpace::TVectorialSpaceSelf::TVector {

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
	Point3d() : TVectorBase() {
		(*this)[0]=0;
		(*this)[1]=0;
		(*this)[2]=0;
		(*this)[TAffineSpace::dim]=1;
	}
	/** Constructor de copia
	 */
	Point3d(const Point3d& p) : TVectorBase() {				
		(*this).copy(p);			
	}
	/** Construye un punto con coordenadas (hx,hy,hz,1)
	 */
	Point3d(const TData& hx, const TData& hy, const TData& hz) : TVectorBase() {
    (*this)[0]=hx;
		(*this)[1]=hy;
		(*this)[2]=hz;
		(*this)[TAffineSpace::dim]=1;
	}
	/** Construye un punto con coordenadas (hx,hy,hz,hw)
	 */
	Point3d(const TData& hx, const TData& hy, const TData& hz, const TData& hw) : TVectorBase() {
		if(hw >= 0){
			(*this)[0]=hx;
			(*this)[1]=hy;
			(*this)[2]=hz;
			(*this)[TAffineSpace::dim]=hw;
		}else{
			(*this)[0]=-hx;
			(*this)[1]=-hy;
			(*this)[2]=-hz;
			(*this)[TAffineSpace::dim]=-hw;
		}
	}
	
public:
	/** Operador de asignación
	 */
	Point3d<TAffineSpace>& operator=(const VectorAssignable<TData>& x) {
		x.assign(*this);
		return *this;
	}
	/** Operador de igualdad
	 */
	bool operator==(const Point3d& p) const{
		return ( (gethx()*p.gethw() == p.gethx()*gethw()) && 
			(gethy()*p.gethw() == p.gethy()*gethw()) && 
			(gethz()*p.gethw()==p.gethz()*gethw()) );
	};
	/** Operador de desigualdad
	 */
	bool operator!=(const Point3d& p) const{
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
	/** Retorna la coordenada z del punto (cartesiano)
	 */
	const TData getz() const {
		return (gethz()/gethw());
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
	/** Retorna la coordenada z del punto (homogeneo)
	 */
	const TData gethz() const {
		return this->getData(2);
	};
	/** Retorna la coordenada homogenea del punto
	 */
	const TData gethw() const {
		return this->getData(3);
	};
	/** Retorna la i-esima coordenada cartesiana del punto
	 */
	const TData getCartesian(const int& i) const{
		switch(i) {
			case 0: return getx();
			case 1: return gety();
			case 2: return getz();
		}
		return 0;
	}
	/** Retorna la i-esima coordenada homogenea del punto
	 */
	const TData getHomogeneous(const int& i) const{
		switch(i) {
			case 0: return gethx();
			case 1: return gethy();
			case 2: return gethz();
			case 3: return gethw();
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
	operator+(const Point3d<TAffineSpace>& a)	{
		return *this;
	}
	/**
	 */
	VectorExpr<BinaryOperation<ExprLiteral<TData>,TData*,DApMultiply<TData>,TData>,TData>		
	operator*(const TData& a) {
  		return *this;
	}
	// Oculta las implementaciones de los operadores para se ajusten bien al algebra del espacio vectorial
	/**
	 */
	/*VectorExpr<BinaryOperation< Point3d<TAffineSpace>::TData* ,Point3d<TAffineSpace>::TData*,DApAdd<Point3d<TAffineSpace>::TData>,Point3d<TAffineSpace>::TData>,Point3d<TAffineSpace>::TData>
	operator+(const Point3d<TAffineSpace>& a)	{
		return *this;
	}
	/**
	 */
	/*VectorExpr<BinaryOperation<ExprLiteral<Point3d<TAffineSpace>::TData>,Point3d<TAffineSpace>::TData*,DApMultiply<Point3d<TAffineSpace>::TData>,Point3d<TAffineSpace>::TData>,Point3d<TAffineSpace>::TData>
	operator*(const TData& a) {
  		return *this;
	}*/
};

CGL_END

#endif