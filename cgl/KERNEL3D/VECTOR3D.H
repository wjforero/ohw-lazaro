/**
 * $Revision: 1.2 $
 * $Date: 2003/03/18 12:36:05 $
 */

#ifndef VECTOR3D_H
#define VECTOR3D_H

#include "base.h"

CGL_BEGIN

/** Define un vector afin en un espacio 3d.
 *	En realidad es un vector de un espacio euclidiano de dimension 4
 */
template <class TAffineSpace>
class Vector3d : public TAffineSpace::TVectorialSpaceSelf::TVector {
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
	Vector3d() : TVectorBase() {
		(*this)[0]=0;
		(*this)[1]=0;
		(*this)[2]=0;
		(*this)[TAffineSpace::dim]=0;
	}
	/** Constructor de copia
	 *	@param v Vector a partir del cual se construye
	 */
	Vector3d(const Vector3d& v) : TVectorBase()	{	
		copy(v);	
	}
	/** Construye un vector con coordenadas (hx,hy,hz,1)
	 */
	Vector3d(const TData& hx, const TData& hy, const TData& hz) : TVectorBase() {
    (*this)[0]=hx;
		(*this)[1]=hy;
		(*this)[2]=hz;
		(*this)[TAffineSpace::dim]=0;
	}
	/** Construye un vector con coordenadas (hx,hy,hz,hw)
	 */
	Vector3d(const TData& hx, const TData& hy, const TData& hz, const TData& hw) : TVectorBase() {
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
	 *  @param x lado derecho en la expresión
	 */
  Vector3d<TAffineSpace>& operator=(const VectorAssignable<TData>& x)	{
		//cout<<"hola"<<endl;
		x.assign(*this);
		return *this;	
	}			
	/** Operador de igualdad
	 */
	bool operator==(const Vector3d& v) const{
		return (  (gethx()*v.gethw() == v.gethx()*gethw()) && 
							(gethy()*v.gethw() == v.gethy()*gethw()) && 
							(gethz()*v.gethw() == v.gethz()*gethw()) );
	};
	/** Operador de desigualdad
	 */
	bool operator!=(const Vector3d& v) const{
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
	/** Retorna la coordenada z del vector (cartesiano)
	 */
	const TData getz() const {
		return (gethz()/gethw());
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
	/** Retorna la coordenada z del vector (homogeneo)
	 */
	const TData gethz() const {
		return this->getData(2);
	};
	/** Retorna la coordenada homogenea del vector
	 */
	const TData gethw() const {
		return this->getData(3);
	};
	/** Retorna la i-esima coordenada cartesiana del vector
	 */
	const TData getCartesian(const int& i) const{
		switch(i) {
			case 0: return getx();
			case 1: return gety();
			case 2: return getz();
		}
		return 0;
	}
	/** Retorna la i-esima coordenada homogenea del vector
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

public:
	/** Retorna la dimensión del espacio afin que lo contiene
	 *	@return Dimension del espacio afín que lo contiene
	 */
	const unsigned int getDim() const {
  	return VectorVectorial<TVectorialSpace>::getDim()-1;
	}

public:
	/** Producto cruz de dos vectores en R^3
	 * @param a vector izquierdo de la operación
	 * @param b vector derecho de la operación
	 * @param cp vector donde se almacenará el producto cruz
	 */
	static void crossProduct(const Vector3d<TAffineSpace>& a, const Vector3d<TAffineSpace>& b, Vector3d<TAffineSpace>& cp) {
		cp[0]=a.gethy()*b.gethz() - a.gethz()*b.gethy();
		cp[1]=a.gethz()*b.gethx() - a.gethx()*b.gethz();
		cp[2]=a.gethx()*b.gethy() - a.gethy()*b.gethx();
		cp[3]=a.gethw()*b.gethw();
	}
  /** Producto interno de dos vectores
	 * @param p vector izquierdo a la operación
	 * @param q vector derecho a la operación
	 * @param ip variable donde se almacenará el resultado
	 */
	static void innerProduct(const Vector3d& p, const Vector3d& q, TData& ip) {
		Vector3d pq;
		pq = p*q;
		ip = pq[0]+pq[1]+pq[2];
	};
	/** Producto externo de dos vectores
	 */
	static void outerProduct(const Vector3d& p, const Vector3d& q, TMatrix& out) {
		out[0][0]=p.gethx()*q.gethx(); out[0][1]=p.gethx()*q.gethy(); out[0][2]=p.gethx()*q.gethz(); out[0][3]=p.gethx()*q.gethw(); 
		out[1][0]=p.gethy()*q.gethx(); out[1][1]=p.gethy()*q.gethy(); out[1][2]=p.gethy()*q.gethz(); out[1][3]=p.gethy()*q.gethw(); 
    out[2][0]=p.gethz()*q.gethx(); out[2][1]=p.gethz()*q.gethy(); out[2][2]=p.gethz()*q.gethz(); out[2][3]=p.gethz()*q.gethw(); 
    out[3][0]=p.gethw()*q.gethx(); out[3][1]=p.gethw()*q.gethy(); out[3][2]=p.gethw()*q.gethz(); out[3][3]=p.gethw()*q.gethw(); 
	};
};

CGL_END

#endif