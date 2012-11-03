/**
 * $Revision: 1.2 $
 * $Date: 2003/03/18 12:36:06 $
 */

#ifndef QUADTREE_H
#define QUADTREE_H

#include "base.h"

CGL_BEGIN

template<class TPAffineSpace, class TPData>
class Quadtree {
public:
	/** las caras de un quadtree ( Norte, Este, Sur, Oeste)
	 */
	enum QT_EDGE{N,E,S,W};
	/** los vertices de un quadtree (Noroeste, Noreste, Suroeste, Sureste)
	 */
	enum QT_VERTEX{NW,NE,SW,SE};
public:
	/** Espacio afin al que pertenece el quadtree
	 */
	typedef TPAffineSpace TAffineSpace;
	/** Tipo de datos que almacenara el quadtree
	 */
	typedef TPData TData;
	/** Tipo de vector del espacio afin
	 */
	typedef typename TAffineSpace::TAffineVector TVector;
	/** Tipo de punto del espacio afin
	 */
	typedef typename TAffineSpace::TAffinePoint TPoint;
public: // Constructores
	/** construye un quadtree con la cantidad de niveles pasados como parametro
   * @param levels la cantidad de niveles que tendra el arbol
	 */
	Quadtree(const unsigned int &levels);
	/** construye un quadtree cuyo padre es father y cuyo codigo es code
   * @param father el nodo padre del quadtree
   * @param code el codigo del quadtree
	 */
	Quadtree(Quadtree* father=0, char code=0);
	/** libera los recursos del quadtree
	 */
	~Quadtree();
public: // Operadores
	/** retorna un apuntador al i-esimo hijo del quadtree
   * @param i indice del hijo a retornar
	 */
	Quadtree* operator[](const unsigned int& i);
public:
	//Acceso
	/** retorna un apuntador al padre del quadtree. 0 en caso no tener padre
	 */
	Quadtree* father();
	/** retorna un apuntador al i-esimo hijo del quadtree
   * @param i indice del hijo a retornar
	 */
	Quadtree* son(const unsigned int &i);
	/** retorna el nivel al que pertenece el quadtree dentro del arbol mayor
	 */
	const unsigned int getLevel() const ;
	/** retorna la posición en el espacio del nodo (vertice SW)
	 */
	const TPoint& getPosition() const;
	/** retorna las dimensiones del quadtree como un vector
	 */
	const TVector& getSize() const;
	/** retorna el dato almacenado en el quadtree
	 */
	const TData getData() const{
		return m_data;
	}
	/** modifica el dato almacenado en el quadtree
	 */
	void setData(TData data) {
    //cout<<data<<" ";
		m_data = data;
    //cout<<m_data<<" ";
	}
public:	// Inserción
	/** retorna el quadtree al que pertenece un dato dentro del arbol
   * @param data el dato que se desea insertar
   * @param locate apuntador a la función de localización
	 */
	Quadtree* insertData(TData data, unsigned char (*locate)(TData data, Quadtree* node));
	/** modifica el maximo número de niveles que se puede bajar en el arbol.
   * @param maxLevel maxima cantidad de niveles del arbol
	 */
	void maxLevel(unsigned int maxLevel, bool align=false);
	/** subdivide un quadtree en sus 4 hijos
	 */
	void subdivide();
public:
	/** retorna el tipo de nodo que representa node
	 * (NW,NE,SW, o SE)
	 */
	static int sonType(Quadtree* node);
	/** true si el nodo es una hoja, false en caso contrario
	 */
	bool isLeaf();
public:	// Adjacency and neighbors in quadtrees
	/** Retorna el nodo correspondiente al bloque mas pequeño
	 * que es adyacente al lado I del nodo, y cuyo tamaño es 
	 * igual al del nodo.
	 */
	Quadtree* eqEdgeNeighbor(int i);
	/** Retorna el nodo correspondiente al bloque mas pequeño
	 * (puede ser gray) que es adyacente al lado I del nodo, 
	 * y cuyo tamaño es mayor o igual al del nodo.
	 */
	Quadtree* gtEdgeNeighbor(int i);
	/** Retorna el nodo correspondiente al bloque mas pequeño
	 * que es adyacente al vertice I del nodo, y cuyo tamaño es 
	 * igual al del nodo.
	 */
	Quadtree* eqVertexNeighbor(int i);
	/** Retorna el nodo correspondiente al bloque mas pequeño
	 * (puede ser gray) que es adyacente al vertice I del nodo, 
	 * y cuyo tamaño es mayor o igual al del nodo.
	 */
	Quadtree* gtVertexNeighbor(int i);
public:
  /**
   */
  void getLeafs(vector<Quadtree*>& leafs) {
    if (isLeaf()) {
      leafs.push_back(this);
      return;
    }
    else {
      if(son(NW)!=0) son(NW)->getLeafs(leafs);
      if(son(NE)!=0) son(NE)->getLeafs(leafs);
      if(son(SW)!=0) son(SW)->getLeafs(leafs);
      if(son(SE)!=0) son(SE)->getLeafs(leafs);
    }
  }
	/** encuentra los vecinos a un nodo que cumplen cierta propiedad
   * @param neighbors vector donde se almacenaran los nodos vecinos
   * @param property apuntador a la función que verifica la propiedad deseada
	 */
	void findNeighbors(vector<Quadtree*>& neighbors,bool(*property)(TData d1, TData d2));
protected:
	/** función recursiva para las busquedas
	 */
	static void traverseNeighbors(Quadtree* parent, Quadtree* node, vector<Quadtree*>& neighbors,bool(*property)(TData d1, TData d2));
	/** 
	 */
	static void unmarkNeighbors(Quadtree* node);
private:
	//Atributos
	/** apuntador al padre del quadtree
	 */
	Quadtree* m_father;
	/** arreglo con los hijos del quadtree
	 */
	Quadtree* m_childs[4];
	/** dato almacenado en el quadtree
	 */
	TData m_data;
	/** nivel en el que se encuentra el quadtree en el arbol mayor
	 */
	unsigned int m_level;
	/** dimensiones del quadtree
	 */
	TVector m_size;
	/** posición del quadtree (vertice SW)
	 */
	TPoint m_position;
	/** codigo del quadtree
	 */
	unsigned char m_code;
private:
	/** maximo numero de niveles de un quadtree
	 */
	static unsigned int m_maxLevel;
	/**
	 */
	static bool m_align;
private: //Tablas de adyacencia
	/** adj[i][o] es true sii el cuadrante o es adyacente al 
	 * lado o vertice i  
	 */
	static bool m_adj[8][4];
	/** reflect[i][o] retorna el valor de sontype de el bloque 
	 * de igual tamaño (no necesariamente un hermano) que 
	 * comparte el lado o vertice i de un bloque que es de 
	 * sontype o.
	 */
	static int m_reflect[8][4];
	/** common_edge[i][o] retorna el tipo de lado del bloque 
	 * que contiene a o que es comun al cuadrante o.
	 */
	static int m_commonEdge[4][4];
};

