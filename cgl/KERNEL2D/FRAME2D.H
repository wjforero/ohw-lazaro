/**
 * $Revision: 1.1 $
 * $Date: 2003/03/11 20:55:39 $
 */

#ifndef FRAME2D_H
#define FRAME2D_H

#include "base.h"

CGL_BEGIN

/**	Esta clase modela un marco de referencia de un espacio afín
 */
template<class TAffineSpace>
class Frame2d {
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
	Frame2d() : m_matrixStdToFrame(TAffineSpace::dim+1),m_matrixFrameToStd(TAffineSpace::dim+1) {   			
		m_matrixStdToFrame.setIdentity();
		m_matrixFrameToStd.setIdentity();
	}
	/**	Construye un frame a partir de un punto origen y un conjunto de vectores base
	 *	@param o punto origen
	 *	@param basis conjunto de vectores base
	 */
	Frame2d(const TAffinePoint& o,TAffineVector* basis[]) : m_matrixStdToFrame(TAffineSpace::dim+1),m_matrixFrameToStd(TAffineSpace::dim+1) {
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
	~Frame2d() {
		destroy();
	}

public:
	/**	Retorna el iesimo vector base
	 *	@param i indice del vector base
	 */		
	TAffineVector getBasis(const unsigned int& i) const	{
		TAffineVector v;
		int j;
		v[0]=1;			
		for(j=0;j<TAffineSpace::dim+1;j++) {
			v[j]=m_matrixStdToFrame[i][j];
		}			
		return v;
	}
	/**	Retorna el punto origen
	 */
	TAffinePoint getOrigin() const {
		TAffinePoint p;
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
		if( (m_matrixStdToFrame[0][0]*m_matrixStdToFrame[1][1]) - 
			(m_matrixStdToFrame[1][0]*m_matrixStdToFrame[0][1]) > 0 ) {
			m_matrixStdToFrame[1][0] = -m_matrixStdToFrame[1][0];
			m_matrixStdToFrame[1][1] = -m_matrixStdToFrame[1][1];
		}
	}
protected:		
	/**
	 */
	void copy(const Frame2d<TAffineSpace>& frame) {
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