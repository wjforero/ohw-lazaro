/**
 * $Revision$
 * $Date$
 */

#ifndef HEEDGE_H
#define HEEDGE_H

#include "base.h"

CGL_BEGIN

/** Modela un edge de la estructura de datos Halfedge
 */
template <class THalfedge> 
class HalfedgeEdge {
public:
  // Tipos de datos
  /**
   */
  typedef typename THalfedge::TVertex TVertex;
  /**
   */
  typedef typename THalfedge::TEdge TEdge;
  /** Tipo de dato del punto
   */
  typedef typename THalfedge::TSpace::TAffinePoint TPoint;
public:
  /** Constructor por defecto
   */
  HalfedgeEdge() {
    m_vertex = 0;
  }
  /** Construye un edge con el vertice especificado
   * @param point el punto a almacenar
   */
  HalfedgeEdge(TVertex& vertex) {
    m_vertex = &vertex;
  }
  /** Destructor
   */
  ~HalfedgeEdge() {
  }
public:
  // Operadores
public:
  // Metodos de acceso
  /** Modifica el vertice almacenado
   */
  void setVertex(TVertex& vertex) {
    m_vertex = &vertex;
  }
  /** retorna el vertice almacenado como una referencia constante
   */
  TVertex& getVertex() const {
    return *m_vertex;
  }
  /** retorna el vertice almacenado
   */
  TVertex& getVertex() {
    return *m_vertex;
  }
  /** Modifica el edge opuesto
   */
  void setOpposite(TEdge& opposite) {
    m_opposite = &opposite;
  }
  /** retorna el edge opuesto como una referencia constante
   */
  TEdge& getOpposite() const {
    return *m_opposite;
  }
  /** retorna el edge opuesto
   */
  TEdge& getOpposite() {
    return *m_opposite;
  }
  /**
   */
  void setNext(TEdge& next) {
    m_next = &next;
  }
  /**
   */
  TEdge& getNext() {
    return *m_next;
  }
  /**
   */
  TEdge& getNext() const {
    return *m_next;
  }
private:
  // Atributos
  /** Vertice origen
   */
  TVertex* m_vertex;
  /** Edge opuesto
   */
  TEdge* m_opposite;
  /** Siguiente edge 
   */
  TEdge* m_next;

};

template <class THalfedge> 
ostream& operator<<(ostream& os, HalfedgeEdge<THalfedge>& edge){    
  os<<" v "<<edge.getVertex();
  os<<" v "<<edge.getOpposite().getVertex();
  return os;
}

CGL_END

#endif