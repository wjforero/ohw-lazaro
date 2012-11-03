/**
 * $Revision: 1.2 $
 * $Date: 2003/03/18 12:36:05 $
 */

#ifndef PREDICATES3D_H
#define PREDICATES3D_H

#include "base.h"

CGL_BEGIN

/** Esta clase encapsula los predicados de la geometria afin en un espacio 
 *  de dos dimensiones.
 */
template<class TAffineSpace>
class Predicates3d {
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
		return (dif[0]+dif[1]+dif[2]);
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
		TData phx = p.gethx();
		TData phy = p.gethy();
		TData phw = p.gethw();
		TData qhx = q.gethx();
		TData qhy = q.gethy();
		TData qhw = q.gethw();
		TData rhx = r.gethx();
		TData rhy = r.gethy();
		TData rhw = r.gethw();
		
		TData qy_py = ( qhy*qhw*phw*phw - phy*phw*qhw*qhw );
		TData qx_px = ( qhx*qhw*phw*phw - phx*phw*qhw*qhw );
		TData rx_px = ( rhx*rhw*phw*phw - phx*phw*rhw*rhw );
		TData ry_py = ( rhy*rhw*phw*phw - phy*phw*rhw*rhw );

		TData px2_py2_rx2_ry_2 =
			phx*phx*rhw*rhw + phy*phy*rhw*rhw - rhx*rhx*phw*phw - rhy*rhy*phw*phw ;
		TData px2_py2_qx2_qy_2 =
			phx*phx*qhw*qhw + phy*phy*qhw*qhw - qhx*qhx*phw*phw - qhy*qhy*phw*phw ;

		TData vvx = qy_py * px2_py2_rx2_ry_2 - ry_py * px2_py2_qx2_qy_2;
		TData vvy = rx_px * px2_py2_qx2_qy_2 - qx_px * px2_py2_rx2_ry_2;
		TData vvw = TData(2) * ( qx_px * ry_py - rx_px * qy_py );

		return TAffinePoint( vvx, vvy, vvw );
	}

};

CGL_END

#endif

