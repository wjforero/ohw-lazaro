#ifndef CONVEXHULL_H
#define CONVEXHULL_H

// Inclusiones especificas
#include "base.h"
#include "../structures/simplexopposite.h"
#include "../structures/simplex.h"
#include "../kernel/affinespace.h"
#include "../kernel/primitives.h"
#include "simplexdimjump.h"

#include "utils.h"

#include <list>
#include <stack>
using namespace std;

CGL_BEGIN  

/** Representa un punto que se inserta a la Envolvente Convexa.
*/	
template <class TAffineSpace>
class VertConvexHull : public TAffineSpace::TAffinePoint
{
  public:
    /** Tipo de dato del punto en el espacio Afín
    */
    typedef typename TAffineSpace::TAffinePoint TPoint;
    /** Tipo Iterador del punto en el espacio Afín
    */
    typedef typename TAffineSpace::TAffinePoint* IterPoint;

  public:
    /** Constructor por omisión, por omisión el punto original es NULL y es antiorigen
    */
    VertConvexHull():TPoint(){
      m_point=0;
      m_isAntiorigin=true;
    }

    /** Construye un vertice con el punto p, con el valor de antiorigen en antiorigin
    *   @param p Punto original que se inserta a la triangulación
    *   @param antiorigin Valor para el antiorigen si es true el punto es el antiorigen
    */
    VertConvexHull(IterPoint p,const bool& antiorigin) : TPoint(*p){
      m_point=p;        
      m_isAntiorigin=antiorigin;
    }

    /** Retorna una bandera indicando si el punto es el antiorigen
    *   @return true si es antiorigen, false en caso contrario
    */
    bool getAntiorigin() const{
      return m_isAntiorigin;
    }

    /** Fija el valor de la bandera del antiorigen
    *   @param antiorigin Valor a fijar en la bandera de antiorigen
    */
    void setAntiorigin(const bool& antiorigin){
      m_isAntiorigin=antiorigin;
    }

    /** Retorna el id del punto
    *   @return Id el punto
    */
    long getId() const{
      return m_id;
    }

    /** Fija el id del punto
    *   @param Id del punto
    */
    void setId(const long& id){
      m_id=id;
    }

    /** Retorna el punto original
    *   @return Punto original con el cual se construyo la Envolvente Convexa
    */
    IterPoint getPoint(){        
      return m_point;
    }

  private:
    // Apuntador al punto original
    IterPoint m_point;
    // Bandera de antiorigen
    bool m_isAntiorigin;
    // Id del punto
    long m_id;
};

template <class TAffineSpace>
class ConvexHull
{
	public:
    // ConvexHull
    /** Tipo de la envolvente convexa misma
    */
		typedef ConvexHull<TAffineSpace> TConvexHullSelf;
    /** Tipo del espacio afin
    */
    typedef TAffineSpace TAffineSpace;    
    /** Tipo punto del espacio afin
    */
    typedef typename PointAffine<TAffineSpace> TPoint;
    /** Tipo Iterador del punto del espacio afin
    */
    typedef typename PointAffine<TAffineSpace>* IterPoint;
    /** Tipo simplice del espacio afín
    */
    typedef typename Simplex<TAffineSpace> TSimplex;
    /** Tipo Iterador del simplice del espacio afín
    */
    typedef typename Simplex<TAffineSpace>* IterSimplex;
    /** Tipo Simplex de transición cuando hay un salto de dimensión
    */
    typedef typename SimplexDimJump<TConvexHullSelf> TSimplexDimJump;
    /** Tipo Iterador del simplex de salto de dimensión
    */
    typedef typename SimplexDimJump<TConvexHullSelf>* IterSimplexDimJump;
    /** Tipo Iterador a un vertice del convex hull
    */
    typedef typename VertConvexHull<TAffineSpace>* IterVertexConvexHull;      

  public:
    /** Tipo de busqueda para el grafo 
    */
    enum{STACKONLYVISIBLE, STACKALL,RECURSIVE,ALLSIMPLEX};

	public:
		/** Contruye el convex hull por omisión
		*/
		ConvexHull();

