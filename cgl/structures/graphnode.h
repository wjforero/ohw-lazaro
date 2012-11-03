/**
 * $Revision$
 * $Date$
 */

#ifndef GRAPHNODE_H
#define GRAPHNODE_H

CGL_BEGIN

/** Representa un nodo de un grafo
 */
template <class TGraph>
class GraphNode {
public:
  // Tipos de datos
  /**
   */
  typedef GraphNode<TGraph> TSelf;
  /**
   */
  typedef typename TGraph::TEdge TEdge;
  /**
   */
  typedef typename TGraph::TData TData;
public:
  // Constructores
  /**
   */
  GraphNode() {
    m_id = -1;
  }
  /**
   */
  GraphNode(const unsigned int& id) : m_id(id) {
    //cout<<"m_id: "<<m_id<<endl;
  }
  /**
   */
  GraphNode( const GraphNode& node ) {
    setData(node.getData());
    m_id = node.getId();
  }
  /**
   */
  ~GraphNode() {
  }
public:
  // Metodos de acceso
  /**
   */
  unsigned int getId() const {
     return m_id;
  }
  /**
   */
  void setId(const unsigned int& id) {
    m_id = id;
  }
  /**
   */
  TData getData() const{
    return m_data;
  }
  /**
   */
  void setData(TData data) {
    m_data = data;
  }
private:
  // Atributos
  /**
   */
  unsigned int m_id;
  /**
   */
  TData m_data;
  
};

/** 
 */	
template <class TGraph>
ostream& operator<<(ostream& os, const GraphNode<TGraph>& gn){    
  os<<gn.getId();
  return os;
}

CGL_END

#endif//GRAPHNODE_H