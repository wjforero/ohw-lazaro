/**
 * $Revision$
 * $Date$
 */

#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H

CGL_BEGIN

/** Representa un arco de un grafo
 */
template <class TGraph>
class GraphEdge {
public:
  // Tipos de datos
  /**
   */
  typedef GraphEdge<TGraph> TSelf;
  /**
   */
  typedef typename TGraph::TNode TNode;
  /**
   */
  typedef typename TGraph::TData TData;
public:
  // Constructores
  /**
   */
  GraphEdge(TNode& n0, TNode& n1) {
    m_nodes[0] = &n0;
    m_nodes[1] = &n1;
  }
  virtual ~GraphEdge() {
  }
public:
  // Operadores
public:
  // Metodos de acceso
  /** Retorna el nodo 
   */
  TNode& getNode(const unsigned int& i) const {
    return *m_nodes[i];
  }
  /**
   */
  TNode& mate(const TNode& node) {
    if(node.getId()==getNode(0).getId())
      return *m_nodes[1];
    else
      return *m_nodes[0];
  }
  /**
   */
  float getWeight() const {
    return m_weight;
  }
  /**
   */
  void setWeight(const float& weight) {
    m_weight = weight;
  }
public:
  // Otros
  /**
   */
  void swap() {
    TNode* node = m_nodes[0];
    m_nodes[0] = m_nodes[1];
    m_nodes[1] = node;
  }
private:
  // Atributos
  /**
   */
  TNode* m_nodes[2];
  /**
   */
  float m_weight;

};

/** 
 */	
template <class TGraph>
ostream& operator<<(ostream& os, const GraphEdge<TGraph>& ge){    
  os<<ge.getNode(0)<<"->"<<ge.getNode(1);
  return os;
}

CGL_END

#endif//GRAPHEDGE_H