    /** Destructor
    */
    ~ConvexHull();

    /** Inserta el punto en el convex hull
    *   @param p Punto a insertar en la triangulación
    */
    void insertPoint(IterPoint p);

    /** Inserta el vector del vector vect en la triangulación
    *   @param vect Vector de puntos a insertar
    */
    void insertPoints(vector<IterPoint>& vect);

    /** Inserta el vector del vector vect en la triangulación en orden aleatorio
    *   @param vect Vector de puntos a insertar
    */
    void insertPointsRandom(vector<IterPoint>& vect,const unsigned int& seed);
    
    /** Verifica si un simplex es acotado o no
    *   @param s Iterador del simplex a verificar
    *   @return true si el simplex es acotado, false en caso contrario
    */
    bool isBounded(IterSimplex s)const;        
  
    /** Retorna una referencia al baricentro de los puntos que forma el grafo de partida
    *   @return Referencia al baricentro de la envolvente, pero solo para los puntos que
    *           forman el grafo inicial.
    */
    const TPoint& getCenter() const;

    /** Almacena en el vector sc los simplexes resultado
    *   @param sc Vector donde se almacena el resultado
    */
    void getResult(vector<IterSimplex>& sc);

    /** Salva la malla a un m3d
    */
    void getResult(vector<IterPoint>& vect,unsigned int** simplicial,unsigned int& size);

    /** Retorna el vertice insertado originalmente en el convex hull
    */    
    static IterVertexConvexHull getVertexConvexHull(IterPoint ip);

  private:
    /** Crea un simplex y lo inserta al simplicial complex
    *   @return el nuevo simplex creado
    */
    IterSimplex newSimplexHull();

    /** Enlaza dos simplices S1 y S2, el S2 como vertice v1 de S1 y S1 como vertice v2 de S2
    */
    void setNeighbour(IterSimplex S1,const unsigned int& v1, IterSimplex S2, const unsigned int& v2);

    /** Inserta el primer punto para la construcción del convexhull
    *   @param el punto a insertar
    */
    void insertFirstPoint(IterPoint p);

    /** Inserta un punto que salta de dimension
    *   @param p el punto que salta de dimensión
    */
    void insertDimensionJump(IterPoint p);
    
    /** Inserta un punto que no salta de dimensión
    *   @param p el punto que se inserta
    */
    void insertNonDimensionJump(IterPoint p);

    /** Recalcula el centro con el punto p
    *   @param p el nuevo punto para recalcular el baricentro
    */
    void setCentro(IterPoint p);

    /** Retorna true si el punto salta de dimensión, es así si el
    *   punto no se encuentra en la combinación afin de los puntos
    *   insertados anteriormente
    *   @param p el punto a verificar
    */
    bool isDimensionJump(IterPoint p);

    /** Fija el hiperplano soporte del simplice s, para un punto p
    *   situado en su interior.
    *   @param p Punto a insertar
    *   @param s Simplice cuyo hiperplano soporte se calcula
    */
    void setHiperplane(IterPoint p, IterSimplex s);

    /** Realiza la rutina de busqueda con una pila
    */
    void findAndLocateStack(IterSimplex S,IterSimplex* ultVisited,IterPoint p,list<IterSimplex>& simplicialComplex,int& location);

    /** Realiza la rutina de busqueda recursiva
    */
    void findAndLocateRecursive(IterSimplex S,IterSimplex* ultVisited,IterPoint p,list<IterSimplex>& simplicialComplex,int& location);

    /** Limpia las marcas de las busquedas recursivas
    */
    void clearMarks(IterSimplex S,const int& option);

    /** Crea los simplices para los ridges dectectados cuando un punto es visible.
    */
    void createRidgeHorizonSimplex(list<IterSimplex>& simplicialComplex,IterPoint& p,list<IterSimplex>& newSimplices);

    /** Actualiza las relaciones de vecindad para los puntos que no son dimensión jump
    */
    void actNeighbourNonDimJump(list<IterSimplex>& newSimplices,IterPoint z);

