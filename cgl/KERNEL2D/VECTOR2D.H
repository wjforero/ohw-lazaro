/**
 * $Revision: 1.1 $
 * $Date: 2003/03/11 20:55:39 $
 */

#ifndef VECTOR2D_H
#define VECTOR2D_H

#include "base.h"

CGL_BEGIN

/** Define un vector afin en un espacio 2d.
 *	En realidad es un vector de un espacio euclidiano de dimension 3
 */
template <class TAffineSpace>
class Vector2d : public TAffineSpace::TVectorialSpaceSelf::TVector {
public:			
	/** Tipo del espacio vectorial al cual pertenece
	 */
	typedef typename TAffineSpace::TVectorialSpaceSelf TVectorialSpace;
	/**	Tipo de dato del vector
	 */
	typedef typename TAffineSpace::TVectorialSpaceSelf::TData TData;
	/**
	 */
	typedef typename TAffineSpace::TVectorialSpaceSelf::TVector TVectorBase;
	/**
	 */
	typedef typename TAffineSpace::TMatrix TMatrix;
public:
	/** Constructor por omisión
	 */
	Vector2d() : TVectorBase() {
		(*this)[0]=0;
		(*this)[1]=0;
		(*this)[TAffineSpace::dim]=0;
	}
	/** Constructor de copia
	 *	@param v Vector a partir del cual se construye
	 */
	Vector2d(const Vector2d& v) : TVectorBase()	{	
		copy(v);				
	}
	/** Construye un vector con coordenadas (hx,hy,1)
	 */
	Vector2d(const TData& hx, const TData& hy) : TVectorBase() {
    (*this)[0]=hx;
		(*this)[1]=hy;
		(*this)[TAffineSpace::dim]=0;
	}
	/** Construye un vector con coordenadas (hx,hy,hw)
	 */
	Vector2d(const TData& hx, const TData& hy, const TData& hw) : TVectorBase() {
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
	 *  @param x lado derecho en la expresión
	 */
  Vector2d<TAffineSpace>& operator=(const VectorAssignable<TData>& x)	{
		x.assign(*this);
		return *this;
	};

	/** Operador de igualdad
	 */
	bool operator==(const Vector2d& v) const{
		return (  (gethx()*v.gethw() == v.gethx()*gethw()) && 
							(gethy()*v.gethw() == v.gethy()*gethw()) && 
							(gethz()*v.gethw() == v.gethz()*gethw()) );
	};
	/** Operador de desigualdad
	 */
	bool operator!=(const Vector2d& v) const{
		return !((*this)==v);
	};

public:
	/** Retorna la coordenada x del vector (cartesiano)
	 */
	const TData getx() const {
		return (gethx()/gethw());
	};
	/** Retorna la coordenada y del vector (cartesiano)
	 */
	const TData gety() const {
		return (gethy()/gethw());
	};
	/** Retorna la coordenada x del vector (homogeneo)
	 */
	const TData gethx() const {
		return this->getData(0);
	};
	/** Retorna la coordenada y del vector (homogeneo)
	 */
	const TData gethy() const {
		return this->getData(1);
	};
	/** Retorna la coordenada homogenea del vector
	 */
	const TData gethw() const {
		return this->getData(2);
	};
	/** Retorna la i-esima coordenada cartesiana del vector
	 */
	const TData getCartesian(const int& i) const{
		switch(i) {
			case 0: return getx();
			case 1: return gety();
		}
		return 0;
	}
	/** Retorna la i-esima coordenada homogenea del vector
	 */
	const TData getHomogeneous(const int& i) const{
		switch(i) {
			case 0: return gethx();
			case 1: return gethy();
			case 3: return gethw();
		}
		return 0;
	}

public:
	/** Retorna la dimensión del espacio afin que lo contiene
	 *	@return Dimension del espacio afín que lo contiene
	 */
	const unsigned int getDim() const {
  	return VectorVectorial<TVectorialSpace>::getDim()-1;
	}

public:
	/** Producto interno de dos vectores
	 * @param p vector izquierdo a la operación
	 * @param q vector derecho a la operación
	 * @param ip variable donde se almacenará el resultado
	 */
	static void innerProduct(const Vector2d& p, const Vector2d& q, TData& ip) {
		Vector2d pq;
		pq = p*q;
		ip = pq[0]+pq[1];
	};
	/** Producto externo de dos vectores
	 */
	static void outerProduct(const Vector2d& p, const Vector2d& q, TMatrix& out) {
		out[0][0]=p.gethx()*q.gethx(); out[0][1]=p.gethx()*q.gethy(); 
		out[1][0]=p.gethy()*q.gethx(); out[1][1]=p.gethy()*q.gethy(); 
	};
};

CGL_END

#endif