///////////////////////////////////////////////////////////////////
// Constructor / Destructor
///////////////////////////////////////////////////////////////////

/**
 */
template <class TPAffineSpace, class TPData>
Quadtree<TPAffineSpace,TPData>::Quadtree(const unsigned int &levels){
	m_father = 0;	m_data = 0;	m_level = 0; m_code = 0x00;
	m_position = TPoint(0,0);	m_size = TVector(1,1);
	m_childs[0] =  m_childs[1] = m_childs[2] = m_childs[3] = 0;
	maxLevel(levels,true);
	if(m_level==m_maxLevel)
		return;
	subdivide();
}

/**
 */
template <class TPAffineSpace, class TPData>
Quadtree<TPAffineSpace,TPData>::Quadtree(Quadtree<TPAffineSpace,TPData>* father, char code){
	m_father = father;
	m_childs[0] = m_childs[1] = m_childs[2] = m_childs[3] = 0;
	m_data = 0; m_code = 0x00;
	m_level = (father==0 ? 0 : father->getLevel()+1);
	if(father==0)	{
		m_position = TPoint(0,0);
		m_size = TVector(1,1);
	}	else	{
		TVector fsize; fsize = father->getSize();
		TPoint fposition; fposition = father->getPosition();
		m_size = TVector(fsize.gethx()/2,fsize.gethy()/2);
		m_position = TPoint(  ((code&0x01) ? fposition.getx() + m_size.gethx() : fposition.getx()),
												((code&0x02) ? fposition.gety() + m_size.gethy() : fposition.gety()) );
	}
}

