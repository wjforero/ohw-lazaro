/**
 * $Revision:$
 * $Date:$
 */

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "base.h"

CGL_BEGIN

template<class TAffineSpace>
class Triangle3d {
public:
  // Tipos de datos
  /**
	 */
	typedef typename TAffineSpace::TAffinePoint TAffinePoint;
  /**
	 */
	typedef typename TAffineSpace::TAffineVector TAffineVector;
public:
  // Constructores
  /**
   */
  Triangle3d() {
    m_vertices[0] = 0;
    m_vertices[1] = 0;
    m_vertices[2] = 0;
  }
  /**
   */
  Triangle3d(const TAffinePoint& p, const TAffinePoint& q, const TAffinePoint& r) {
    m_vertices[0] = new TAffinePoint(p);
    m_vertices[1] = new TAffinePoint(q);
    m_vertices[2] = new TAffinePoint(r);
  }
  /**
   */
  ~Triangle3d() {
    delete m_vertices[0];
    delete m_vertices[1];
    delete m_vertices[2];
  }
private:
  // Atributos
  /**
   */
  TAffinePoint* m_vertices[3];

};

CGL_END

#endif//TRIANGLE_H