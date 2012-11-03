/**
 * $Revision: 1.1 $
 * $Date: 2003/03/11 20:55:39 $
 */

#ifndef PREDICATES2D_H
#define PREDICATES2D_H

#include "base.h"
#include "point2d.h"
#include "vector2d.h"
//#include "triangle2d.h"
#include "circle2d.h"

CGL_BEGIN

/** Esta clase encapsula los predicados de la geometria afin en un espacio 
 *  de dos dimensiones.
 */
template<class TAffineSpace>
class Predicates2d {
public:
	/** Define el tipo de dato del espacio vectorial del cual hereda.
	 */
	typedef typename TAffineSpace::TVectorialSpaceSelf TVectorialSpace;
	/** Define el tipo de dato del espacio vectorial es decir de sus vectores.
	 */
	typedef typename TAffineSpace::TVectorialSpaceSelf::TData TData;
	/**
	 */
	typedef typename TAffineSpace::TAffinePoint TAffinePoint;
	/**
	 */
	typedef typename TAffineSpace::TAffineVector TAffineVector;
	
public:
	//Distancias

	/** Calcula la distancia entre dos puntos
	 */
	static TData squaredDistance(const TAffinePoint& p1, const TAffinePoint& p2) {
		TAffineVector dif;
		dif=p1-p2;
		dif=dif*dif;
		return (dif[0]+dif[1]);
	}

public:
	// Operaciones de intersección

public:
	// Predicados y construcciones

	/** Calcula el circumcentro del triángulo formado por p, q y r
	 *  @param p primer vertice del triángulo
	 *  @param q segundo vertice del triángulo
	 *  @param r tercer vertice del triángulo
	 */
	static TAffinePoint circumcenter(const TAffinePoint& p, const TAffinePoint& q, const TAffinePoint& r) {
		cout<<p<<endl;
		cout<<q<<endl;
		cout<<r<<endl;
		TDataType phx = p.gethx();
		TDataType phy = p.gethy();
		TDataType phw = p.gethw();
		TDataType qhx = q.gethx();
		TDataType qhy = q.gethy();
		TDataType qhw = q.gethw();
		TDataType rhx = r.gethx();
		TDataType rhy = r.gethy();
		TDataType rhw = r.gethw();
		
		TDataType qy_py = ( qhy*qhw*phw*phw - phy*phw*qhw*qhw );
		TDataType qx_px = ( qhx*qhw*phw*phw - phx*phw*qhw*qhw );
		TDataType rx_px = ( rhx*rhw*phw*phw - phx*phw*rhw*rhw );
		TDataType ry_py = ( rhy*rhw*phw*phw - phy*phw*rhw*rhw );

		TDataType px2_py2_rx2_ry_2 =
			phx*phx*rhw*rhw + phy*phy*rhw*rhw - rhx*rhx*phw*phw - rhy*rhy*phw*phw ;
		TDataType px2_py2_qx2_qy_2 =
			phx*phx*qhw*qhw + phy*phy*qhw*qhw - qhx*qhx*phw*phw - qhy*qhy*phw*phw ;

		TDataType vvx = qy_py * px2_py2_rx2_ry_2 - ry_py * px2_py2_qx2_qy_2;
		TDataType vvy = rx_px * px2_py2_qx2_qy_2 - qx_px * px2_py2_rx2_ry_2;
		TDataType vvw = TDataType(2) * ( qx_px * ry_py - rx_px * qy_py );

		return TAffinePoint( vvx, vvy, vvw );
	}

};

CGL_END

#endif