/**
 *
 */
template <class TPAffineSpace, class TPData>
Quadtree<TPAffineSpace,TPData>::~Quadtree(){
	delete m_childs[0];
	delete m_childs[1];
	delete m_childs[2];
	delete m_childs[3];
}

///////////////////////////////////////////////////////////////////
// Operadores
///////////////////////////////////////////////////////////////////

/**
 */
template <class TPAffineSpace, class TPData>
Quadtree<TPAffineSpace,TPData>* Quadtree<TPAffineSpace,TPData>::operator [](const unsigned int& i){
	return m_childs[i];
}

///////////////////////////////////////////////////////////////////
// Acceso
///////////////////////////////////////////////////////////////////

/**
 */
template <class TPAffineSpace, class TPData>
Quadtree<TPAffineSpace,TPData>* Quadtree<TPAffineSpace,TPData>::father(){
	return m_father;
}

/**
 */
template <class TPAffineSpace, class TPData>
Quadtree<TPAffineSpace,TPData>* Quadtree<TPAffineSpace,TPData>::son(const unsigned int& i){
	return m_childs[i];
}

/**
 */
template <class TPAffineSpace, class TPData>
const unsigned int Quadtree<TPAffineSpace,TPData>::getLevel() const{
	return m_level;
}

/**
 */
template <class TPAffineSpace, class TPData>
const Quadtree<TPAffineSpace,TPData>::TPoint& Quadtree<TPAffineSpace,TPData>::getPosition() const {
	return m_position;
}

/**
 */
template <class TPAffineSpace, class TPData>
const Quadtree<TPAffineSpace,TPData>::TVector& Quadtree<TPAffineSpace,TPData>::getSize() const {
	return m_size;
}

///////////////////////////////////////////////////////////////////
// Inserción
///////////////////////////////////////////////////////////////////

/**
 */
template <class TPAffineSpace, class TPData>
Quadtree<TPAffineSpace,TPData>* Quadtree<TPAffineSpace,TPData>::insertData(TData data, unsigned char (*locate)(TData data, Quadtree<TPAffineSpace,TPData>* node)){
	if(locate==0)
		return 0;

	char child = 0x00;
	child = (*locate)(data,this);
	if(isLeaf()){
		if(m_data==0) {//es hoja y no tiene datos
			m_data=data;
			return this;
		}else{//es hoja y tiene datos
			if(m_level==m_maxLevel)//se ha llegado al maximo nivel
				return this;
			if(m_childs[child]==0)
				m_childs[child] = new Quadtree<TPAffineSpace,TPData>(this, child);
			return m_childs[child]->insertData(data,locate);
		}
	}else{//no es hoja
		if(m_childs[child]==0)
			m_childs[child] = new Quadtree<TPAffineSpace,TPData>(this, child);
		return m_childs[child]->insertData(data,locate);
	}
}

/**
 */
template <class TPAffineSpace, class TPData>
void Quadtree<TPAffineSpace,TPData>::maxLevel(unsigned int maxLevel, bool align){
	m_maxLevel = maxLevel;
	m_align = align;
}

/**
 */
