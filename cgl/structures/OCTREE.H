/**
 * $Revision$
 * $Date$
 */

#ifndef OCTREE_H
#define OCTREE_H

#include "base.h"

#include "visitor.h"
using namespace ohw::structures;

#include "cell.h"

CGL_BEGIN

/** Representa un arbol octal. Un arbol octal es un tipo de 
 * estructura de datos basado en la descomposición recursiva 
 * del espacio.
 */
template<class TAffineSpace, class TOctreeData>
class Octree : public Cell<TAffineSpace> {
public:
  /** Etiquetas para los cotantes y vertices de un octree
	 */
	enum OT_OCTANT{LDB,LDF,LUB,LUF,RDB,RDF,RUB,RUF};
	/** Etiquetas para las aristas de un octree
	 */
	enum OT_EDGE{LD,LU,LB,LF,RD,RU,RB,RF,DB,DF,UB,UF};
	/** Etiquetas para las caras de un octree.
	 */
	enum OT_FACE{L,R,D,U,B,F};
public:
  // Tipos de datos
  /** El mismo octree
   */
  typedef Octree<TAffineSpace, TOctreeData> TSelf;
  /** El tipo de dato almacenado por el octree
   */
  typedef TOctreeData TData;
  /** El objeto que corresponde al patron de visitante para los 
   * recorridos del octree
   */
  typedef PrePostVisitor<TSelf*> TPrePostVisitor;
  /** El objeto que corresponde al patron de visitante para los 
   * recorridos del octree
   */
  typedef Visitor<TSelf*> TVisitor;
  /** El espacio afin en el que esta inmerso el octree
   * (por definición debe corresponder con un espacio afin 
   * de dimensión 3)
   */
  typedef typename TAffineSpace TSpace;
public:
  // Constructores
  /** Construye un arbol octal de altura cero, sin padre.
   */
  Octree() {
    m_parent = 0;
    m_height = 0;
    for(unsigned int i=0; i<8; i++) {
      m_childs[i] = 0;
    }
  }
public:
  /** Construye un arbol octal de dimensiones y altura especificos.
   * @param position La posición del arbol en el espacio (vertice LDB)
   * @param size las dimensiones de la celda representada por el octree
   * @param height la altura inicial del octree
   */
  Octree(const TPoint& position, const TVector& size, unsigned int height) {
    m_parent = 0;
    m_height = 0;
    for(unsigned int i=0; i<8; i++) {
      m_childs[i] = 0;
    }
    setPosition(position);
    setSize(size);
    subdivide(height-1);
  }
protected:
  /** Construye un octree cuyo padre y etiqueta son especificados.
   * @param parent el nodo padre del octree. La posición y tamaño del nodo
   *   son determinados a partir dela posición y tamaño del padre, asi como 
   *   de la etiqueta correspondiente.
   * @param code la etiqueta que corresponde al octante que representa
   *   este nodo en el padre
   */
  Octree(Octree& parent, unsigned int& code) {
    m_parent = &parent;
    m_height = 0;
    TPoint &position = *new TPoint(parent.getPosition());
    TVector &size = *new TVector();
    for(unsigned int i=0; i<TSpace::dim; i++) {
      size[i] = parent.getSize()[i]/2;
      if( 0x04>>i & code )
        position[i] = position[i] + size[i];
    }
    setPosition(position);
    setSize(size);
    for(unsigned int i=0; i<8; i++) {
      m_childs[i] = 0;
    }
    delete &position;
    delete &size;
  }
public:
  /** Libera los recursos ocupados por el octree
   */
  ~Octree() {
    for(unsigned int i=0; i<8; i++) {
      if(m_childs[i] != 0)
        delete m_childs[i];
    }
  }
public:
  // Operadores
  /** Compara dos octrees. Dos octrees son iguales si...
   * @param g el octree con el cual se desea comparar
   */
  bool operator == (const Octree& g) {
    return false;
  }
public:
  // Metodos de acceso
  /** retorna el i-esimo hijo del octree
   * @param i indice del hijo referenciado. Debe estar entre 0 y getDegree()
   */
  Octree& getSubtree(const unsigned int& i) const {
    if(m_childs[i]==0)
      cout<<"el hijo es nulo?"<<endl;
    return *m_childs[i];
  }
  /** retorna una referencia al padre del nodo.
   * Si el nodo no tiene padre se genera una excepción. 
   * Se recomienda verificar primero que el nodo tenga padre (getLabel()==-1)
   */
  Octree& getParent() {
    return *m_parent;
  }
  /** especifica cual es el padre del nodo
   * @param parent el padre deel nodo
   */
  void setParent(Octree& parent) {
    m_parent = &parent;
  }
  /** retorna el grado del arbol octal. Por definición 8
   */
  unsigned int getDegree() {
    return 8;
  }
  /** retorna la altura del arbol.
   * Un arbol tiene altura 0 si es una hoja. La altura dee un nodo
   * determina el mayor numero de pasos para llegar a una hoja.
   */
  unsigned int getHeight() {
    return m_height;
  }
  /** modifica la altura del nodo. Inicialmente un nodo tiene altura 0
   * @param height la altura del nodo
   */
  void setHeight(const unsigned int& height) {
    if(m_height > height)
      return;
    m_height = height;
    if(m_parent!=0)
      getParent().setHeight(height+1);
  }
  /** retorna el dato almacenado por el nodo del arbol.
   */
  unsigned int getSizeOfData() {
    return m_data.size();
  }
  /** retorna el dato almacenado por el nodo del arbol.
   */
  TData& getData(unsigned int i) {
    return m_data[i];
  }
  /** 
   */
  void addData(TData& data) {
    m_data.push_back( data );
  }
  /** retorna el label del nodo. El label corresponde al octante 
   * que representa el nodo en su padre. Si el nodo no tiene padre
   * este metodo retorna -1.
   */
  int getLabel() const {
    if(m_parent==0)
      return -1;
    Octree& parent = *m_parent;
    for(unsigned int i=0; i<parent.getDegree(); i++) {
      if( this == &(parent.getSubtree(i)) )
        return i;
    }
  }
  /** retorna verdadero si el nodo no contiene datos, y falso en caso contrario
   */
  bool isEmpty() {
    return (m_data.size() == 0);
  }
  /**
   */
  bool isLeaf() {
    return (m_childs[0] == 0 && m_childs[1] == 0 && m_childs[2] == 0 && m_childs[3] == 0 &&
            m_childs[4] == 0 && m_childs[5] == 0 && m_childs[6] == 0 && m_childs[7] == 0 );
  }
public:
  /** subdivide el nodo en su ocho octantes
   */
  void subdivide() {
    for(unsigned int i=LDB; i<RUF; i++) {
      m_childs[i] = new Octree(*this,i);
      m_childs[i]->setParent(*this);
      setHeight(getHeight()+1);
    }
  }
protected:
  /** subdivide el nodo y sus hijos hasta alcanzar la altura especificada
   * @param height altura del nodo
   */
  void subdivide(unsigned int height) {
    for(unsigned int i=LDB; i<RUF; i++) {
      m_childs[i] = new Octree(*this,i);
      m_childs[i]->setParent(*this);
      setHeight(getHeight()+1);
      if(height>0)
        m_childs[i]->subdivide(height-1);
    }
  }
public:
  /** localiza un dato usando el visitor dado
   * @param data dato a localizar
   * @param locator visitor usado para el recorrido
   */
  void locateData(TData data, TVisitor& locator) {
    locator.setData(this);
    if( isLeaf() )
      return;
    for(unsigned int i=0; i<getDegree(); i++) {
      if(locator.visit(&getSubtree(i))) {
        getSubtree(i).locateData(data, locator);
        break;
      }
    }
  }
  /**
   */
  /*void insertData(TData data, TVisitor& locator) {
    locator.setData(this);
    if( isLeaf() ) {
      if( isEmpty() ) {
        setData(data);
        return;
      } else {
        subdivide();
        insertData(getData(),locator);
        setData(0);
      }
    }
    for(unsigned int i=0; i<getDegree(); i++) {
      if(locator.visit(&getSubtree(i))) {
        getSubtree(i).insertData(data, locator);
        break;
      }
    }
  }
public:
  // Recorrido de un Octree
  /** DephtFirstTraversal
   */
  void depthFirstTraversal(TPrePostVisitor& visitor) {
    if ( visitor.isDone() )
      return;
    if ( !isEmpty() ) {
      Octree& parent = visitor.preVisit( *this );
      for (unsigned int i = 0; i < getDegree (); ++i)
        getSubtree(i).depthFirstTraversal(visitor);
      visitor.postVisit( parent );
    }
  }
  /** breadthFirstTraversal
   */
  void breadthFirstTraversal(TVisitor& visitor) {
    queue<Octree*> &hqueue = *new queue<Octree*>();
    if(!isEmpty())
      hqueue.push(this);
    while(!hqueue.empty() && !visitor.isDone()) {
      Octree& head = *hqueue.front();
      visitor.visit(head);
      for (unsigned int i=0; i<head.getDegree(); i++) {
        Octree& child = head.getSubtree(i);
        if(!child.isEmpty())
          hqueue.push(&child);
      }
    }
    delete &hqueue;
  }
public:
  // Adjacency and neighbors in quadtrees
  /** Retorna el nodo correspondiente al bloque mas pequeño
   * que es adyacente a la cara I del nodo, y cuyo tamaño es 
   * igual al del nodo.
   */
  Octree* eqFaceNeighbor(int i) {
    Octree* q = 0;
    if (m_parent!=0 && m_adj[i][this->getLabel()])
      q = this->getParent().eqFaceNeighbor(i);
    else q = m_parent;
    if(q!=0)
      return q->m_childs[ m_reflect[i][this->getLabel()] ];
    else
      return q;
  }
  /** Retorna el nodo correspondiente al bloque mas pequeño
   * (puede ser gray) que es adyacente a la cara I del nodo, 
   * y cuyo tamaño es mayor o igual al del nodo.
   */
  Octree* gtFaceNeighbor(int i) {
    Octree* q = 0;
    //Se busca un ancestro comun
    if (m_parent!=0 && m_adj[i][this->getLabel()])
      q = this->getParent().gtFaceNeighbor(i);
    else q = m_parent;
    //Se igue el camino reflejado para localizar el vecino
    if(q!=0 && q->isLeaf()==false)
      return q->m_childs[ m_reflect[i][this->getLabel()] ];
    else
      return q;
  }
  /** Retorna el nodo correspondiente al bloque mas pequeño
   * que es adyacente al lado I del nodo, y cuyo tamaño es 
   * igual al del nodo.
   */
  Octree* eqEdgeNeighbor(int i) {
    Octree* q = 0;
    if (m_parent!=0 && m_adj[i+6][this->getLabel()])
      q = this->getParent().eqEdgeNeighbor(i);
    else if(m_parent!=0 && m_commonFace[i][this->getLabel()]!=-1)
      q = this->getParent().eqFaceNeighbor(m_commonFace[i][this->getLabel()]);
    else q = m_parent;
    if(q!=0)
      return q->m_childs[ m_reflect[i+6][this->getLabel()] ];
    else
      return q;
  }
  /** Retorna el nodo correspondiente al bloque mas pequeño
   * (puede ser gray) que es adyacente al lado I del nodo, 
   * y cuyo tamaño es mayor o igual al del nodo.
   */
  Octree* gtEdgeNeighbor(int i) {
    Octree* q = 0;
    //Se busca un ancestro comun
    if (m_parent!=0 && m_adj[i+6][this->getLabel()])
      q = this->getParent().gtEdgeNeighbor(i);
    else if(m_parent!=0 && m_commonFace[i][this->getLabel()]!=-1)
      q = this->getParent().gtFaceNeighbor(m_commonFace[i][this->getLabel()]);
    else q = m_parent;
    //Se igue el camino reflejado para localizar el vecino
    if(q!=0 && q->isLeaf()==false)
      return q->m_childs[ m_reflect[i+6][this->getLabel()] ];
    else
      return q;
  }
  /** Retorna el nodo correspondiente al bloque mas pequeño
   * que es adyacente al vertice I del nodo, y cuyo tamaño es 
   * igual al del nodo.
   */
  Octree* eqVertexNeighbor(int i) {
    Octree* q = 0;
    if (m_parent!=0 && m_adj[i+18][this->getLabel()])
      q = this->getParent().eqVertexNeighbor(i);
    else if(m_parent!=0 && m_commonEdge[i][this->getLabel()]!=-1)
      q = this->getParent().eqEdgeNeighbor(m_commonEdge[i][this->getLabel()]);
    else if(m_parent!=0 && m_commonFace[i+12][this->getLabel()]!=-1)
      q = this->getParent().eqFaceNeighbor(m_commonFace[i+12][this->getLabel()]);
    else q = m_parent;
    if(q!=0)
      return q->m_childs[ m_reflect[i+18][this->getLabel()] ];
    else
      return q;
  }
  /** Retorna el nodo correspondiente al bloque mas pequeño
   * (puede ser gray) que es adyacente al vertice I del nodo, 
   * y cuyo tamaño es mayor o igual al del nodo.
   */
  Octree* gtVertexNeighbor(int i) {
    Octree* q = 0;
    //Se busca un ancestro comun
    if (m_parent==0)
      q = &this->getParent();
    else if(m_adj[i+18][this->getLabel()])
      q = this->getParent().gtVertexNeighbor(i);
    else if(m_commonEdge[i][this->getLabel()]!=-1)
      q = this->getParent().gtEdgeNeighbor(m_commonEdge[i][this->getLabel()]);
    else if(m_commonFace[i+12][this->getLabel()]!=-1)
      q = this->getParent().eqFaceNeighbor(m_commonFace[i+12][this->getLabel()]);
    else q = m_parent;
    //Se sigue el camino reflejado para localizar el vecino
    if(q!=0 && q->isLeaf()==false)
      return q->m_childs[ m_reflect[i+18][this->getLabel()] ];
    else
      return q; 
  }
  /** busca los vecinos que cumplen una propiedad determinada.
   * la propiedad esta implicita el el visitor dado
   * @param finder visitor usado para la logica de la busqueda
   * @param neighbrs arreglo en el que se almacenan los nodos vecinos
   */
  void findNeighbors(TVisitor& finder, vector<Octree*>& neighbors) {

    queue<Octree*> &hqueue = *new queue<Octree*>();
    if(!isEmpty())
      hqueue.push(this);
    while(!hqueue.empty() && !finder.isDone()) {
      Octree& head = *hqueue.front();
      bool out = visitor.visit(head);
      if( !out ) {
        neighbors.push_back(head);
        for (unsigned int i=0; i<6; i++) {
          Octree* face = head.gtFaceNeighbor(i);
          for(unsigned int j=0; j<hqueue.size(); j++) {
            if(hqueue[j]==face)
              out= true;
          }
          if( face != 0 && !out){
            hqueue.push( face );
          }
        }
      }
    }
    delete &hqueue;
  }
private:
  // Atributos
  /** Conjunto de arboles hijo
   */
  Octree* m_childs[8];
  /**
   */
  Octree* m_parent;
  /** Dato almacenado
   */
  vector<TData> m_data;
  /**
   */
  unsigned int m_height;

private: 
  //Tablas de adyacencia
	/**	adj[i][o] es true sii el cuadrante o es adyacente al 
	 * lado o vertice i
	 */
	static bool m_adj[26][8];
	/** reflect[i][o] retorna el valor de sontype de el bloque 
	 * de igual tamaño (no necesariamente un hermano) que 
	 * comparte el lado o vertice i de un bloque que es de 
	 * sontype o.
	 */
	static int m_reflect[26][8];
	/** common_face[i][o] retorna el tipo de cara del bloque 
	 * que contiene a o que es comun al cuadrante o.
	 */
	static int m_commonFace[20][8];
	/** common_edge[i][o] retorna el tipo de lado del bloque 
	 * que contiene a o que es comun al cuadrante o.
	 */
	static int m_commonEdge[8][8];

};