    /** Obtiene el complejo simplicial asociado a esta Envolvente Convexa.
    */
    const list<IterSimplex>& getSimplicialSimplexes() const;

    /** Almacena el convex hull en la lista CH
    */
    void getConvexHull(list<IterSimplex>& CH);

    /** Retorna 1 si el punto p esta en el semiespacio derecho del hiperplano
    *   soporte de s, -1 si esta en el izquierdo, 0 si esta sobre el hiperplano.
    */
    int getSideOfBase(IterPoint p, IterSimplex s);

    /** Retorna true si el punto p esta contenido en el hiperplano soporte de s,
    *   false en caso contrario
    */
    bool containedInBase(IterPoint p, IterSimplex s);      

    /** Retorna true si el punto p esta en el envolvente afín, false en caso contrario
    */
    bool isAffineHull(IterPoint p, IterSimplex s);

    /** Retorna fija la bandera del tipo de busqueda
    */
    void setTypeFind(const unsigned int& typeFind);

    /** Imprime valores del complejo simplical a la salida estandar
    */
    void print();    

  private:

    // Bandera para el número de simplices creados en cada iteración
    unsigned int m_newSimplicesCreate;
    // Bandera para la opción para la primitiva determinante o hiperplano soporte
    unsigned char m_optionPrimitive;
    // Contador del número de simplices visitados por el procedimeinto de busqueda
    // de simplices visibles
    unsigned int m_simplexVisit;
    // tipo de primitiva
    unsigned int m_primitiveType;   
    // Marca
		unsigned char m_mark;
    // Punto antiorigen
    VertConvexHull<TAffineSpace> m_antiorigin;
    // Complejo simplicial
    list<IterSimplex> m_simplicialComplex;
    // Baricenter
    TPoint m_center;
    // Simplex origen
    IterSimplex m_simplexOrigin;
    // Indica en que variable va la dimension
    unsigned int m_cdim;
    // Simplex Inicial en la busqueda
    IterSimplex m_simplexIni;
    // Id para los puntos
    long m_id;
    // Tipo de 
    int m_typeFind;
    // Puntos que conforma el convex hull
    vector<VertConvexHull<TAffineSpace>*> m_pointsConvexHull;
};

template <class TAffineSpace>
ConvexHull<TAffineSpace>::ConvexHull()
{
  // Fija el baricentro en vacio    
  TAffineSpace::TData tzero;
  tzero=0;    
  m_center.setData(tzero);    
  m_simplexOrigin=0;    
  m_cdim=0;    
  m_simplexIni=0;      
  m_antiorigin.setAntiorigin(true);    
  m_simplexVisit=1;
  m_optionPrimitive=2;
  m_id=1;
  m_typeFind=RECURSIVE;//STACKONLYVISIBLE, STACKALL,RECURSIVE,ALLSIMPLEX
}

template <class TAffineSpace>
ConvexHull<TAffineSpace>::~ConvexHull()
{            
  for(list<IterSimplex>::iterator i=m_simplicialComplex.begin() ; i!=m_simplicialComplex.end() ; i++){
    delete *i;
  }
  for(vector<VertConvexHull<TAffineSpace>*>::iterator ip=m_pointsConvexHull.begin() ; ip!=m_pointsConvexHull.end() ; ip++)
    delete (*ip);
}

template <class TAffineSpace>
ConvexHull<TAffineSpace>::IterSimplex ConvexHull<TAffineSpace>::newSimplexHull()
{    
  IterSimplex s=new TSimplex();
  m_simplicialComplex.push_back(s);
  return s;
}

template <class TAffineSpace>
void ConvexHull<TAffineSpace>::setNeighbour(IterSimplex S1,const unsigned int& v1, IterSimplex S2, const unsigned int& v2){
	//S2 es opuesto al vertice v1
  if(S1!=0 && S2!=0){      
	  S1->setOpposite(S2,v1,v2);      
	  //S1 es opuesto al vertice v2
	  S2->setOpposite(S1,v2,v1);
  }else{
    EXCEPTION("Falla al enlazar");
  }
}