template <class TPAffineSpace, class TPData>
void Quadtree<TPAffineSpace,TPData>::subdivide(){
	if(m_level==m_maxLevel)
		return;
	m_childs[NW]=new Quadtree<TPAffineSpace,TPData>(this,NW);m_childs[NW]->subdivide();
	m_childs[NE]=new Quadtree<TPAffineSpace,TPData>(this,NE);m_childs[NE]->subdivide();
	m_childs[SW]=new Quadtree<TPAffineSpace,TPData>(this,SW);m_childs[SW]->subdivide();
	m_childs[SE]=new Quadtree<TPAffineSpace,TPData>(this,SE);m_childs[SE]->subdivide();
}

///////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////

/**
 */
template <class TPAffineSpace, class TPData>
int Quadtree<TPAffineSpace,TPData>::sonType(Quadtree<TPAffineSpace,TPData>* node){
	Quadtree<TPAffineSpace,TPData>* father = node->father();
	if(father!=0) {
		if(father->son(NW)==node)
			return NW;
		else if(father->son(NE)==node)
			return NE;
		else if(father->son(SW)==node)
			return SW;
		else if(father->son(SE)==node)
			return SE;
	}
	return 0;
}

/** true si el nodo es una hoja, false en caso contrario
 */
template <class TPAffineSpace, class TPData>
bool Quadtree<TPAffineSpace,TPData>::isLeaf(){
	if(m_childs[0]==0 && m_childs[1]==0 && m_childs[2]==0 && m_childs[3]==0)
		return true;
	else 
		return false;
}

///////////////////////////////////////////////////////////////////
// Adjacency and neighbors in quadtrees
///////////////////////////////////////////////////////////////////

/** Retorna el nodo correspondiente al bloque mas pequeño 
 * que es adyacente al lado I del nodo, y cuyo tamaño es 
 * igual al del nodo.
 */
template <class TPAffineSpace, class TPData>
Quadtree<TPAffineSpace,TPData>* Quadtree<TPAffineSpace,TPData>::eqEdgeNeighbor(int i){
	Quadtree<TPAffineSpace,TPData>* q = 0;
	if (m_adj[i][Quadtree<TPAffineSpace,TPData>::sonType(this)])
		q = this->father()->eqEdgeNeighbor(i);
	else
		q = this->father();
	if(q!=0)
		return q->son(m_reflect[i][Quadtree<TPAffineSpace,TPData>::sonType(this)]);
	else
		return q;
}

/**
	* Retorna el nodo correspondiente al bloque mas pequeño
	* (puede ser gray) que es adyacente al lado I del nodo, 
	* y cuyo tamaño es mayor o igual al del nodo.
	*/
template <class TPAffineSpace, class TPData>
Quadtree<TPAffineSpace,TPData>* Quadtree<TPAffineSpace,TPData>::gtEdgeNeighbor(int i){
	Quadtree<TPAffineSpace,TPData>* q = 0;
	//Se busca un ancestro comun
	if (this->father()!=0 && m_adj[i][Quadtree<TPAffineSpace,TPData>::sonType(this)])
		q = this->father()->gtEdgeNeighbor(i);
	else
		q = this->father();
	//Se igue el camino reflejado para localizar el vecino
	if(q!=0 && q->isLeaf()==false)
		return q->son(m_reflect[i][Quadtree<TPAffineSpace,TPData>::sonType(this)]);
	else
		return q;
}

/**
	* Retorna el nodo correspondiente al bloque mas pequeño
	* que es adyacente al vertice I del nodo, y cuyo tamaño es 
	* igual al del nodo.
	*/
template <class TPAffineSpace, class TPData>
Quadtree<TPAffineSpace,TPData>* Quadtree<TPAffineSpace,TPData>::eqVertexNeighbor(int i){
	Quadtree<TPAffineSpace,TPData>* q = 0;
	if (m_adj[i+4][Quadtree<TPAffineSpace,TPData>::sonType(this)])
		q = this->father()->eqVertexNeighbor(i);
	else if(m_commonEdge[i][Quadtree<TPAffineSpace,TPData>::sonType(this)]!=-1)
		q = this->father()->eqEdgeNeighbor(m_commonEdge[i][Quadtree<TPAffineSpace,TPData>::sonType(this)]);
	else
		q = this->father();
	if(q!=0)
		return q->son(m_reflect[i+4][Quadtree<TPAffineSpace,TPData>::sonType(this)]);
	else
		return q;
}

