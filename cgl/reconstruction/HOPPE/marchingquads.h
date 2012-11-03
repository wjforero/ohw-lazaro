/**
 * $Revision: 1.2 $
 * $Date: 2003/03/18 12:36:06 $
 */

#ifndef MARCHINGQUADS_H
#define MARCHINGQUADS_H

#include "base.h"

CGL_BEGIN

template <class TVolumetricSpace>
class MarchingQuads {
public:
	/**
	 */
	typedef typename TVolumetricSpace TVolumetricSpace;
	/**
	 */
	typedef typename TVolumetricSpace::TAffineSpace TAffineSpace;
	/**
	 */
	typedef typename TAffineSpace::TAffineVector TVector;
	/**
	 */
	typedef typename TAffineSpace::TAffinePoint TPoint;
public:
	/**
	 */
	enum VERTEX {NW, NE, SW, SE};
	/*
	 */
	enum EDGE {N, E, S, W};
private:
	/**
	 */
	MarchingQuads() {
	}
public:
	/**
	 */
	static void processCell(const TPoint& pos, const TVector& size, const unsigned char& code) {
		const int* cases = getCase(code);
		TPoint val;
		TVector x(size.gethx()/2,0);
		TVector y(0,size.gethy()/2);
		unsigned int j=0;
		while(cases[j]!=-1 && j<4) {
			val = pos;
			switch(cases[j]) {
				case 0: val = pos+x+y+y; break;
        case 1: val = pos+x+x+y; break;
				case 2: val = pos+x; break;
				case 3: val = pos+y; break;
			}
			myOutput<<val[0]<<" "<<val[1]<<" "<<val[2]<<" "<<endl;
			j++;
		}
	}
	/**
	 */
	static ofstream myOutput;
	/**
	 */
public:
	/**
	 */
	static const int* getCase(const unsigned char& index) {
		return m_segmentcase[index];
	}
	/**
	 */
private:
	/**
	 */
	static int m_segmentcase[16][4];
};

template <class TVolumetricSpace>
ofstream MarchingQuads<TVolumetricSpace>::myOutput;

template <class TVolumetricSpace>
int MarchingQuads<TVolumetricSpace>::m_segmentcase[16][4]={
/*00*/{-1,-1,-1,-1},
/*01*/{ W, N,-1,-1},
/*02*/{ N, E,-1,-1},
/*03*/{ W, E,-1,-1},
/*04*/{ S, W,-1,-1},
/*05*/{ S, N,-1,-1},
/*06*/{ N, E, S, W},
/*07*/{ S, E,-1,-1},
/*08*/{ E, S,-1,-1},
/*09*/{ N, W, E, S},
/*10*/{ N, S,-1,-1},
/*11*/{ W, S,-1,-1},
/*12*/{ E, W,-1,-1},
/*13*/{ E, N,-1,-1},
/*14*/{ N, W,-1,-1},
/*15*/{-1,-1,-1,-1}
	};

CGL_END

#endif