template <class TAffineSpace>
void ConvexHull<TAffineSpace>::insertPoint(IterPoint pt){    
  VertConvexHull<TAffineSpace>* vp=new VertConvexHull<TAffineSpace>(pt,false);
  vp->setId(m_id++);    
  m_pointsConvexHull.push_back(vp);
  IterPoint p=vp;
  if(m_cdim==0){
		insertFirstPoint(p);
		setCentro(p);            
	}else{// El punto puede ser dimension jump, genere un salto de dimension o no            
		if(isDimensionJump(p)){        
      
			insertDimensionJump(p);
			setCentro(p);        
      m_simplexOrigin->setVert(p,m_cdim-1);
		}
		else{        
			insertNonDimensionJump(p);
		}
	}
}

template <class TAffineSpace>
void ConvexHull<TAffineSpace>::insertPoints(vector<IterPoint>& vect){
   unsigned int i=0;
   for(i=0;i<vect.size();i++){
    insertPoint(vect[i]);
   }
}

template <class TAffineSpace>
void ConvexHull<TAffineSpace>::insertPointsRandom(vector<IterPoint>& vect,const unsigned int& seed){
   unsigned int i=0;
   shuffle(vect,seed);
   insertPoints(vect);
}


template <class TAffineSpace>
void ConvexHull<TAffineSpace>::setCentro(IterPoint p){
  unsigned int aCentro=0;
	//Crea el centro incrementalmente con p
	for(aCentro=0 ; aCentro < p->getDim() ; aCentro++ ){
		if(m_cdim==1)
			m_center[aCentro]=m_center[aCentro]+p->getData(aCentro);
		else
			m_center[aCentro]=((m_cdim-1)*m_center[aCentro]+p->getData(aCentro))/m_cdim;
	}
}

template <class TAffineSpace>
void ConvexHull<TAffineSpace>::insertFirstPoint(IterPoint p)
{
  //inicialmente hay un simplex cuyos puntos son el antiorigen y el punto que se inserta
  IterSimplex simplexAntiOrigin = newSimplexHull();    
  simplexAntiOrigin->setPeak(&m_antiorigin);        

	m_simplexOrigin = newSimplexHull();
	m_simplexOrigin->setPeak(p);
	setNeighbour(m_simplexOrigin,0,simplexAntiOrigin,0);    

	m_cdim++;
}

template <class TAffineSpace>
bool ConvexHull<TAffineSpace>::isDimensionJump(IterPoint p){
	// Retorna true si el punto es Dimension jump
	// En esta condición se verifica que cualquier punto puede expresarse
	// como combinación lineal de los otros puntos      
	if(m_cdim-1 == TAffineSpace::dim){
		return false;
	}
  
	// Crea un vector para posteriormente evaluar si el nuevo punto pertenece
	// al affine hull de los puntos anteriormente insertados	  
	bool isAffine = !isAffineHull(p,m_simplexOrigin);        
	return isAffine;
}

/*  INSERSIÓN DE UN PUNTO DIMENSION JUMP*/  
/**	Con esta rutina se construye un simplice inicial d-dimensional sobre el cual
*		se enlazan las caras de la correspondiente envolvente	
*/
template <class TAffineSpace>
void ConvexHull<TAffineSpace>::insertDimensionJump(IterPoint p){
	// Si el nuevo punto es dimension Jump
	// Para cada nuevo simplex en el conjunto anterior debo insertarle el nuevo punto
	// Para cada Bounded simplex en la triangulación anterior creo un unBounded simplex
	// Actualizo las relaciones de vecindad    
	list<IterSimplexDimJump>* listDimensionJump = new list<IterSimplexDimJump>();
	for(list<IterSimplex>::iterator i=m_simplicialComplex.begin() ; i!=m_simplicialComplex.end();i++){
		// Crea un simplex de transición
		IterSimplexDimJump ele = new TSimplexDimJump(*i,p,m_cdim,&m_antiorigin,this);
		listDimensionJump->push_back(ele);
	}            
  for(list<IterSimplexDimJump>::iterator i1=listDimensionJump->begin();i1!=listDimensionJump->end();i1++){
    if(isBounded((*i1)->getFather())){
			// Si es bounded tiene un hijo unbounded este hijo hay que agregarlo a la lista de simplexes
    	m_simplicialComplex.push_back((*i1)->getSonUnbounded());

		}      
  }   

	for(list<IterSimplexDimJump>::iterator i2=listDimensionJump->begin();i2!=listDimensionJump->end();i2++){
		// Actualiza las relaciones de vecindad del simplex de transición		  
		(*i2)->setNeighbour1();      
	}

  for(list<IterSimplexDimJump>::iterator i3=listDimensionJump->begin();i3!=listDimensionJump->end();i3++){
		// Actualiza las relaciones de vecindad del simplex de transición		
		(*i3)->setNeighbour2();      
	}
	// Después de insertarlo aumenta la dimension en 1	
	m_cdim++;	  

	for(list<IterSimplexDimJump>::iterator i4=listDimensionJump->begin();i4!=listDimensionJump->end();i4++)
  	delete *i4;
	delete listDimensionJump;		
}

