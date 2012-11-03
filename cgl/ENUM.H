/**
 * $Revision: 1.1 $
 * $Date: 2003/03/11 20:55:29 $
 */

#ifndef ENUM_H
#define ENUM_H

#include "base.h"

CGL_BEGIN

enum OrientedSide{ON_NEGATIVE_SIDE = -1,ON_ORIENTED_BOUNDARY,ON_POSITIVE_SIDE};

enum BoundedSide{ON_UNBOUNDED_SIDE = -1, ON_BOUNDARY, ON_BOUNDED_SIDE};

enum ComparisonResult{SMALLER = -1, EQUAL, LARGER};

enum Sign{NEGATIVE = -1, ZERO, POSITIVE};
typedef Sign Orientation;

const Orientation  LEFTTURN = POSITIVE;
const Orientation  RIGHTTURN = NEGATIVE;

const Orientation  CLOCKWISE = NEGATIVE;
const Orientation  COUNTERCLOCKWISE = POSITIVE;

const Orientation  COLLINEAR = ZERO;
const Orientation  COPLANAR = ZERO;
const Orientation  DEGENERATE = ZERO;

CGL_END

#endif

