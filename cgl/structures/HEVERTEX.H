/**
 * $Revision$
 * $Date$
 */

#ifndef HEVERTEX_H
#define HEVERTEX_H

#include "base.h"

CGL_BEGIN

/** Modela un vertice de la estructura de datos Halfedge
 */
template <class THalfedge> 
class HalfedgeVertex {
public:
  // Tipos de datos
  /**
   */
  typedef typename THalfedge::TEdge TEdge;
  /** Tipo de dato del punto
   */
  typedef typename THalfedge::TSpace::TAffinePoint TPoint;
public:
  /** Constructor por defecto
   */
  HalfedgeVertex() {
    m_point = 0;
  }
  /** Construye un verrtice con el punto especificado
   * @param point el punto a almacenar
   */
  HalfedgeVertex(TPoint& point) {
    m_point = &point;
  }
  /** Destructor
   */
  ~HalfedgeVertex() {
  }
public:
  // Operadores
public:
  // Metodos de acceso
  /** Modifica el punto almacenado
   */
  void setPoint(TPoint& point) {
    m_point = &point;
  }
  /** retorna el punto almacenado como una referencia constante
   */
  TPoint& getPoint() const {
    return *m_point;
  }
  /** retorna el punto almacenado
   */
  TPoint& getPoint() {
    return *m_point;
  }
private:
  // Atributos
  /** Punto almacenado
   */
  TPoint* m_point;

};

template <class THalfedge> 
ostream& operator<<(ostream& os, HalfedgeVertex<THalfedge>& vertex){    
  os<<vertex.getPoint();
  return os;
}

CGL_END

#endif