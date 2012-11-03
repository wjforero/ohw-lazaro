/**
 * $Revision: 1.1 $
 * $Date: 2003/03/11 20:55:39 $
 */

#ifndef AFFTRANSFORMATION2D_H
#define AFFTRANSFORMATION2D_H

#include "base.h"

CGL_BEGIN

template <class TAffineSpace>
class AffTransformation2d : public TAffineSpace::TransformationAffine {
public:
	/**
	 */
	typedef typename TAffineSpace::Matrix TMatrix;
	/**
	 */
	typedef typename TAffineSpace::TAffineFrame TAffineFrame;
	/**
	 */
	typedef typename TAffineSpace::TAffinePoint TAffinePoint;
	/**
	 */
	typedef typename TAffineSpace::TAffineVector TAffineVector;

public:
	/**
	 */
	AffTransformation2d() : TAffineSpace::TransformationAffine(), m_inversetransformation(0), m_isInvertible(0) {
	}
	/**
	 */
	AffTransformation2d(const TAffineFrame& f,const TAffinePoint& o,TAffinePoint* v[])	{			
	}		
	/**
	 */
	AffTransformation2d(const TAffineFrame& f1,const TAffineFrame& f2)	{
	}
	/**
	 */
	AffTransformation2d(const AffTransformation2d<TAffineSpace>& f)	{
	}

public:		
	// Suceptibles a implementación de expression templates
	/**
	 */
	AffTransformation2d<TAffineSpace> compose(const AffTransformation2d<TAffineSpace>& t);		
	/**
	 */
	AffTransformation2d<TAffineSpace> inverse(const AffTransformation2d<TAffineSpace>& t1);
	/**
	 */
	TAffinePoint transform(const TAffinePoint& p )const;
	/**
	 */
	TAffineVector transform(const TAffineVector& p )const;

private:
	/**
	 */
	TMatrix* m_inversetransformation;
	/**
	 */
	bool m_isInvertible;
};

CGL_END

#endif