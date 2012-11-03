#ifndef SPHERE_H
#define SPHERE_H

// inclusiones especificas
#include "base.h"

CGL_BEGIN

/** Una esfera en un espacio afín
 */
template <class TAffineSpace>
class Sphere{
	public:					
    typedef typename TAffineSpace::TAffinePoint TAffinePoint;
		typedef typename TAffineSpace::TAffinePoint* IterPoint;

  public:
    Sphere(IterPoint* points,const unsigned int& sizepoints){
      initialize(points,sizepoints);      
    }

    IterPoint getCenter(){
      if(m_center==0)
        EXCEPTION("Falla el centro no esta definido");
      return m_center;
    }

    float getCuadRadius(){
      if(m_radius==-1)
        EXCEPTION("Falla el radio no esta definido");
      return m_radius;
    }

    ~Sphere(){
      delete m_center;
    }
  protected:
    void initialize(IterPoint* points,const unsigned int& sizepoints){
      m_center=0;
      m_radius=-1;
      if(points[0]->getDim()==3 && sizepoints==4){
		     setSphere3D4Points(points);
      }else if(points[0]->getDim()==3 && sizepoints==3){
		    setSphere3D3Points(points);
	    }else if(points[0]->getDim()==2 && sizepoints==3){
		    setSphere2D3Points(points);
	    }else if(points[0]->getDim()==2 && sizepoints==2){
		    setSphere2D2Points(points);
	    }
    }

  private:    
    void tricircumcenter3d(double a[3], double b[3], double c[3], double circumcenter[3], double *xi, double *eta);
    void tetcircumcenter(double a[3], double b[3], double c[3],double d[3], double circumcenter[3], double *xi, double *eta,double *zeta);
    void setSphere3D4Points(IterPoint* points);
    void setSphere3D3Points(IterPoint* points);
    void findCircle(double &cx,double &cy,double x1,double y1,double x2,double y2,double x3,double y3);
    void setSphere2D2Points(IterPoint* points);
    void setSphere2D3Points(IterPoint* points);

    IterPoint m_center;	
    float m_radius;


};

template <class TAffineSpace>
void Sphere<TAffineSpace>::setSphere2D3Points(IterPoint* points){
	double cx, cy;
	findCircle(cx,cy,(*points[0])[0],(*points[0])[1],(*points[1])[0],(*points[1])[1],(*points[2])[0],(*points[2])[1]);
	m_center=new TAffinePoint();
	(*m_center)[0] = cx;
	(*m_center)[1] = cy;
	m_radius=(cx-(*points[0])[0])*(cx-(*points[0])[0]) + (cy-(*points[0])[1])*(cy-(*points[0])[1]);
}

template <class TAffineSpace>
void Sphere<TAffineSpace>::setSphere2D2Points(IterPoint* points){
	m_center=new TAffinePoint();
	(*m_center)[0] = ((*points[0])[0] + (*points[1])[0])/2;
	(*m_center)[1] = ((*points[0])[1] + (*points[1])[1])/2;
  m_radius=((*m_center)[0]-(*points[0])[0])*((*m_center)[0]-(*points[0])[0]) + ((*m_center)[1]-(*points[0])[1])*((*m_center)[1]-(*points[0])[1]);
}

template <class TAffineSpace>
void Sphere<TAffineSpace>::findCircle(double &cx,double &cy,double x1,double y1,double x2,double y2,double x3,double y3){
	double a=(y2-y3)*(x2-x1)-(y2-y1)*(x2-x3);
	double a1=(x1+x2)*(x2-x1)+(y2-y1)*(y1+y2);
	double a2=(x2+x3)*(x2-x3)+(y2-y3)*(y2+y3);
	cx=(a1*(y2-y3)-a2*(y2-y1))/a/2;
	cy=(a2*(x2-x1)-a1*(x2-x3))/a/2;
}

