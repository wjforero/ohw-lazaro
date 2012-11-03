/**
 * $Revision$
 * $Date$
 */

#ifndef HEHALFEDGE_H
#define HEHALFEDGE_H

#include "base.h"
#include "hevertex.h"
#include "heedge.h"
#include "heface.h"

CGL_BEGIN

/** Modela una estructura de datos Halfedge
 */
template <template <class HE> class THEVertex, 
          template <class HE> class THEEdge,
          template <class HE> class THEFace,
          class TAffineSpace >
class Halfedge {
public:
  // Tipos de datos
  /**
   */
  typedef Halfedge<THEVertex,THEEdge,THEFace,TAffineSpace> TSelf;
  /** Vertice del halfedge
   */
  typedef typename THEVertex<TSelf> TVertex;
  /** Edge del halfedge
   */
  typedef typename THEEdge<TSelf> TEdge;
  /** Face del halfedge
   */
  typedef typename THEFace<TSelf> TFace;
  /** Tipo de espacio afín
   */
  typedef typename TAffineSpace TSpace;
  /**
   */
  typedef typename TAffineSpace::TAffinePoint TPoint;
public:
  /** Constructor por defecto
   */
  Halfedge() {
  }
  /** Destructor
   */
  ~Halfedge() {
    // Se eliminan las caras
    for(unsigned int i=0; i<m_faces.size(); i++) {
      delete m_faces[i];
    } m_faces.clear();
    // Se eliminan los edges
    for(unsigned int i=0; i<m_edges.size(); i++) {
      delete m_edges[i];
    } m_edges.clear();
    // Se eliminan los vertices
    for(unsigned int i=0; i<m_vertices.size(); i++) {
      delete m_vertices[i];
    } m_vertices.clear();
  }
public:
  // Operadores
public:
  // Metodos de acceso
  /** crea un nuevo vertice y lo agrega a la estructura de datos
   */
  TVertex& addVertex(TPoint& point) {
    TVertex& newVertex = *new TVertex(point);
    m_vertices.push_back( &newVertex );
    return newVertex;
  }
  /** retorna el i-esimo vertice
   */
  TVertex& getVertex(unsigned int& i) {
    return *m_vertices[i];
  }
  /** retorna el i-esimo vertice como una referencia constante
   */
  TVertex& getVertex(unsigned int& i) const {
    return *m_vertices[i];
  }
  /** retorna el número de vertices de la estructura de datos
   */
  unsigned int getNumberOfVerttices() {
    return m_vertices.size();
  }
  /**
   */
  TEdge& makeEdge(unsigned int& p, unsigned int& q) {
    TEdge& edge1 = *new TEdge( getVertex(p) );
    TEdge& edge2 = *new TEdge( getVertex(q) );
    edge1.setOpposite(edge2); edge2.setOpposite(edge1); 
    m_edges.push_back(&edge1);
    return edge1;
  }
  /** crea una nueva cara y la agrega a la estructura de datos
   */
  TFace& addFace(unsigned int p, unsigned int q, unsigned int r) {
    TFace& newFace = *new TFace();
    TEdge& edge1 = makeEdge(p,q);
    TEdge& edge2 = makeEdge(q,r);
    TEdge& edge3 = makeEdge(r,p);
    edge1.setNext( edge2 ); 
    edge2.setNext( edge3 ); 
    edge3.setNext( edge1 ); 
    newFace.setEdge( edge1 );
    m_faces.push_back( &newFace );
    return newFace;
  }
  /** retorna el i-esimo face
   */
  TFace& getFace(unsigned int& i) {
    return *m_faces[i];
  }
  /** retorna el i-esimo face como una referencia constante
   */
  TFace& getFace(unsigned int& i) const {
    return *m_faces[i];
  }
  /** retorna el número de faces de la estructura de datos
   */
  unsigned int getNumberOfFaces() {
    return m_faces.size();
  }
private:
  // Atributos
  /** vertices del halfedge
   */
  vector<TVertex*> m_vertices;
  /** edges del halfedge
   */
  vector<TEdge*> m_edges;
  /** faces del halfedge
   */
  vector<TFace*> m_faces;

};

CGL_END

#endif//HEHALFEDGE_H