/**
 * $Revision: 1.2 $
 * $Date: 2003/03/18 12:36:06 $
 */

#ifndef VOLUMETRICSPACE_H
#define VOLUMETRICSPACE_H

#include "base.h"
#include "structures/quadtree.h"
#include "structures/octree.h"
#include "marchingcubes.h"
#include "marchingquads.h"

CGL_BEGIN

template<class TPAffineSpace>
class VolumetricSpace2d {
public:
	/**
	 */
	typedef VolumetricSpace2d<TPAffineSpace> TSelf;
	/**
	 */
	typedef TPAffineSpace TAffineSpace;
	/**
	 */
	typedef typename TAffineSpace::TData TData;
	/**
	 */
	typedef HoppeVertex<TSelf> TVertex;
	/**
	 */
	typedef Quadtree<TAffineSpace,TVertex*> TSpatialStructure;
	/**
	 */
	typedef MarchingQuads<TSelf> TMarchingCells;
  /**
   */
  enum {dim=2};
  /**
   */
  enum {dimc=4};
};

template<class TPAffineSpace>
class VolumetricSpace3d {
public:
	public:
	/**
	 */
	typedef VolumetricSpace3d<TPAffineSpace> TSelf;
	/**
	 */
	typedef TPAffineSpace TAffineSpace;
	/**
	 */
	typedef typename TAffineSpace::TData TData;
	/**
	 */
	typedef HoppeVertex<TSelf> TVertex;
	/**
	 */
	typedef Octree<TAffineSpace,TVertex*> TSpatialStructure;
	/**
	 */
	typedef MarchingCubes<TSelf> TMarchingCells;
  /**
   */
  enum {dim=3};
  /**
   */
  enum {dimc=8};
};

CGL_END

#endif