/*INSERSIÓN DE UN PUNTO NO DIMENSION JUMP*/
/*
Construye las caras de una envolvente convexa d-dimensional
*/
template <class TAffineSpace>
void ConvexHull<TAffineSpace>::insertNonDimensionJump(IterPoint p){
	// Si no son dimension Jump
	// Localizo x (Dentro de la envolvente, fuera, o sobre algún lado)
	// Encuentro los hull visible facets
	// Encuentro los horizon rigde
	// Creo los nuevos simplices enlazandolos con los horizon rigde	
  // Actualizo las relaciones de vecindad	    
  IterSimplex ultVisited;  
  list<IterSimplex>* visible_simplices = new list<IterSimplex>();
  int location = -1;
  
  // Localización de x en la envolvente y extrae la lista de x-visible
	// -1 esta adentro, 1 esta afuera, 0 esta sobre uno de los lados
  
  if(m_simplexIni==0){
		m_simplexIni=m_simplexOrigin;
	}    
  clearMarks(m_simplexIni,1);    
  m_simplexVisit=0;
  m_newSimplicesCreate=0;
  if((m_typeFind == STACKONLYVISIBLE) || (m_typeFind == STACKALL)){
    findAndLocateStack(m_simplexOrigin, &ultVisited, p, *visible_simplices, location);    
  }else if(m_typeFind==RECURSIVE){
    findAndLocateRecursive(m_simplexOrigin, &ultVisited, p, *visible_simplices, location);    
  }else if(m_typeFind==ALLSIMPLEX){    
    for(list<IterSimplex>::iterator ij=m_simplicialComplex.begin();ij!=m_simplicialComplex.end();ij++){
      IterSimplex T=*ij;     
      if ( !(isBounded(T)) ) {
        int side = getSideOfBase(p,T);
				if ( side >= 0 ) {            
					//Inserta el simplex en la lista de simplex visibles					
          visible_simplices->push_back(T);
					//Marca el simplex como visible
					T->setMarkVisible(true);	
					
	        location = 1;
        }
      }
    }
  }

  switch(location){
  	case -1:// Esta adentro de la envolvente no hace nada
					clearMarks(m_simplexIni,1);
					return;
					break;
  	case 0:// Esta sobre la envolvente no hace nada
					clearMarks(m_simplexIni,1);
					return;
					break;
  	case 1:// Esta fuera de la envolvente
					// Debe crear nuevos simplices que se encuentren
					// Se debe remplazar el peak que hasta ese momento era en antiorigen
					// Por p en los simplices de la lista de m_simplicialComplex
					list<IterSimplex>* newSimplices=new list<IterSimplex>();
					//Encuentra los horizon ridge y crea los simplex asociados            
					createRidgeHorizonSimplex(*visible_simplices,p,*newSimplices);            
					m_simplexVisit=0;
					clearMarks(m_simplexIni,3);            
					actNeighbourNonDimJump(*newSimplices,p);			      

					delete newSimplices;
					break;
	}    
  delete visible_simplices;    
}