template <class TAffineSpace>
void Sphere<TAffineSpace>::setSphere3D3Points(IterPoint* points){
  int ac;
	double a[3]={(*points[0])[0],(*points[0])[1],(*points[0])[2]};
	double b[3]={(*points[1])[0],(*points[1])[1],(*points[1])[2]};
	double c[3]={(*points[2])[0],(*points[2])[1],(*points[2])[2]};
	double circumcenter[3]={0,0,0};

	double xi=1;
	double eta=1;

	tricircumcenter3d(a,b,c,circumcenter,&xi,&eta);

	m_center=new TAffinePoint();	
	(*m_center)[0]=(*points[0])[0] + circumcenter[0];
	(*m_center)[1]=(*points[0])[1] + circumcenter[1];
	(*m_center)[2]=(*points[0])[2] + circumcenter[2];

	m_radius=0;

	for(ac=0;ac<3;ac++){
			m_radius+=((*m_center)[ac]-(*points[0])[ac])*((*m_center)[ac]-(*points[0])[ac]);
	}
}

template <class TAffineSpace>
void Sphere<TAffineSpace>::setSphere3D4Points(IterPoint* points){
  int ac=0;
	double a[3]={(*points[0])[0],(*points[0])[1],(*points[0])[2]};
	double b[3]={(*points[1])[0],(*points[1])[1],(*points[1])[2]};
	double c[3]={(*points[2])[0],(*points[2])[1],(*points[2])[2]};
	double d[3]={(*points[3])[0],(*points[3])[1],(*points[3])[2]};
	double circumcenter[3]={0,0,0};

	double xi=1;
	double eta=1;
	double zeta=1;

	tetcircumcenter(a,b,c,d,circumcenter,&xi,&eta,&zeta);

	m_center=new TAffinePoint();	
	(*m_center)[0]=(*points[0])[0] + circumcenter[0];
	(*m_center)[1]=(*points[0])[1] + circumcenter[1];
	(*m_center)[2]=(*points[0])[2] + circumcenter[2];

	m_radius=0;

	for(ac=0;ac<3;ac++){
			m_radius+=((*m_center)[ac]-(*points[0])[ac])*((*m_center)[ac]-(*points[0])[ac]);
	}
}


