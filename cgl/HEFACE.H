/**
 * $Revision$
 * $Date$
 */

#ifndef HEFACE_H
#define HEFACE_H

#include "base.h"

CGL_BEGIN

/** Modela un face de la estructura de datos Halfedge
 */
template <class THalfedge> 
class HalfedgeFace {
public:
  // Tipos de datos
  /**
   */
  typedef typename THalfedge::TEdge TEdge;
  /**
   */
  typedef typename THalfedge::TEdge TEdge;
  /** Tipo de dato del punto
   */
  typedef typename THalfedge::TSpace::TAffinePoint TPoint;
public:
  /** Constructor por defecto
   */
  HalfedgeFace() {
    m_edge = 0;
  }
  /** Construye una cara con el edge especificado
   * @param point el punto a almacenar
   */
  HalfedgeFace(TEdge& edge) {
    m_edge = &edge;
  }
  /** Destructor
   */
  ~HalfedgeFace() {
  }
public:
  // Operadores
public:
  // Metodos de acceso
  /** Modifica el edge almacenado
   */
  void setEdge(TEdge& edge) {
    m_edge = &edge;
  }
  /** retorna el edge almacenado como una referencia constante
   */
  TEdge& getEdge() const {
    return *m_edge;
  }
  /** retorna el edge almacenado
   */
  TEdge& getEdge() {
    return *m_edge;
  }
private:
  // Atributos
  /** Edge origen
   */
  TEdge* m_edge;

};

template <class THalfedge> 
ostream& operator<<(ostream& os, HalfedgeFace<THalfedge>& face){    
  os<<"e0 "<<face.getEdge()<<endl;
  os<<"e1 "<<face.getEdge().getNext()<<endl;
  os<<"e2 "<<face.getEdge().getNext().getNext()<<endl;
  return os;
}

CGL_END

#endif//HEFACE_H