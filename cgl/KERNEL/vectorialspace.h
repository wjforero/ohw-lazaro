#ifndef VECTORIALSPACE_H
#define VECTORIALSPACE_H
	
// Inclusiones especificas
#include "base.h"
#include "space.h"
#include "numericalvector.h"
#include "vectorvectorial.h"

#include "algebra.h"
using namespace ohw::numerical;

CGL_BEGIN

/**	Modela un espacio vectorial de dimension fínita.
 *	Un algebra líneal puede modelarse a traves de un conjunto de operadores y de algebra para los operadores.
 *	Además el algebra debe cumplir con una serie de axiomas. La clase además provee matrices para las
 *	respectivas transformaciones
 */
template <class TData, unsigned int _dim=2>
class CGVectorialSpace : public CGSpace {
public:
	/* Dimensión del espacio.
	 */
	enum{dim=_dim};
	/** Tipo de los datos vectoriales.
	 */
	typedef TData TData;
	/** Tipo del espacio vectorial que contiene.
	 */
	typedef typename CGVectorialSpace<TData,dim> TVectorialSpaceSelf;
	/** Tipo Vector de un espacio vectorial.
	 */
	typedef typename VectorVectorial<TVectorialSpaceSelf> TVector;
	/** Tipo Matrix de un espacio vectorial.
	 */
  typedef typename Algebra<TData>::TMatrix TMatrix;
	/** Construye un espacio vectorial con la dimension obtenida por el template.
	 */

public:
	/**
	 */
	CGVectorialSpace() : CGSpace("Espacio Vectorial"), m_dim(dim) {
	}		
	/** Construye un espacio con el nombre especificado.
	 *	@param strName Nombre del espacio vectorial.
	 */			
	CGVectorialSpace(string strName) : CGSpace(strName), m_dim(dim) {
	}
	/** Construye un espacio vectorial con la dimension obtenida por el constructor.
	 *	@param _dim Dimension del espacio vectorial.
	 */
	CGVectorialSpace(const unsigned int& _dim) : CGSpace("Espacio Vectorial"), m_dim(_dim) {
	}
	/** Factoria de Vectores.
	 *	@return Retorna un vector de este tipo de espacio.		
	 */
	TVector getVector() const;
	/** Retorna la dimension del espacio vectorial.
	 *	@return Retorna la dimension del espacio.
	 */
	unsigned int getDim() const;

protected:
	/** Dimensión del espacio vectorial
	 */
	unsigned int m_dim;

};


/** Retorna la dimension del espacio
 */
template <class TData, unsigned int _dim>
inline unsigned int	CGVectorialSpace<TData,_dim>::getDim() const {
	return m_dim;
}

/** Factoria de vectores
 */
template <class TData, unsigned int _dim>
CGVectorialSpace<TData,_dim>::TVector CGVectorialSpace<TData,_dim>::getVector() const {
	TVector r;
	return r;
}	

CGL_END

#endif