/*****************************************************************************/
/*                                                                           */
/*  tetcircumcenter()   Find the circumcenter of a tetrahedron.              */
/*                                                                           */
/*  The result is returned both in terms of xyz coordinates and xi-eta-zeta  */
/*  coordinates, relative to the tetrahedron's point `a' (that is, `a' is    */
/*  the origin of both coordinate systems).  Hence, the xyz coordinates      */
/*  returned are NOT absolute; one must add the coordinates of `a' to        */
/*  find the absolute coordinates of the circumcircle.  However, this means  */
/*  that the result is frequently more accurate than would be possible if    */
/*  absolute coordinates were returned, due to limited floating-point        */
/*  precision.  In general, the circumradius can be computed much more       */
/*  accurately.                                                              */
/*                                                                           */
/*  The xi-eta-zeta coordinate system is defined in terms of the             */
/*  tetrahedron.  Point `a' is the origin of the coordinate system.          */
/*  The edge `ab' extends one unit along the xi axis.  The edge `ac'         */
/*  extends one unit along the eta axis.  The edge `ad' extends one unit     */
/*  along the zeta axis.  These coordinate values are useful for linear      */
/*  interpolation.                                                           */
/*                                                                           */
/*  If `xi' is NULL on input, the xi-eta-zeta coordinates will not be        */
/*  computed.                                                                */
/*                                                                           */
/*****************************************************************************/
template <class TAffineSpace>
void Sphere<TAffineSpace>::tetcircumcenter(double a[3], double b[3], double c[3],double d[3], double circumcenter[3], double *xi, double *eta,double *zeta){
  double xba, yba, zba, xca, yca, zca, xda, yda, zda;
  double balength, calength, dalength;
  double xcrosscd, ycrosscd, zcrosscd;
  double xcrossdb, ycrossdb, zcrossdb;
  double xcrossbc, ycrossbc, zcrossbc;
  double denominator;
  double xcirca, ycirca, zcirca;

  /* Use coordinates relative to point `a' of the tetrahedron. */
  xba = b[0] - a[0];
  yba = b[1] - a[1];
  zba = b[2] - a[2];
  xca = c[0] - a[0];
  yca = c[1] - a[1];
  zca = c[2] - a[2];
  xda = d[0] - a[0];
  yda = d[1] - a[1];
  zda = d[2] - a[2];
  /* Squares of lengths of the edges incident to `a'. */
  balength = xba * xba + yba * yba + zba * zba;
  calength = xca * xca + yca * yca + zca * zca;
  dalength = xda * xda + yda * yda + zda * zda;
  /* Cross products of these edges. */
  xcrosscd = yca * zda - yda * zca;
  ycrosscd = zca * xda - zda * xca;
  zcrosscd = xca * yda - xda * yca;
  xcrossdb = yda * zba - yba * zda;
  ycrossdb = zda * xba - zba * xda;
  zcrossdb = xda * yba - xba * yda;
  xcrossbc = yba * zca - yca * zba;
  ycrossbc = zba * xca - zca * xba;
  zcrossbc = xba * yca - xca * yba;

  /* Calculate the denominator of the formulae. */
#ifdef EXACT
  /* Use orient3d() from http://www.cs.cmu.edu/~quake/robust.html     */
  /*   to ensure a correctly signed (and reasonably accurate) result, */
  /*   avoiding any possibility of division by zero.                  */
  denominator = 0.5 / orient3d(b, c, d, a);
#else
  /* Take your chances with floating-point roundoff. */
  denominator = 0.5 / (xba * xcrosscd + yba * ycrosscd + zba * zcrosscd);
#endif

  /* Calculate offset (from `a') of circumcenter. */
  xcirca = (balength * xcrosscd + calength * xcrossdb + dalength * xcrossbc) *
           denominator;
  ycirca = (balength * ycrosscd + calength * ycrossdb + dalength * ycrossbc) *
           denominator;
  zcirca = (balength * zcrosscd + calength * zcrossdb + dalength * zcrossbc) *
           denominator;
  circumcenter[0] = xcirca;
  circumcenter[1] = ycirca;
  circumcenter[2] = zcirca;

  if (xi != (double *) NULL) {
    /* To interpolate a linear function at the circumcenter, define a    */
    /*   coordinate system with a xi-axis directed from `a' to `b',      */
    /*   an eta-axis directed from `a' to `c', and a zeta-axis directed  */
    /*   from `a' to `d'.  The values for xi, eta, and zeta are computed */
    /*   by Cramer's Rule for solving systems of linear equations.       */
    *xi = (xcirca * xcrosscd + ycirca * ycrosscd + zcirca * zcrosscd) *
          (2.0 * denominator);
    *eta = (xcirca * xcrossdb + ycirca * ycrossdb + zcirca * zcrossdb) *
           (2.0 * denominator);
    *zeta = (xcirca * xcrossbc + ycirca * ycrossbc + zcirca * zcrossbc) *
            (2.0 * denominator);
  }
}


/*****************************************************************************/
/*                                                                           */
/*  tricircumcenter3d()   Find the circumcenter of a triangle in 3D.         */
/*                                                                           */
/*  The result is returned both in terms of xyz coordinates and xi-eta       */
/*  coordinates, relative to the triangle's point `a' (that is, `a' is       */
/*  the origin of both coordinate systems).  Hence, the xyz coordinates      */
/*  returned are NOT absolute; one must add the coordinates of `a' to        */
/*  find the absolute coordinates of the circumcircle.  However, this means  */
/*  that the result is frequently more accurate than would be possible if    */
/*  absolute coordinates were returned, due to limited floating-point        */
/*  precision.  In general, the circumradius can be computed much more       */
/*  accurately.                                                              */
/*                                                                           */
/*  The xi-eta coordinate system is defined in terms of the triangle.        */
/*  Point `a' is the origin of the coordinate system.  The edge `ab' extends */
/*  one unit along the xi axis.  The edge `ac' extends one unit along the    */
/*  eta axis.  These coordinate values are useful for linear interpolation.  */
/*                                                                           */
/*  If `xi' is NULL on input, the xi-eta coordinates will not be computed.   */
/*                                                                           */
/*****************************************************************************/

