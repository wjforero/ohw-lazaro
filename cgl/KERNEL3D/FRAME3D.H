/**
 * $Revision: 1.2 $
 * $Date: 2003/03/18 12:36:05 $
 */

#ifndef FRAME3D_H
#define FRAME3D_H

#include "base.h"

CGL_BEGIN

/**	Esta clase modela un marco de referencia de un espacio afín
 */
template<class TAffineSpace>
class Frame3d {
public:
	/** Tipo de dato del espacio afín
	 */
	typedef typename TAffineSpace TAffineSpace;
	/** Tipo de dato de la matriz
	 */
	typedef typename TAffineSpace::TMatrix TMatrix;
	/** Tipo de dato del punto afín
	 */
	typedef typename TAffineSpace::TAffinePoint TAffinePoint;
	/** Tipo de dato del vector afín
	 */
	typedef typename TAffineSpace::TAffineVector TAffineVector;

public:
	/** Construye el frame estandar del espacio		
	 */
	Frame3d() : m_matrixStdToFrame(TAffineSpace::dim+1),m_matrixFrameToStd(TAffineSpace::dim+1) {   			
		m_matrixStdToFrame.setIdentity();
		m_matrixFrameToStd.setIdentity();
	}
	/**	Construye un frame a partir de un punto origen y un conjunto de vectores base
	 *	@param o punto origen
	 *	@param basis conjunto de vectores base
	 */
	Frame3d(const TAffinePoint& o,TAffineVector* basis[]) : m_matrixStdToFrame(TAffineSpace::dim+1),m_matrixFrameToStd(TAffineSpace::dim+1) {
		int i,j;
		m_matrixStdToFrame.newsize(TAffineSpace::dim+1, TAffineSpace::dim+1);

		// Define una matriz de cambio de coordenadas
		for (i=0;i<TAffineSpace::dim;i++) {
			for (j=0;j<TAffineSpace::dim+1;j++) {
				m_matrixStdToFrame[i][j]=basis[i]->operator[](j);
			}
		}

		for (j=0;j<TAffineSpace::dim+1;j++) {
			m_matrixStdToFrame[TAffineSpace::dim][j]=o.getData(j);
		}
		
		m_matrixFrameToStd.newsize(TAffineSpace::dim+1,TAffineSpace::dim+1);
		m_isInvertible=m_matrixFrameToStd.inverse(m_matrixFrameToStd);
	}

	/** Destruye el frame afín
	 */
	~Frame3d() {
		destroy();
	}

public:
	/**	Retorna el iesimo vector base
	 *	@param i indice del vector base
	 */		
	TAffineVector& getBasis(const unsigned int& i) const	{
		TAffineVector& v = *new TAffineVector();
		int j;
		v[0]=1;			
		for(j=0;j<TAffineSpace::dim+1;j++) {
			v[j]=m_matrixStdToFrame[i][j];
		}			
		return v;
	}
	/**
	 *	Retorna el punto origen
	 */
	TAffinePoint& getOrigin() const {
		TAffinePoint& p = *new TAffinePoint();
		int j;
		for(j=0;j<TAffineSpace::dim+1;j++) {
			p[j]=m_matrixStdToFrame[TAffineSpace::dim][j];
		}
		return p;
	}
	/** Retorna la matriz de transformación del estandar al frame
	 */
	const TMatrix& getStdToFrame() const {
		return m_matrixStdToFrame;
	}
	/** Retorna la matriz de frame al estandzar
	 */
	const TMatrix& getFrameToStd() const {
		return m_matrixFrameToStd;
	}
	/** Retorna un boolean que indica si el frame tiene trasformada inversa
	 *	@return true si tiene inversa, false en caso contrario.
	 */
	bool getIsInvertible() const {
		return m_isInvertible;
	}
public:
	/**
	 */
	void makeRightHand() {
    TAffineVector z;
    TAffineVector::crossProduct(getBasis(0),getBasis(1),z);
    TAffineSpace::TData dot; TAffineVector::innerProduct(z,getBasis(2),dot);
		if( dot < 0 ) {
      for(unsigned int j=0;j<TAffineSpace::dim+1;j++) {
			  m_matrixStdToFrame[2][j] = -m_matrixStdToFrame[2][j];
		  }	
		}
	}
protected:		
	/**
	 */
	void copy(const Frame3d<TAffineSpace>& frame) {
		m_matrixStdToFrame=frame.m_matrixStdToFrame;
		m_matrixFrameToStd=frame.m_matrixFrameToStd;
	}
	/**
	 */
	void destroy() {
	}		

private:		
	/**
	 */
	TMatrix m_matrixStdToFrame;
	/**
	 */
	TMatrix m_matrixFrameToStd;
	/**
	 */
	bool m_isInvertible;
};

CGL_END

#endif