/**
	* Retorna el nodo correspondiente al bloque mas pequeño
	* (puede ser gray) que es adyacente al vertice I del nodo, 
	* y cuyo tamaño es mayor o igual al del nodo.
	*/
template <class TPAffineSpace, class TPData>
Quadtree<TPAffineSpace,TPData>* Quadtree<TPAffineSpace,TPData>::gtVertexNeighbor(int i){
	Quadtree<TPAffineSpace,TPData>* q = 0;
	//Se busca un ancestro comun
	if (this->father()==0)
		q = this->father();
	else if(m_adj[i+4][Quadtree<TPAffineSpace,TPData>::sonType(this)])
		q = this->father()->gtVertexNeighbor(i);
	else if(m_commonEdge[i][Quadtree<TPAffineSpace,TPData>::sonType(this)]!=-1)
		q = this->father()->gtEdgeNeighbor(m_commonEdge[i][Quadtree<TPAffineSpace,TPData>::sonType(this)]);
	else
		q = this->father();
	//Se igue el camino reflejado para localizar el vecino
	if(q!=0 && q->isLeaf()==false)
		return q->son(m_reflect[i+4][Quadtree<TPAffineSpace,TPData>::sonType(this)]);
	else
		return q;
}

/**
 */
template <class TPAffineSpace, class TPData>
void Quadtree<TPAffineSpace,TPData>::findNeighbors(
			vector< Quadtree<TPAffineSpace,TPData>* >& neighbors, 
			bool (*property)(TData d1, TData d2) ) {

	Quadtree<TPAffineSpace,TPData>* next = 0;
	m_code=0xff;
	next = this->gtEdgeNeighbor(N);//cout<<"visita al norte"<<endl;
	if(next!=0 && (next->m_code&0x80) != 0x80) 
		traverseNeighbors(this,next,neighbors,property);
  next = this->gtEdgeNeighbor(E);//cout<<"visita al este"<<endl;
	if(next!=0 && (next->m_code&0x80) != 0x80) 
		traverseNeighbors(this,next,neighbors,property);
	next = this->gtEdgeNeighbor(S);//cout<<"visita al sur"<<endl;
	if(next!=0 && (next->m_code&0x80) != 0x80) 
		traverseNeighbors(this,next,neighbors,property);
	next = this->gtEdgeNeighbor(W);//cout<<"visita al oeste"<<endl;
	if(next!=0 && (next->m_code&0x80) != 0x80) 
		traverseNeighbors(this,next,neighbors,property);
	unmarkNeighbors(this);
}

/**
 */
template <class TPAffineSpace, class TPData>
void Quadtree<TPAffineSpace,TPData>::traverseNeighbors(
			Quadtree<TPAffineSpace,TPData>* parent,
			Quadtree<TPAffineSpace,TPData>* node,
			vector< Quadtree<TPAffineSpace,TPData>* >& neighbors, 
			bool (*property)(TData d1, TData d2) ) {

	node->m_code=0xff;
  //cout<<node->getData()<<endl;
	if((*property)(parent->getData(),node->getData()))
		neighbors.push_back(node);
	else
		return;

	Quadtree<TPAffineSpace,TPData>* next = 0;
	next = node->gtEdgeNeighbor(N);
	if(next!=0 && (next->m_code&0x80) != 0x80) 
		traverseNeighbors(parent,next,neighbors,property);
  next = node->gtEdgeNeighbor(E);
	if(next!=0 && (next->m_code&0x80) != 0x80) 
		traverseNeighbors(parent,next,neighbors,property);
	next = node->gtEdgeNeighbor(S);
	if(next!=0 && (next->m_code&0x80) != 0x80) 
		traverseNeighbors(parent,next,neighbors,property);
	next = node->gtEdgeNeighbor(W);
	if(next!=0 && (next->m_code&0x80) != 0x80) 
		traverseNeighbors(parent,next,neighbors,property);
}