template <class TAffineSpace>
void Sphere<TAffineSpace>::tricircumcenter3d(double a[3], double b[3], double c[3], double circumcenter[3], double *xi, double *eta){
  double xba, yba, zba, xca, yca, zca;
  double balength, calength;
  double xcrossbc, ycrossbc, zcrossbc;
  double denominator;
  double xcirca, ycirca, zcirca;

  /* Use coordinates relative to point `a' of the triangle. */
  xba = b[0] - a[0];
  yba = b[1] - a[1];
  zba = b[2] - a[2];
  xca = c[0] - a[0];
  yca = c[1] - a[1];
  zca = c[2] - a[2];
  /* Squares of lengths of the edges incident to `a'. */
  balength = xba * xba + yba * yba + zba * zba;
  calength = xca * xca + yca * yca + zca * zca;

  /* Cross product of these edges. */
#ifdef EXACT
  /* Use orient2d() from http://www.cs.cmu.edu/~quake/robust.html     */
  /*   to ensure a correctly signed (and reasonably accurate) result, */
  /*   avoiding any possibility of division by zero.                  */
  xcrossbc = orient2d(b[1], b[2], c[1], c[2], a[1], a[2]);
  ycrossbc = orient2d(b[2], b[0], c[2], c[0], a[2], a[0]);
  zcrossbc = orient2d(b[0], b[1], c[0], c[1], a[0], a[1]);
#else
  /* Take your chances with floating-point roundoff. */
  xcrossbc = yba * zca - yca * zba;
  ycrossbc = zba * xca - zca * xba;
  zcrossbc = xba * yca - xca * yba;
#endif

  /* Calculate the denominator of the formulae. */
  denominator = 0.5 / (xcrossbc * xcrossbc + ycrossbc * ycrossbc +
                       zcrossbc * zcrossbc);

  /* Calculate offset (from `a') of circumcenter. */
  xcirca = ((balength * yca - calength * yba) * zcrossbc -
            (balength * zca - calength * zba) * ycrossbc) * denominator;
  ycirca = ((balength * zca - calength * zba) * xcrossbc -
            (balength * xca - calength * xba) * zcrossbc) * denominator;
  zcirca = ((balength * xca - calength * xba) * ycrossbc -
            (balength * yca - calength * yba) * xcrossbc) * denominator;
  circumcenter[0] = xcirca;
  circumcenter[1] = ycirca;
  circumcenter[2] = zcirca;

  if (xi != (double *) NULL) {
    /* To interpolate a linear function at the circumcenter, define a     */
    /*   coordinate system with a xi-axis directed from `a' to `b' and    */
    /*   an eta-axis directed from `a' to `c'.  The values for xi and eta */
    /*   are computed by Cramer's Rule for solving systems of linear      */
    /*   equations.                                                       */

    /* There are three ways to do this calculation - using xcrossbc, */
    /*   ycrossbc, or zcrossbc.  Choose whichever has the largest    */
    /*   magnitude, to improve stability and avoid division by zero. */
    if (((xcrossbc >= ycrossbc) ^ (-xcrossbc > ycrossbc)) &&
        ((xcrossbc >= zcrossbc) ^ (-xcrossbc > zcrossbc))) {
      *xi = (ycirca * zca - zcirca * yca) / xcrossbc;
      *eta = (zcirca * yba - ycirca * zba) / xcrossbc;
    } else if ((ycrossbc >= zcrossbc) ^ (-ycrossbc > zcrossbc)) {
      *xi = (zcirca * xca - xcirca * zca) / ycrossbc;
      *eta = (xcirca * zba - zcirca * xba) / ycrossbc;
    } else {
      *xi = (xcirca * yca - ycirca * xca) / zcrossbc;
      *eta = (ycirca * xba - xcirca * yba) / zcrossbc;
    }
  }
}




CGL_END

#endif