template <class TAffineSpace>
void ConvexHull<TAffineSpace>::findAndLocateStack(IterSimplex S,IterSimplex* ultVisited,IterPoint p,list<IterSimplex>& visible_simplices,int& location){
  unsigned int i=0;    
  bool haveNeight=false;
  stack<IterSimplex> vstack;
  vstack.push(S);
  
  bool haveUnBounded=false;
  while(!vstack.empty()){      
    m_simplexVisit++;
    IterSimplex T=vstack.top();
    
    if ( !(isBounded(T)) && !T->getMark()) {
      if(!haveUnBounded && !(isBounded(T))){          
        haveUnBounded=true;
      }

	    int side = getSideOfBase(p,T);
	    if(side==2){
		    EXCEPTION("Falla en la busqueda");
	    }
	    if ( side >= 0 ) {            
		    //Inserta el simplex en la lista de simplex visibles					
        visible_simplices.push_back(T);
		    //Marca el simplex como visible
		    T->setMarkVisible(true);		      
	      location = 1;
      }
    }
    T->setMark(true);
    haveNeight=false;
    for(i = 0; i < m_cdim; i++ ) {        
      if(!T->getOpposite(i)->getMark() && T!=T->getOpposite(i)){          
        if(m_typeFind == STACKONLYVISIBLE){
          if(!haveUnBounded){              
            haveNeight=true;
            vstack.push(T->getOpposite(i));
          }else if(!isBounded(T->getOpposite(i))){              
            haveNeight=true;
            vstack.push(T->getOpposite(i));
          }
        }else if(m_typeFind == STACKALL){
          haveNeight=true;
          vstack.push(T->getOpposite(i));
        }
      }
    }      
    
    if(!haveNeight)
       vstack.pop();      
  }    
}

template <class TAffineSpace>
void ConvexHull<TAffineSpace>::findAndLocateRecursive(IterSimplex S,IterSimplex* ultVisited,IterPoint p,list<IterSimplex>& visible_simplices,int& location){
	//Este procedimiento usa el metodo de all-visibility
  m_simplexVisit++;
	S->setMark(true);
	for( unsigned int i = 0; i < m_cdim; i++ ) {
		// Busca en todos los vecinos uno, que no este marcado y que
		// Este en el semiespacio correcto y además sea unbounded
    IterSimplex T = S->getOpposite(i);
		int side =0;
	  if ( T!=0 && !T->getMark() ) {               
      side = getSideOfBase(p,T);
			if ( !(isBounded(T)) ) {					
				if(side==2){
					EXCEPTION("Falla en la busqueda");
				}				  
        if ( side > 0 ) {        
					//Inserta el simplex en la lista de simplex visibles					
          visible_simplices.push_back(T);
					//Marca el simplex como visible
					T->setMarkVisible(true);	
					
	        location = 1;
        }
				if ( side==0 && location == -1 && containedInBase(p,T) ) {
          location = 0;
          *ultVisited = T;
          return;
        }                
      }			
			if ( side >= 0 || (side == 0 && location == -1) ) {          
      	findAndLocateRecursive(T,ultVisited,p,visible_simplices,location);
      }
    }
    else{
    }
  }
}

/*LIMPIA LAS MARCAS DE VISITA*/
template <class TAffineSpace>
void ConvexHull<TAffineSpace>::clearMarks(IterSimplex S,const int& option){	      
  stack<IterSimplex> vstack;    
  vstack.push(S);

  while(!vstack.empty()){      
		m_simplexVisit++;
    IterSimplex T=vstack.top();
    if(option==1)
	    T->setMark(false);
	  else if(option==2)
	    T->setMarkVisible(false);
    else if(option==3){
      T->setMark(false);        
      T->setMarkVisible(false);
    }

    bool noMark=true;
    for( unsigned int i = 0; i < m_cdim; i++){        
      if((option==1 || option==3) && T->getOpposite(i)->getMark()){
        vstack.push(T->getOpposite(i));
        noMark=false;
      }
      if(option==2 && T->getOpposite(i)->getMarkVisible()){
        vstack.push(T->getOpposite(i));
        noMark=false;
      }
    }

    if(noMark){
      vstack.pop();
    }
  }
}