template<class TAffineSpace, class TOctreeData>
ostream& operator<<(ostream& os, Octree<TAffineSpace,TOctreeData>& ot){    
  os<<"ot: "<<ot.getLabel()<<",  height: "<<ot.getHeight();
  return os;
}

///////////////////////////////////////////////////////////////////
//Tablas de adyacencia
///////////////////////////////////////////////////////////////////

/** adj[i][o] es true sii el cuadrante o es adyacente al 
 * lado o vertice i  
 */
template<class TAffineSpace, class TOctreeData>
bool Octree<TAffineSpace,TOctreeData>::m_adj[26][8] = {
/*       |  LDB  |  LDF  |  LUB  |  LUF  |  RDB  |  RDF  |  RUB  |  RUF  | */
/* L  */ { true  , true  , true  , true  , false , false , false , false },
/* R  */ { false , false , false , false , true  , true  , true  , true  },
/* D  */ { true  , true  , false , false , true  , true  , false , false },
/* U  */ { false , false , true  , true  , false , false , true  , true  },
/* B  */ { true  , false , true  , false , true  , false , true  , false },
/* F  */ { false , true  , false , true  , false , true  , false , true  },
/* LD */ { true  , true  , false , false , false , false , false , false }, 
/* LU */ { false , false , true  , true  , false , false , false , false },
/* LB */ { true  , false , true  , false , false , false , false , false },
/* LF */ { false , true  , false , true  , false , false , false , false },
/* RD */ { false , false , false , false , true  , true  , false , false },
/* RU */ { false , false , false , false , false , false , true  , true  },
/* RB */ { false , false , false , false , true  , false , true  , false },
/* RF */ { false , false , false , false , false , true  , false , true  },
/* DB */ { true  , false , false , false , true  , false , false , false },
/* DF */ { false , true  , false , false , false , true  , false , false },
/* UB */ { false , false , true  , false , false , false , true  , false },
/* UF */ { false , false , false , true  , false , false , false , true  },
/* LDB*/ { true  , false , false , false , false , false , false , false },
/* LDF*/ { false , true  , false , false , false , false , false , false },
/* LUB*/ { false , false , true  , false , false , false , false , false },
/* LUF*/ { false , false , false , true  , false , false , false , false },
/* RDB*/ { false , false , false , false , true  , false , false , false },
/* RDF*/ { false , false , false , false , false , true  , false , false },
/* RUB*/ { false , false , false , false , false , false , true  , false },
/* RUF*/ { false , false , false , false , false , false , false , true  }};

