/**
 * $Revision: 1.1 $
 * $Date: 2003/03/11 20:55:40 $
 */

#ifndef AFFTRANSFORMATION3D_H
#define AFFTRANSFORMATION3D_H

#include "base.h"

CGL_BEGIN

template <class TAffineSpace>
class AffTransformation3d : public TAffineSpace::TransformationAffine {
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
	AffTransformation3d() : TAffineSpace::TransformationAffine(), m_inversetransformation(0), m_isInvertible(0) {
	}
	/**
	 */
	AffTransformation3d(const TAffineFrame& f,const TAffinePoint& o,TAffinePoint* v[])	{			
	}		
	/**
	 */
	AffTransformation3d(const TAffineFrame& f1,const TAffineFrame& f2)	{
	}
	/**
	 */
	AffTransformation3d(const AffTransformation3d<TAffineSpace>& f)	{
	}

public:		
	// Suceptibles a implementación de expression templates
	/**
	 */
	AffTransformation3d<TAffineSpace> compose(const AffTransformation3d<TAffineSpace>& t);		
	/**
	 */
	AffTransformation3d<TAffineSpace> inverse(const AffTransformation3d<TAffineSpace>& t1);
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