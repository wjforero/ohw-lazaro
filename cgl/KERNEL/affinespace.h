#ifndef AFFINESPACE_H
#define	AFFINESPACE_H

// inclusiones especificas
#include "base.h"
#include "vectorialspace.h"
#include "vectoraffine.h"
#include "pointaffine.h"
#include "vectornormalaffine.h"
#include "transformationprojective.h"
#include "transformationaffine.h"
#include "frameaffine.h"

CGL_BEGIN
/**
 *	Esta clase modela un espacio afin, mediante su algebra.
 */
template <class TData, unsigned int _dim=2>
class CGAffineSpace: public CGVectorialSpace<TData,_dim+1> {
public:
	/**
	 */
	enum{dim=_dim};
	/**
	 */
	typedef TData TData;
	/**
	 */
	typedef CGAffineSpace<TData,_dim> TTypeSelf;
	/**
	 */
	typedef CGVectorialSpace<TData,_dim+1> TVectorialSpaceSelf;
	/**
	 */
	typedef typename VectorAffine<TTypeSelf> TAffineVector;
	/**
	 */
	typedef typename VectorNormalAffine<TTypeSelf> TAffineNormalVector;
	/**
	 */
	typedef typename PointAffine<TTypeSelf> TAffinePoint;
	/**
	 */
	typedef TransformationAffine<TTypeSelf,TTypeSelf> TAffineTransformation;
	/**
	 */
	typedef TransformationProjective<TTypeSelf,TTypeSelf> TProjectiveTransformation;
	/**
	 */
	typedef FrameAffine<TTypeSelf> TAffineFrame;
	/**
	 */
	CGAffineSpace() : TVectorialSpaceSelf("Espacio Afin") {			
	}	
	/**
	 */
	CGAffineSpace(const unsigned int& _dim) : TVectorialSpaceSelf(_dim+1) {
		setName("Espacio Afin");
	}
	/**
	 */
	CGAffineSpace(string strName) : TVectorialSpaceSelf(strName)	{			
	}
	/**
	 */
	unsigned int getDim()const;		
	/** Factoria de vector afin
	 */
	TAffineVector getVector() const;
	/** Factoria de vectores normales
	 */
	TAffineNormalVector getNormalVector() const;
	/** Factoria de puntos afines
	 */
	TAffinePoint getPoint() const;
	/** Factoria de transformaciones afines
	 */
	TAffineTransformation getAffineTransformation() const;
	/** Factoria de transformaciones proyectivas
	 */
	TProjectiveTransformation getProjectiveTransformation() const;
	/** Factoria de marcos afines
	 */
	TAffineFrame getFrame() const;
};


/** Retorna la dimension del espacio
 */
template <class TData, unsigned int _dim>
inline unsigned int	CGAffineSpace<TData,_dim>::getDim() const {
	return m_dim-1;
}

/**
 */
template <class TData, unsigned int _dim>
CGAffineSpace<TData,_dim>::TAffineVector CGAffineSpace<TData,_dim>::getVector() const {
	TAffineVector r;
	return r;
}

/**
 */
template <class TData, unsigned int _dim>
CGAffineSpace<TData,_dim>::TAffineNormalVector CGAffineSpace<TData,_dim>::getNormalVector() const {
	TAffineNormalVector r;
	return r;
}

/**
 */
template <class TData, unsigned int _dim>
CGAffineSpace<TData,_dim>::TAffinePoint CGAffineSpace<TData,_dim>::getPoint() const {
  cout<<"Crea el punto "<<endl;
	TAffinePoint r;
  cout<<"Fin crea el punto "<<endl;
  cout<<r<<endl<<" los datos";
	return r;
}

/**
 */
template <class TData, unsigned int _dim>
CGAffineSpace<TData,_dim>::TAffineTransformation CGAffineSpace<TData,_dim>::getAffineTransformation() const {
	TAffineTransformation at;
	return at;
}

/**
 */
template <class TData, unsigned int _dim>
CGAffineSpace<TData,_dim>::TAffineFrame CGAffineSpace<TData,_dim>::getFrame() const {
	TAffineFrame r;
	return r;
}

/**
 */
template <class TData, unsigned int _dim>
CGAffineSpace<TData,_dim>::TProjectiveTransformation CGAffineSpace<TData,_dim>::getProjectiveTransformation() const {
	TProjectiveTransformation r;
	return r;
}

CGL_END

#endif