/** reflect[i][o] retorna el valor de sontype de el bloque 
 * de igual tamaño (no necesariamente un hermano) que 
 * comparte el lado o vertice i de un bloque que es de 
 * sontype o.
 */
template<class TAffineSpace, class TOctreeData>
int Octree<TAffineSpace, TOctreeData>::m_reflect[26][8] = {
/*       | LDB | LDF | LUB | LUF | RDB | RDF | RUB | RUF | */
/* L  */ { RDB , RDF , RUB , RUF , LDB , LDF , LUB , LUF },
/* R  */ { RDB , RDF , RUB , RUF , LDB , LDF , LUB , LUF },
/* D  */ { LUB , LUF , LDB , LDF , RUB , RUF , RDB , RDF },
/* U  */ { LUB , LUF , LDB , LDF , RUB , RUF , RDB , RDF },
/* B  */ { LDF , LDB , LUF , LUB , RDF , RDB , RUF , RUB },
/* F  */ { LDF , LDB , LUF , LUB , RDF , RDB , RUF , RUB },
/* LD */ { RUB , RUF , RDB , RDF , LUB , LUF , LDB , LDF },
/* LU */ { RUB , RUF , RDB , RDF , LUB , LUF , LDB , LDF },
/* LB */ { RDF , RDB , RUF , RUB , LDF , LDB , LUF , LUB },
/* LF */ { RDF , RDB , RUF , RUB , LDF , LDB , LUF , LUB },
/* RD */ { RUB , RUF , RDB , RDF , LUB , LUF , LDB , LDF },
/* RU */ { RUB , RUF , RDB , RDF , LUB , LUF , LDB , LDF },
/* RB */ { RDF , RDB , RUF , RUB , LDF , LDB , LUF , LUB },
/* RF */ { RDF , RDB , RUF , RUB , LDF , LDB , LUF , LUB },
/* DB */ { LUF , LUB , LDF , LDB , RUF , RUB , RDF , RDB },
/* DF */ { LUF , LUB , LDF , LDB , RUF , RUB , RDF , RDB },
/* UB */ { LUF , LUB , LDF , LDB , RUF , RUB , RDF , RDB },
/* UF */ { LUF , LUB , LDF , LDB , RUF , RUB , RDF , RDB },
/* LDB*/ { RUF , RUB , RDF , RDB , LUF , LUB , LDF , LDB },
/* LDF*/ { RUF , RUB , RDF , RDB , LUF , LUB , LDF , LDB },
/* LUB*/ { RUF , RUB , RDF , RDB , LUF , LUB , LDF , LDB },
/* LUF*/ { RUF , RUB , RDF , RDB , LUF , LUB , LDF , LDB },
/* RDB*/ { RUF , RUB , RDF , RDB , LUF , LUB , LDF , LDB },
/* RDF*/ { RUF , RUB , RDF , RDB , LUF , LUB , LDF , LDB },
/* RUB*/ { RUF , RUB , RDF , RDB , LUF , LUB , LDF , LDB },
/* RUF*/ { RUF , RUB , RDF , RDB , LUF , LUB , LDF , LDB }};

