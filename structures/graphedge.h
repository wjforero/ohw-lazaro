/**
 * $Revision$
 * $Date$
 */

#ifndef GRAPHEDGE_H
#define GRAPHEDGE_H

ST_BEGIN

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
  /** Construye un arco entre los nodos dados
   * @param n0 cabeza del arco
   * @param n1 cola del arco
   */
  GraphEdge(TNode& n0, TNode& n1) {
    m_nodes[0] = &n0;
    m_nodes[1] = &n1;
  }
  /** Destructor
   */
  virtual ~GraphEdge() {
  }
public:
  // Operadores
public:
  // Metodos de acceso
  /** Retorna el nodo 
   * @param i especifica el nodo buscado. posibles valore: 0 ó 1
   */
  TNode& getNode(const unsigned int& i) const {
    return *m_nodes[i];
  }
  /** retorna el nodo contrario al nodo dado
   * @param node el nodo contrario al buscado
   */
  TNode& mate(const TNode& node) {
    if(node.getId()==getNode(0).getId())
      return *m_nodes[1];
    else
      return *m_nodes[0];
  }
  /** retorna el peso del arco
   */
  float getWeight() const {
    return m_weight;
  }
  /** modifica el peso del arco
   * @param weight nuevo peso para el arco
   */
  void setWeight(const float& weight) {
    m_weight = weight;
  }
public:
  // Otros
  /** invierte los nodos. la cabeza pasa a aser cola y viceversa.
   */
  void swap() {
    TNode* node = m_nodes[0];
    m_nodes[0] = m_nodes[1];
    m_nodes[1] = node;
  }
private:
  // Atributos
  /** nodos del arco
   */
  TNode* m_nodes[2];
  /** peso del arco
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

ST_END

#endif//GRAPHEDGE_H