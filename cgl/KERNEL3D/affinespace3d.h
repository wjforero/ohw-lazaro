/**
 * $Revision: 1.2 $
 * $Date: 2003/03/18 12:36:04 $
 */

#ifndef AFFINESPACE3D_H
#define	AFFINESPACE3D_H

// inclusiones especificas
#include "base.h"
#include "vector3d.h"
#include "point3d.h"
#include "affinetransformation3d.h"
#include "frame3d.h"
#include "predicates3d.h"

CGL_BEGIN

/**	Esta clase modela una geometría afin 3d, mediante su algebra.
 */
template <class TData>
class AffineSpace3d: public CGVectorialSpace<TData,4> {

public:
	/**
	 */
	enum{dim=3};
	/**
	 */
	typedef TData TData;
	/**
	 */
	typedef AffineSpace3d<TData> TTypeSelf;
	/**
	 */
	typedef CGVectorialSpace<TData,4> TVectorialSpaceSelf;
	/**
	 */
	typedef typename Vector3d< TTypeSelf > TAffineVector;
	/**
	 */
	typedef typename Point3d<TTypeSelf> TAffinePoint;
	/**
	 */
	typedef Frame3d<TTypeSelf> TAffineFrame;
	/**
	 */
	typedef typename TVectorialSpaceSelf::TMatrix TMatrix;
	/**
	 */
	typedef Predicates3d<TTypeSelf> TPredicates;

public:
	/**
	 */
	AffineSpace3d() : TVectorialSpaceSelf("Espacio Afin 3d")	{			
	}	
	/**
	 */
	AffineSpace3d(string strName) : TVectorialSpaceSelf(strName)	{			
	}
	/**
	 */
	unsigned int getDim() const;		
	/**
	 */
	TAffineVector getVector(const TData& x, const TData& y, const TData& z) const;
	/**
	 */
	TAffinePoint getPoint(const TData& x, const TData& y, const TData& z) const;
	/**
	 */
	TAffinePoint getOriginPoint() const;
	/**
	 */
	TAffineFrame getFrame() const;

};

/** Retorna la dimension del espacio
 */
template <class TData>
inline unsigned int	AffineSpace3d<TData>::getDim() const {
	return m_dim-1;
}

/**
 */
template <class TData>
AffineSpace3d<TData>::TAffineVector AffineSpace3d<TData>::getVector(const TData& x, const TData& y, const TData& z) const {
	TAffineVector r(x,y,z);
	return r;
}

/**
 */
template <class TData>
AffineSpace3d<TData>::TAffinePoint AffineSpace3d<TData>::getPoint(const TData& x, const TData& y, const TData& z) const {
	TAffinePoint r(x,y,z);
	return r;
}

/**
 */
template <class TData>
AffineSpace3d<TData>::TAffinePoint AffineSpace3d<TData>::getOriginPoint() const {
	TAffinePoint r(0,0,0);
	return r;
}

/**
 */
template <class TData>
AffineSpace3d<TData>::TAffineFrame AffineSpace3d<TData>::getFrame() const {
	TAffineFrame r;
	return r;
}

CGL_END

#endif