/** common_face[i][o] retorna el tipo de cara del bloque 
 * que contiene a o que es comun al cuadrante o.
 */
template<class TAffineSpace, class TOctreeData>
int Octree<TAffineSpace, TOctreeData>::m_commonFace[20][8] = {
/*       | LDB | LDF | LUB | LUF | RDB | RDF | RUB | RUF | */
/* LD */ {  -1 ,  -1 ,  L  ,  L  ,  D  ,  D  ,  -1 ,  -1 },
/* LU */ {  L  ,  L  ,  -1 ,  -1 ,  -1 ,  -1 ,  U  ,  U  },
/* LB */ {  -1 ,  L  ,  -1 ,  L  ,  B  ,  -1 ,  B  ,  -1 },
/* LF */ {  L  ,  -1 ,  L  ,  -1 ,  -1 ,  F  ,  -1 ,  F  },
/* RD */ {  D  ,  D  ,  -1 ,  -1 ,  -1 ,  -1 ,  R  ,  R  },
/* RU */ {  -1 ,  -1 ,  U  ,  U  ,  R  ,  R  ,  -1 ,  -1 },
/* RB */ {  B  ,  -1 ,  B  ,  -1 ,  -1 ,  R  ,  -1 ,  R  },
/* RF */ {  -1 ,  F  ,  -1 ,  F  ,  R  ,  -1 ,  R  ,  -1 },
/* DB */ {  -1 ,  D  ,  B  ,  -1 ,  -1 ,  D  ,  B  ,  -1 },
/* DF */ {  D  ,  -1 ,  -1 ,  F  ,  D  ,  -1 ,  -1 ,  F  },
/* UB */ {  B  ,  -1 ,  -1 ,  U  ,  B  ,  -1 ,  -1 ,  U  },
/* UF */ {  -1 ,  F  ,  U  ,  -1 ,  -1 ,  F  ,  U  ,  -1 },
/* LDB*/ {  -1 ,  -1 ,  -1 ,  L  ,  -1 ,  D  ,  B  ,  -1 },
/* LDF*/ {  -1 ,  -1 ,  L  ,  -1 ,  D  ,  -1 ,  -1 ,  F  },
/* LUB*/ {  -1 ,  L  ,  -1 ,  -1 ,  B  ,  -1 ,  -1 ,  U  },
/* LUF*/ {  L  ,  -1 ,  -1 ,  -1 ,  -1 ,  F  ,  U  ,  -1 },
/* RDB*/ {  -1 ,  D  ,  B  ,  -1 ,  -1 ,  -1 ,  -1 ,  R  },
/* RDF*/ {  D  ,  -1 ,  -1 ,  F  ,  -1 ,  -1 ,  R  ,  -1 },
/* RUB*/ {  B  ,  -1 ,  -1 ,  U  ,  -1 ,  R  ,  -1 ,  -1 },
/* RUF*/ {  -1 ,  F  ,  U  ,  -1 ,  R  ,  -1 ,  -1 ,  -1 }};

