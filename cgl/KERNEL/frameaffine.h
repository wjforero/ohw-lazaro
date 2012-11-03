#ifndef FRAMEAFFINE_H
#define FRAMEAFFINE_H

// inclusiones especificas
#include "base.h"

CGL_BEGIN

/** Esta clase modela un marco de referencia de un espacio afín
 */
template <class TCGAfinneSpace>
class FrameAffine {
public:
	/** Tipo de dato de la matriz
	 */
	typedef typename TCGAfinneSpace::Matrix TMatrix;
	/** Tipo de dato del espacio afín
	 */
	typedef TCGAfinneSpace TAffineSpace;
	/** Tipo de dato del punto afín
	 */
	typedef typename TCGAfinneSpace:: AffinePoint TAffinePoint;
	/** Tipo de dato del vector afín
	 */
	typedef typename TCGAfinneSpace:: AffineVector TAffineVector;

public:
	/** Construye el frame estandar del espacio		
	 */
	FrameAffine() : m_matrixStdToFrame(TCGAfinneSpace::dim+1),m_matrixFrameToStd(TCGAfinneSpace::dim+1)	{   			
		m_matrixStdToFrame.setIdentity();
		m_matrixFrameToStd.setIdentity();
	}
	/**	Construye un frame a partir de un punto origen y un conjunto de vectores base
	 *	@param o punto origen
	 *	@param basis conjunto de vectores base
	 */
	FrameAffine(const TAffinePoint& o,TAffineVector* basis[]) : m_matrixStdToFrame(TCGAfinneSpace::dim+1),m_matrixFrameToStd(TCGAfinneSpace::dim+1)	{			
		int i,j;
		m_matrixStdToFrame.newsize(TCGAfinneSpace::dim+1, TCGAfinneSpace::dim+1);

		// Define una matriz de cambio de coordenadas
		for (i=0;i<TCGAfinneSpace::dim;i++) {
			for (j=0;j<TCGAfinneSpace::dim+1;j++) {
				m_matrixStdToFrame[i][j]=basis[i]->operator[](j);
			}
		}
		for (j=0;j<TCGAfinneSpace::dim+1;j++) {
			m_matrixStdToFrame[TCGAfinneSpace::dim][j]=o.getData(j);
		}
		m_matrixFrameToStd.newsize(TCGAfinneSpace::dim+1,TCGAfinneSpace::dim+1);
		m_isInvertible=m_matrixFrameToStd.inverse(m_matrixFrameToStd);
	}
	/** Destruye el frame afín
	 */
	~FrameAffine() {
		destroy();
	}

public:
	/**	Retorna el iesimo vector base
	 *	@param i indice del vector base
	 */		
	TAffineVector getBasis(const unsigned int& i) const {
		TAffineVector v;
		int j;
		v[0]=1;			
		for(j=0;j<TCGAfinneSpace::dim+1;j++) {				
			v[j]=m_matrixStdToFrame[i][j];
		}			
		return v;
	}
	/**	Retorna el punto origen
	 */
	TAffinePoint getOrigin() const {
		TAffinePoint p;
		int j;
		for(j=0;j<TCGAfinneSpace::dim+1;j++) {
			p[j]=m_matrixStdToFrame[TCGAfinneSpace::dim][j];
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

protected:		
	/** Copia una matriz
	 */
	void copy(const FrameAffine<TCGAfinneSpace>& frame)	{
		m_matrixStdToFrame=frame.m_matrixStdToFrame;
		m_matrixFrameToStd=frame.m_matrixFrameToStd;
	}
	/** Destruye la matriz, proveida por compatibilidad
	 */
	void destroy() {
	}		

private:		
	/** Matriz de transformación del marco estandar al marco
	 */
	TMatrix m_matrixStdToFrame;
	/** Matriz de transformación del marco al estandar
	 */
	TMatrix m_matrixFrameToStd;
	/** Marca para ver si es invertible
	 */
	bool m_isInvertible;
};

CGL_END

#endif