/**
 */
template <class TPAffineSpace, class TPData>
void Quadtree<TPAffineSpace,TPData>::unmarkNeighbors(Quadtree<TPAffineSpace,TPData>* node) {

	/*if(node->m_code==0x00)
		return;*/
  node->m_code=0x00;

	Quadtree<TPAffineSpace,TPData>* next = 0;
	next = node->gtEdgeNeighbor(N);
	if(next!=0 && next->m_code!= 0x00 ) 
		unmarkNeighbors(next);
  next = node->gtEdgeNeighbor(E);
	if(next!=0 && next->m_code!= 0x00 ) 
		unmarkNeighbors(next);
	next = node->gtEdgeNeighbor(S);
	if(next!=0 && next->m_code!= 0x00 ) 
		unmarkNeighbors(next);
	next = node->gtEdgeNeighbor(W);
	if(next!=0 && next->m_code!= 0x00 ) 
		unmarkNeighbors(next);
}

///////////////////////////////////////////////////////////////////
//Tablas de adyacencia y variables estaticas
///////////////////////////////////////////////////////////////////

/**
	* adj[i][o] es true sii el cuadrante o es adyacente al 
	* lado o vertice i  
	*/
template <class TPAffineSpace, class TPData>
bool Quadtree<TPAffineSpace,TPData>::m_adj[8][4] = {
/*       |  NW  |  NE  |  SW  |  SE  | */
/* N  */ { true , true , false, false},
/* E  */ { false, true , false, true },
/* S  */ { false, false, true , true },
/* W  */ { true , false, true , false},
/* NW */ { true , false, false, false}, 
/* NE */ { false, true , false, false},
/* SW */ { false, false, true , false},
/* SE */ { false, false, false, true }};

/**
	* reflect[i][o] retorna el valor de sontype de el bloque 
	* de igual tamaño (no necesariamente un hermano) que 
	* comparte el lado o vertice i de un bloque que es de 
	* sontype o.
	*/
template <class TPAffineSpace, class TPData>
int Quadtree<TPAffineSpace,TPData>::m_reflect[8][4] = {
/*       | NW | NE | SW | SE | */
/* N  */ { SW , SE , NW , NE },
/* E  */ { NE , NW , SE , SW },
/* S  */ { SW , SE , NW , NE },
/* W  */ { NE , NW , SE , SW },
/* NW */ { SE , SW , NE , NW }, 
/* NE */ { SE , SW , NE , NW }, 
/* SW */ { SE , SW , NE , NW }, 
/* SE */ { SE , SW , NE , NW }};

/**
	* common_edge[i][o] retorna el tipo de lado del bloque 
	* que contiene a o que es comun al cuadrante o.
	*/
template <class TPAffineSpace, class TPData>
int Quadtree<TPAffineSpace,TPData>::m_commonEdge[4][4] = {
/*       | NW | NE | SW | SE | */
/* N  */ { -1 ,  N ,  W , -1 },
/* E  */ {  N , -1 , -1 ,  E },
/* S  */ {  W , -1 , -1 ,  S },
/* W  */ { -1 ,  E ,  S , -1 }};

/**
 */
template <class TPAffineSpace, class TPData>
unsigned int Quadtree<TPAffineSpace,TPData>::m_maxLevel=5;

/**
 */
template <class TPAffineSpace, class TPData>
bool Quadtree<TPAffineSpace,TPData>::m_align=false;


/** Output 
 */	
template <class TPAffineSpace, class TPData>
ostream& operator<<(ostream& os, const Quadtree<TPAffineSpace,TPData>& qt){
	os<<"level: "<<qt.getLevel()<<endl;
	os<<"position: "<<qt.getPosition()<<endl;
	os<<"size: "<<qt.getSize();
	return os;
}

CGL_END
#endif