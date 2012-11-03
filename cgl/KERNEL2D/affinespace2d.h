/**
 * $Revision: 1.1 $
 * $Date: 2003/03/11 20:55:39 $
 */

#ifndef AFFINESPACE2D_H
#define	AFFINESPACE2D_H

// inclusiones especificas
#include "base.h"
#include "affinetransformation2d.h"
#include "frame2d.h"
#include "point2d.h"
#include "predicates2d.h"
#include "vector2d.h"

CGL_BEGIN

/**	Esta clase modela una geometría afin 2d, mediante su algebra.
 */
template <class TData>
class AffineSpace2d: public CGVectorialSpace<TData,3> {

public:
	/**
	 */
	enum{dim=2};
	/**
	 */
	typedef TData TData;
	/**
	 */
	typedef AffineSpace2d<TData> TTypeSelf;
	/**
	 */
	typedef CGVectorialSpace<TData,3> TVectorialSpaceSelf;
	/**
	 */
	typedef typename Vector2d< AffineSpace2d<TData> > TAffineVector;
	/**
	 */
	typedef typename Point2d<TTypeSelf> TAffinePoint;
	/**
	 */
	typedef Frame2d<TTypeSelf> TAffineFrame;
	/**
	 */
	typedef typename TVectorialSpaceSelf::TMatrix TMatrix;
	/**
	 */
	typedef Predicates2d<TTypeSelf> TPredicates;

public:
	/**
	 */
	AffineSpace2d() : TVectorialSpaceSelf("Espacio Afin 2d")	{			
	}	
	/**
	 */
	AffineSpace2d(const unsigned int& _dim)	{
		setName("Espacio Afin");
	}
	/**
	 */
	AffineSpace2d(string strName) : TVectorialSpaceSelf(strName)	{			
	}
	/**
	 */
	unsigned int getDim() const;		
	/**
	 */
	TAffineVector getVector() const;
	/**
	 */
	TAffineVector getVector(const TData& x, const TData& y) const;
	/**
	 */
	TAffinePoint getOriginPoint() const;
	/**
	 */
	TAffinePoint getPoint(const TData& x, const TData& y) const;
	/**
	 */
	TAffineFrame getFrame() const;

};

/** Retorna la dimension del espacio
 */
template <class TData>
inline unsigned int	AffineSpace2d<TData>::getDim() const {
	return m_dim-1;
}

/**
 */
template <class TData>
AffineSpace2d<TData>::TAffineVector AffineSpace2d<TData>::getVector() const {
	TAffineVector r(0,0);
	return r;
}

/**
 */
template <class TData>
AffineSpace2d<TData>::TAffineVector AffineSpace2d<TData>::getVector(const TData& x, const TData& y) const {
	TAffineVector r(x,y);
	return r;
}

/**
 */
template <class TData>
AffineSpace2d<TData>::TAffinePoint AffineSpace2d<TData>::getOriginPoint() const {
	TAffinePoint r(0,0);
	return r;
}

/**
 */
template <class TData>
AffineSpace2d<TData>::TAffinePoint AffineSpace2d<TData>::getPoint(const TData& x, const TData& y) const {
	TAffinePoint r(x,y);
	return r;
}

/**
 */
template <class TData>
AffineSpace2d<TData>::TAffineFrame AffineSpace2d<TData>::getFrame() const {
	TAffineFrame r;
	return r;
}

CGL_END

#endif