template <class TAffineSpace>
void ConvexHull<TAffineSpace>::createRidgeHorizonSimplex(list<IterSimplex>& visible_simplices,IterPoint& p,list<IterSimplex>& newSimplices){
	// Un ridge horizon es un face de (d-2) dimensiones
	// Con la propiedad de que exactamente UNO de hull facets inicidentes
	// ve a x
	// Cada horizon forma un nuevo simplex Af conformado por el ridge el punto
	// y el antiorigen
	// Por cada horizon ridge existe un facet G que no es p-visble
	int dcur=m_cdim-1;
	for(list<IterSimplex>::iterator i=visible_simplices.begin();i!=visible_simplices.end();i++){
		//Remplaza el antiorigen por p
		(*i)->setPeak(p);
		for (int k = 1; k <=dcur; k++) {
				//Si alguno de sus vecinos no es visible entonces es un horizon ridge
				if( !(isBounded((*i)->getOpposite(k))) && !(((*i)->getOpposite(k))->getMarkVisible()) ){
					//Crea el nuevo simplex
					IterSimplex newSimplex=newSimplexHull();
          m_newSimplicesCreate++;
					/*****************************/
					// Inserta los puntos de horizon ridge
          for (int j = 1; j <= dcur; j++){
						if ( j != k ){
							// Inserta todos los puntos en ridge excepto el punto
							// cuyo simplex opuesto es el no visible							
							newSimplex->setVert(((*i)->getVert(j)),j);
						}
					}
					// Fija en la posición k el ultimo vertice de i
					if ( k != dcur)
						newSimplex->setVert(((*i)->getVert(dcur)),k);

					newSimplex->setVert(p,dcur);
					newSimplex->setPeak(&m_antiorigin);					

					setNeighbour(newSimplex,dcur,((*i)->getOpposite(k)),(*i)->indexOpposite(k));
					setNeighbour(newSimplex,0,*i,k);
					newSimplices.push_back(newSimplex);
				}
		}
  }	
}


/*Actualiza las relaciones de vecindad en los nuevos simplices creados*/
template <class TAffineSpace>
void ConvexHull<TAffineSpace>::actNeighbourNonDimJump(list<IterSimplex>& newSimplices,IterPoint z){
	//Realiza la actualización de dimension jumps			
	int dcur=m_cdim-1;
	IterSimplex Af;
	for(list<IterSimplex>::iterator i=newSimplices.begin();i!=newSimplices.end();i++){
		Af=(*i);
		for(int k=1;k<dcur;k++){
    	if(Af->getOpposite(k)==0){
      	IterSimplex T=Af->getOpposite(0);
				int y1=1;
				while(T->getVert(y1)!=Af->getVert(k))
					y1++;
				int y2=Af->indexOpposite(0);
				while(T->getVert(0)==z){
					int newy1=0;
					while((T->getOpposite(y1))->getVert(newy1) != T->getVert(y2) ){
						newy1++;
					}
					y2=T->indexOpposite(y1);
					T=T->getOpposite(y1);
					y1=newy1;					
				}
				setNeighbour(Af,k,T,y1);
			}
		}
	}
}

template <class TAffineSpace>
const list<ConvexHull<TAffineSpace>::IterSimplex>& ConvexHull<TAffineSpace>::getSimplicialSimplexes() const{
	return m_listSimplexes;
}

template <class TAffineSpace>
void ConvexHull<TAffineSpace>::getConvexHull(list<IterSimplex>& CH){        
	for(list<IterSimplex>::iterator i=m_listSimplexes->begin();i!=m_listSimplexes->end();i++){
		if(!isBounded((*i))){
			CH.push_back((*i));    	
		}
	}
}

template <class TAffineSpace>
bool ConvexHull<TAffineSpace>::isBounded(IterSimplex s) const
{
  if(s!=0){                  
    return !((VertConvexHull<TAffineSpace>*)s->getVert(0))->getAntiorigin();
  }
  return false;
}

