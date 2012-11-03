/**
 * $Revision$
 * $Date$
 */

#ifndef GRAPHNODE_H
#define GRAPHNODE_H

ST_BEGIN

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
  /** Constrtuctor por defecto. Asigna -1 al identificador del nodo
   */
  GraphNode() {
    m_id = -1;
  }
  /** Construye un nodo con el identificador especificado
  * @param id identificador para el nodo
   */
  GraphNode(const unsigned int& id) : m_id(id) {
    //cout<<"m_id: "<<m_id<<endl;
  }
  /** Constrructor de copia
   * @param nodee nodo que se desea copiar
   */
  GraphNode( const GraphNode& node ) {
    setData(node.getData());
    m_id = node.getId();
  }
  /** Destructor
   */
  ~GraphNode() {
  }
public:
  // Metodos de acceso
  /** retorna el identificador del nodo
   */
  unsigned int getId() const {
     return m_id;
  }
  /** modifica el identificador del nodo
   */
  void setId(const unsigned int& id) {
    m_id = id;
  }
  /** retorna el dato almacenado
   */
  TData getData() const{
    return m_data;
  }
  /** modifica el dato almacenado
   */
  void setData(TData data) {
    m_data = data;
  }
private:
  // Atributos
  /** identificador del nodo
   */
  unsigned int m_id;
  /** dato a almacenar
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

ST_END

#endif//GRAPHNODE_H