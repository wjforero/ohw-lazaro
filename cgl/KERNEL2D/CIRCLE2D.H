/**
 * $Revision: 1.1 $
 * $Date: 2003/03/11 20:55:39 $
 */

#ifndef CIRCLE2D_H
#define CIRCLE2D_H

#include "base.h"
#include "predicates2d.h"

CGL_BEGIN

/** Representa un circulo en un espacio afin 2d.
 *  Un circulo esta representado por un centro y un radio.
 */
template<class TAffineSpace>
class Circle2d 
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
	/**
	 */
	typedef typename TAffineSpace::TAffinePoint TAffinePoint;

public:
	/** Constructor por defecto
	 */
	Circle2d();
	/** Constructor de copia
	 *  @param c circulo a copiar
	 */
	Circle2d(const Circle2d<TAffineSpace>& c);
	/** Construye un circulo con el centro center y radio al cuadrado 
	 *  squaredRadius
	 *  @param center centro del circulo
	 *  @param squaredRadius radio del circulo elevado al cuadrado
	 */
	Circle2d(const TAffinePoint& center, const TDataType squaredRadius);
	/** Crea un circulo que pasa por los puntos a, b y c.
	 *  @param a 
	 *  @param b
	 *  @param c
	 */
	Circle2d(const TAffinePoint& a, const TAffinePoint& b,const TAffinePoint& c);
	/** Destructor
	 */
	~Circle2d();

public:
	/** Operador de asignación
	 */
	Circle2d<TAffineSpace>& operator= (const Circle2d<TAffineSpace>& c);
	/** Comparador de igualdad
	 */
	bool operator== (const Circle2d<TAffineSpace>& c) const;
	/** Comparador de desigualdad
	 */
	bool operator!= (const Circle2d<TAffineSpace>& c) const;

public:
	/** Retorna el centro del circulo
	 */
	const TAffinePoint& getCenter() const;
	/** Retorna el radio al cuadrado del circulo
	 */
	const TDataType& getSquaredRadius() const;

private:
	/** Centro del circulo
	 */
	TAffinePoint m_center;
	/** Radio al cuadrado del circulo
	 */
	TDataType m_squaredRadius;

};

/** Constructor por defecto
 */
template<class TAffineSpace>
Circle2d<TAffineSpace>::Circle2d() {
}

/** Constructor de copia
 *  @param c circulo a copiar
 */
template<class TAffineSpace>
Circle2d<TAffineSpace>::Circle2d(const Circle2d<TAffineSpace>& c)
: m_center(c.m_center), m_squaredRadius(c.m_squaredRadius){
}
	
/** Construye un circulo con el centro center y radio al cuadrado 
 *  squaredRadius
 *  @param center centro del circulo
 *  @param squaredRadius radio del circulo elevado al cuadrado
 */
template<class TAffineSpace>
Circle2d<TAffineSpace>::Circle2d(const TAffinePoint& center, const TDataType squaredRadius)
: m_center(center), m_squaredRadius(squaredRadius){
}

/** Crea un circulo que pasa por los puntos a, b y c.
 *  @param a 
 *  @param b
 *  @param c
 */
template<class TAffineSpace>
Circle2d<TAffineSpace>::Circle2d(const TAffinePoint& a, const TAffinePoint& b,const TAffinePoint& c){
	m_center=Predicates2d<TAffineSpace>::circumcenter(a,b,c);
	m_squaredRadius=Predicates2d<TAffineSpace>::squaredDistance(a,m_center);
}

/** Destructor
 */
template<class TAffineSpace>
Circle2d<TAffineSpace>::~Circle2d(){
}

/** Operador de asignación
 */
template<class TAffineSpace>
Circle2d<TAffineSpace>& Circle2d<TAffineSpace>::operator= (const Circle2d<TAffineSpace>& c){
	m_center=c.m_center;
	m_squaredRadius=c.m_squaredRadius;
}

/** Comparador de igualdad
 */
template<class TAffineSpace>
bool Circle2d<TAffineSpace>::operator== (const Circle2d<TAffineSpace>& c) const{
	return (m_center==c.m_center &&	
		m_squaredRadius==c.m_squaredRadius);
}

/** Comparador de desigualdad
 */
template<class TAffineSpace>
bool Circle2d<TAffineSpace>::operator!= (const Circle2d<TAffineSpace>& c) const{
	return !((*this)==c);
}

/** Retorna el centro del circulo
 */
template<class TAffineSpace>
const Circle2d<TAffineSpace>::TAffinePoint& Circle2d<TAffineSpace>::getCenter() const{
	return m_center;
}

/** Retorna el radio al cuadrado del circulo
 */
template<class TAffineSpace>
const Circle2d<TAffineSpace>::TDataType& Circle2d<TAffineSpace>::getSquaredRadius() const{
	return m_squaredRadius;
}

CGL_END

#endif