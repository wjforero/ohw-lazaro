/**
 * $Revision$
 * $Date$
 */

#ifndef GRAPH_H
#define GRAPH_H

#include "base.h"
#include "graphnode.h"
#include "graphedge.h"
#include "visitor.h"

ST_BEGIN

/** Clase Graph. Representa un Grafo no dirigido.
 * TGraphNode es la clase que representa los nodos del grafo
 * TGraphEdge es la clase que representa los arcos del grafo
 * TGraphData es la clase que representa la información 
 *  almacenada en los nodos.
 */

template <template <class G> class TGraphNode, 
          template <class G> class TGraphEdge,
          class TGraphData >
class Graph {
public:
  // Tipos de datos
  /**
   */
  typedef Graph<TGraphNode,TGraphEdge,TGraphData> TSelf;
  /** tipo de nodo
   */
  typedef TGraphNode<TSelf> TNode;
  /** tipo de edge
   */
  typedef TGraphEdge<TSelf> TEdge;
  /** tipo dee dato almacenado en los nodos
   */
  typedef TGraphData TData;
  /** tipo de visitante para los recorridos
   */
  typedef PrePostVisitor<TNode> TPrePostVisitor;
public:
  // Constructores
  /** Constructor por omisión. Construye un grafo vacio
   */
  Graph() {
  };
  /** Destructor
   */
  ~Graph() {
  }
public:
  // Operadores
  /** compara dos grafos
   */
  bool operator == (const Graph& g) {
    return false;
  }
public:
  // Metodos de acceso
  /** retorna un nuevo nodo que pertenece al grafo
   */
  TNode& newNode() {
    TNode& node = *new TNode(getSizeOfNodes());
    m_nodes.push_back(&node);
    m_edges.push_back(new list<TEdge*>);
    return node;
  }
  /** retorna el i-esimo nodo
   * @param i idedntificador del nodo buscado
   */
  TNode& getNode(const unsigned int& i) const {
    return *m_nodes[i];
  }
  /** retorna el número de nodos del grafo
   */
  unsigned int getSizeOfNodes() const {
    return m_nodes.size();
  }
  /** crea un nuevo arco con el nodo o como origen y el nodo d como destino
   * @param o indentificador del origen del arco
   * @param d indentificador del destino del arco
   * @return devuelve el arco creado
   */
  virtual TEdge& addEdge(const unsigned int& o, const unsigned int& d) {
    return addEdge(getNode(o),getNode(d));
  }
  /** crea un nuevo arco con el nodo o como origen y el nodo d como destino
   * @param o origen del arco
   * @param d destino del arco
   * @return devuelve el arco creado
   */
  virtual TEdge& addEdge(TNode& o, TNode& d) {
    TEdge& edge = *new TEdge(o,d);
    list<TEdge*>& edgeso = *m_edges[o.getId()]; edgeso.push_back(&edge);
    list<TEdge*>& edgesd = *m_edges[d.getId()]; edgesd.push_back(new TEdge(d,o));
    return edge;
  }
  /** deevuelve una lista con los arcos incidentes al nodo node
   * @param node identificador del nodo incidente de los arcos
   * @return la lista con los arcos del nodo
   */
  list<TEdge*>& getEdges(const unsigned int& node) const {
    return *m_edges[node];
  }
  /** deevuelve una lista con los arcos incidentes al nodo node
   * @param node nodo incidente de los arcos
   * @return la lista con los arcos del nodo
   */
  list<TEdge*>& getEdges(const TNode& node) const {
    return *m_edges[node.getId()];
  }
  /** retorna el edge que va de o hacia d
   * @param o origen del arco
   * @param d destino del arco
   * @return el arco buscado
   */
  TEdge& getEdge(const TNode& o, const TNode& d) const {
    list<TEdge*>& edges = getEdges(o);
    TEdge& edge = *(*edges.begin());
    for(list<TEdge*>::iterator j = edges.begin(); j!= edges.end(); j++) {
      if( (*j)->mate(o).getId() == d.getId() ) {
        edge = *(*j);
      }
    }
    return edge;
  }
  /** retorna el edge que va de o hacia d
   * @param o identificador del origen del arco
   * @param d identificador del destino del arco
   * @return el arco buscado
   */
  TEdge& getEdge(const unsigned int& o, const unsigned int& d) const {
    list<TEdge*>& edges = getEdges(getNode(o));
    TEdge& edge = *(*edges.begin());
    for(list<TEdge*>::iterator j = edges.begin(); j!= edges.end(); j++) {
      if( (*j)->mate(o).getId() == d ) {
        edge = *(*j);
      }
    }
    return edge;
  }
  /** retorna el numero de arcos que salen del i-esimo nodo
   * @param i identificador del nodo origen
   * return el tamaño de la lista de arcos
   */
  unsigned int getSizeOfEdges(const unsigned int& i) const{
    return getEdges(i).size();
  }
  /** indica si existe o no el arco que va de o hacia d
   * @param o origen dedl arco
   * @param d destino del arco
   * @return verdadero si existe el arco, ,falso en caso contrario
   */
  bool isEdge(TNode& o, TNode& d) const {
    list<TEdge*>& edges = *m_edges[o.getId()]; 
    for(list<TEdge*>::iterator j = edges.begin(); j!= edges.end(); j++) {
      if( (*j)->mate(o).getId() == d.getId() ) {
        return true;
      }
    }
    return false;
  }
  /** indica si existe o no el arco que va de o hacia d
   * @param o identificador del origen dedl arco
   * @param d identificador del destino del arco
   * @return verdadero si existe el arco, ,falso en caso contrario
   */
  bool isEdge(unsigned int& o, unsigned int& d) const {
    return isEdge( getNode(o), getNode(d) );
  }
public:
  // Recorrido de un grafo
  /** DephtFirstTraversal
   */
  void depthFirstTraversal(TNode& start, TPrePostVisitor& visitor) {
#if OHWDEBUG
    cout<<"depth first traversal"<<endl;
#endif
    vector<bool> visited( getSizeOfNodes() );
    for(unsigned int i=0; i<getSizeOfNodes(); i++)
      visited [i] = false;
    depthFirstTraversal(start, visitor, visited);
    for(unsigned int i=0; i<getSizeOfNodes(); i++) {
      /*if (visited [i] == false)
        cout<<"no se visito el nodo "<<i<<endl;*/
    }
  }
  /** DephtFirstTraversal
   */
  void depthFirstTraversal(const unsigned int& start, TPrePostVisitor& visitor) {
    depthFirstTraversal(getNode(start),visitor);
  }
protected:
  /** DepthFirstTraversal
   */
  void depthFirstTraversal(TNode& node, TPrePostVisitor& visitor, vector<bool>& visited) {
    TNode& pre = visitor.preVisit(node);
    visited [node.getId()] = true;
    list<TEdge*>& edges = getEdges(node);
    for(list<TEdge*>::iterator p=edges.begin(); p!=edges.end(); p++){
      TEdge& edge = *(*p);
      TNode& to = edge.mate(node);
      if( !visited[to.getId()] )
        depthFirstTraversal(to,visitor,visited);
    }
    visitor.postVisit(pre);
  }
public:
  // Algoritmos
  /** Algoritmo de Prim para construir MST.
   * Construye un arbol de minima expansión para el grafo g
   * usando como nodo inicial el nodo s.
   */
  static Graph& primsAlgorithm(const Graph& graph, const unsigned int& source) {

    const unsigned int n = graph.getSizeOfNodes();
    
    Graph& result = *new Graph();
    for( unsigned int v=0; v<n; v++ )
      result.newNode( ).setData( graph.getNode(v).getData() );

    bool* component = new bool[n]; // Vertices in setX
    float* distance = new float[graph.getSizeOfNodes()];
    unsigned int* neighbor = new unsigned int[graph.getSizeOfNodes()]; // Nearest neighbor in setX
    unsigned int w;
    for( w=0; w<n; w++ ) {
      component[w] = false;
      if( graph.isEdge( graph.getNode(source), graph.getNode(w) ) )
        distance[w] = graph.getEdge(graph.getNode(source),graph.getNode(w)).getWeight();
      else
        distance[w] = 1000;
      neighbor[w] = source;
    }
    component[source] = true; // source alone is in the setX.
    for (unsigned int i=1; i<n; i++) {
      unsigned int v; //Add one vertex v to X on each pass.
      //cout<<"se inserta el nodo "<<i<<endl;
      
      list<TEdge*>& edges = graph.getEdges(i);

      float min = 1000;
      
      for( w=0; w<n; w++) {
        if( !component[w] ) {
          if( distance[w] < min ) {
            v = w;
            min = distance[w];
          }
        }
      }
      //cout<<"el menor fue "<<v<<endl;

      if ( min < 1000 ) {
        component[v] = true;
        result.addEdge(result.getNode(v), result.getNode(neighbor[v]) );
        for( w=0; w<n; w++ ) if (!component[w])

          if( graph.isEdge( graph.getNode(v), graph.getNode(w) ) &&
              graph.getEdge(graph.getNode(v),graph.getNode(w)).getWeight() < distance[w]) {
            distance[w] = graph.getEdge(graph.getNode(v),graph.getNode(w)).getWeight();
            neighbor[w] = v; 
          }
      }
      else 
        break; // finished a component in disconnected graph
    }

    return result;

  }
private:
  // Atributos
  /** Conjunto de nodos
   */
  vector< TNode* > m_nodes;
  /** Conjunto de arcos
   */
  vector< list< TEdge* >* > m_edges;

};

ST_END

#endif //GRAPH_H