template <class TAffineSpace>
int ConvexHull<TAffineSpace>::getSideOfBase(IterPoint p, IterSimplex s)
{    
  return Primitives<TAffineSpace>::getSideOfBase(&m_center,p,s,m_optionPrimitive);
}

template <class TAffineSpace>
bool ConvexHull<TAffineSpace>::containedInBase(IterPoint p, IterSimplex s)
{
  return false;
}

template <class TAffineSpace>
void ConvexHull<TAffineSpace>::setHiperplane(IterPoint p, IterSimplex s)
{    
  if(m_optionPrimitive==2){      
    Primitives<TAffineSpace>::setHiperplane(p,s);      
  }
}

template <class TAffineSpace>
const ConvexHull<TAffineSpace>::TPoint& ConvexHull<TAffineSpace>::getCenter() const{    
  return m_center;
}

template <class TAffineSpace>
bool ConvexHull<TAffineSpace>::isAffineHull(IterPoint p, IterSimplex s)
{
  return Primitives<TAffineSpace>::isAffineHull(p,s);    
}

template <class TAffineSpace>
void ConvexHull<TAffineSpace>::setTypeFind(const unsigned int& typeFind){    
  m_typeFind=typeFind;
}

template <class TAffineSpace>
void ConvexHull<TAffineSpace>::getResult(vector<IterSimplex>& sc){
  for(list<IterSimplex>::iterator i=m_simplicialComplex.begin() ; i!=m_simplicialComplex.end();i++){		
    if(!isBounded(*i)){
		  sc.push_back(*i);       
    }
	}
}

template <class TAffineSpace>
void ConvexHull<TAffineSpace>::print(){
  cout<<"El simplicial complex total es:"<<endl;
  for(list<IterSimplex>::iterator i=m_simplicialComplex.begin() ; i!=m_simplicialComplex.end();i++){		  
		cout<<**i<<"\n";
	}
}


template <class TAffineSpace>
ConvexHull<TAffineSpace>::IterVertexConvexHull ConvexHull<TAffineSpace>::getVertexConvexHull(IterPoint ip){
  return (IterVertexConvexHull)ip;
}

template <class TAffineSpace>
void ConvexHull<TAffineSpace>::getResult(vector<IterPoint>& vect,unsigned int** simplicial,unsigned int& size){
  int i=0,j=0,k=0;
  vector<ConvexHull<TAffineSpace>::IterSimplex> sc;
  IterPoint p;
  getResult(sc);   
  vect.clear();  

  for(i=0;i<m_pointsConvexHull.size();i++){
    p=new TPoint();
    for(j=0;j<TAffineSpace::dim;j++){
      (*p)[j]=m_pointsConvexHull[i]->getData(j);
    }
    vect.push_back(p);
  }

  if(sc.size()==0){
    return;
  }

  (*simplicial)=new unsigned int[sc.size()*TAffineSpace::dim];

  k=0;  
  for(i=0;i<sc.size();i++){      
    for(j=1;j<sc[i]->getInsertPoints();j++){                              
      (*simplicial)[k++]=((VertConvexHull<TAffineSpace>*)(sc[i]->getVert(j)))->getId();      
    }    
  }
  size=sc.size();
}

/** Imprime los puntos en diferentes formatos en un ostream
*/
template <class TAffineSpace>
ostream& operator<<(ostream& os,ConvexHull<TAffineSpace>& ch){
  int i=0,j=0;  
  vector<ConvexHull<TAffineSpace>::IterSimplex> sc;
  ch.getResult(sc);   
  if(ch.getModeDraw()==ConvexHull<TAffineSpace>::MXD){
    for(i=0;i<sc.size();i++){      
      for(j=0;j<sc[i]->getInsertPoints();j++){
        os<<sc[i]->getVert(j)<<endl;
      }      
    }    
  }else if(ch.getModeDraw()==ConvexHull<TAffineSpace>::OBJ){

  }
  return os;  
}


CGL_END

#endif