/** common_edge[i][o] retorna el tipo de lado del bloque 
 * que contiene a o que es comun al cuadrante o.
 */
template<class TAffineSpace, class TOctreeData>
int Octree<TAffineSpace, TOctreeData>::m_commonEdge[8][8] = {
/*       | LDB | LDF | LUB | LUF | RDB | RDF | RUB | RUF | */
/* LDB*/ {  -1 ,  LD ,  LB ,  -1 ,  DB ,  -1 ,  -1 ,  -1 },
/* LDF*/ {  LD ,  -1 ,  -1 ,  LF ,  -1 ,  DF ,  -1 ,  -1 },
/* LUB*/ {  LB ,  -1 ,  -1 ,  LU ,  -1 ,  -1 ,  UB ,  -1 },
/* LUF*/ {  -1 ,  LF ,  LU ,  -1 ,  -1 ,  -1 ,  -1 ,  UF },
/* RDB*/ {  DB ,  -1 ,  -1 ,  -1 ,  -1 ,  RD ,  RB ,  -1 },
/* RDF*/ {  -1 ,  DF ,  -1 ,  -1 ,  RD ,  -1 ,  -1 ,  RF },
/* RUB*/ {  -1 ,  -1 ,  UB ,  -1 ,  RB ,  -1 ,  -1 ,  RU },
/* RUF*/ {  -1 ,  -1 ,  -1 ,  UF ,  -1 ,  RF ,  RU ,  -1 }};

CGL_END

#endif //GRAPH_H