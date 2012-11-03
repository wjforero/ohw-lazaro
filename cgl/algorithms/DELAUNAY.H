#ifndef DELAUNAY_H
#define DELAUNAY_H

// Inclusiones especificas
#include "base.h"
#include "../kernel/affinespace.h"
#include "convexhull.h"
#include "../iodata/formatoutput.h"

#include <list>
using namespace std;

CGL_BEGIN

/** Representa un punto para la triangulación de Delaunay con los mismos datos de dimensión d+1
*/
template <class TAffineSpace>
class PointDelaunay:public TAffineSpace::TAffinePoint{
  public:
    /** Dimensión del espacio de proyección
    */
    enum{
      dim=TAffineSpace::dim
    };
    /** Tipo de dato del espacio de proyección
    */
    typedef typename CGAffineSpace<typename TAffineSpace::TData,dim-1> TAfinSpaceDown;
    /** Tipo de dato del punto en el espacio de proyección
    */
    typedef typename TAfinSpaceDown::TAffinePoint TAffinePointDown;
    /** Tipo Iterador para el punto del espacio de proyección
    */
    typedef TAffinePointDown* IterAffinePointDown;
    /** Tipo de dato del espacio
    */
    typedef typename TAffineSpace::TData TDataType;
  public:
    /** Construye un punto de Delaunay con un punto en el espacio de proyección, proyectandolo a su paraboloide
    *   @param pt punto en el espacio de proyección
    */
    PointDelaunay(IterAffinePointDown pt){
      unsigned int i=0;
      TDataType resul;
      resul=0;
      for(i=0;i<pt->getDim();i++){
        resul+=pt->getData(i)*pt->getData(i);
        (*this)[i]=pt->getData(i);
      }
      (*this)[pt->getDim()]=resul;    
      m_pointInDown=pt;        
    }       

    /** Retorna el punto original
    *   @return Apuntador al punto en el espacio de original el de proyeccíón
    */
    IterAffinePointDown getPointDown(){
      return m_pointInDown;
    }
    
  private:
    // Apuntador al punto en el espacio de proyección.    
    IterAffinePointDown m_pointInDown;      
};
 

/** Representa la triangulación de Deluanay de un conjunto de puntos en un espacio afín de dimensión d
*   la triangulación se construye utilizando el teorema del mapa de levantamiento
*/ 
template <class TAffineSpace>
class Delaunay : public FormatOutput
{    
  public:
    /** Dimensión del espacio de proyección
    */
    enum{
      dim=TAffineSpace::dim
    };
    /** Tipo Espacio proyectado de dimensión d+1, sobre este se construira el paraboloide
    */
    typedef CGAffineSpace<typename TAffineSpace::TData,dim+1> TAfinSpaceUp;
    /** Tipo Iterador para los puntos en el espacio del paraboloide
    */
    typedef TAfinSpaceUp::TAffinePoint* IterPointUp;
    /** Tipo de Envolvente convexa para los puntos en el espacio del paraboloide
    */
    typedef ConvexHull<TAfinSpaceUp> TConvexHull;
    /** Tipo Iterador para los simplices de la envolvente convexa en el espacio del paraboloide
    */
    typedef typename ConvexHull<TAfinSpaceUp>::IterSimplex IterSimplex;
    /** Tipo de dato del espacio afín
    */
    typedef typename TAffineSpace::TData TDataType;
    /** Tipo Iterador del punto en el espacio de proyección
    */
    typedef typename TAffineSpace::TAffinePoint* IterPoint;
    /** Tipo del punto en el espacio de proyección
    */
    typedef typename TAffineSpace::TAffinePoint TPoint;
    /** Tipo de un punto de Delaunay
    */
    typedef typename PointDelaunay<TAfinSpaceUp> TPointDelaunay;
    /** Tipo iterador de un punto de Delaunay
    */
    typedef typename PointDelaunay<TAfinSpaceUp>* IterPointDelaunay;
    /** Tipo iterador para el vector de resultados
    */
    typedef vector<IterSimplex> TVectorResult;
	public:
    /** Constructor por omisión    
    */
    Delaunay();
    /** Destructor
    */
    ~Delaunay();
    /** Inserta el punto pt del espacio de proyección el la triangulación de Delaunay
    *   @param pt Punto insertado en la triangulación
    */
    void insertPoint(IterPoint pt);
    /** Inserta el vector de puntos vect en la trianglacion
    *   @param vect Vector con los puntos a insertar.
    */
    void insertPoints(vector<IterPoint>& vect);
    /** Almacena en el vector sc el resultado de la triangulación de Delaunay
    *   @param sc vector donde se almacenan los resultados
    */
    void getResult(vector<IterSimplex>& sc);
    
    /** Obtiene un resultado y lo almacena en un m3d
    */
    void getResult(vector<IterPoint>& vect,unsigned int** simplicial,unsigned int& size);

    /** Convierte un punto en el simplex resultado al punto originalmente insertado
    *   @param p punto en el simplex resultado
    */
    static IterPoint getOriginalPoint(IterPointUp p);
    /** Retorna el i-esimo punto como el punto originalmente insertado
    *   @param i indice del punto pedido
    *   @return apuntador al punto originalmente insertado
    */
    IterPoint getOriginalPoint(const unsigned int& i);
    /** Retorna el tamaño del conjunto de puntos de la triangualción
    *   @return tamaño de los puntos
    */
    unsigned int getSizePoints()const;      
  private:
    /** Retorna el i-esimo punto de delaunay
    */
    IterPointDelaunay getPoint(const unsigned int& i);
    // Convex hull con el cual se calcula la triangulación
    TConvexHull m_ch;
    // Vector con los puntos proyectados
    vector<IterPointDelaunay> m_pointsUp;
    // Indicador del id de los puntos
    int m_id;
};

template <class TAffineSpace>
Delaunay<TAffineSpace>::Delaunay(){    
  // El id de inicial del los puntos es 1
  m_id=1;    
}

template <class TAffineSpace>
Delaunay<TAffineSpace>::~Delaunay(){
  // Elimina los puntos de proyección creados
  unsigned int i=0;
  for(i=0;i<m_pointsUp.size();i++){
    delete m_pointsUp[i];
  }
}

template <class TAffineSpace>
void Delaunay<TAffineSpace>::insertPoint(IterPoint pt){    
  // Inserta el punto en el convex hull
  int i=0;    
  TDataType resul;
  resul=0;    
  // Construye el punto proyectado
  IterPointDelaunay pd=new TPointDelaunay(pt);    
  m_ch.insertPoint(pd);    
  m_pointsUp.push_back(pd);    
}

template <class TAffineSpace>
void Delaunay<TAffineSpace>::insertPoints(vector<IterPoint>& vect){
  unsigned int i=0;
	shuffle(vect,4);
  for(i=0;i<vect.size();i++){
    insertPoint(vect[i]);
  }
}

template <class TAffineSpace>
Delaunay<TAffineSpace>::IterPoint Delaunay<TAffineSpace>::getOriginalPoint(IterPointUp p){
  // Realiza un cast para volver al punto original
  VertConvexHull<TAffineSpace>* vch=(VertConvexHull<TAffineSpace>*)p;    
  IterPointDelaunay pd=(IterPointDelaunay)vch->getPoint();    
  return pd->getPointDown();    
}

template <class TAffineSpace>
void Delaunay<TAffineSpace>::getResult(vector<IterSimplex>& sc){
  // Obtiene los simplexes resultado
  unsigned int i=0;
  vector<IterSimplex> sct;  
  TConvexHull::TPoint pc(m_ch.getCenter());   
  m_ch.getResult(sct);  
  for(i=0;i<sct.size();i++){
    sct[i]->setHiperplanePoint(&pc);          
  }  
  sc.clear();
  for(i=0;i<sct.size();i++){
    if(sct[i]->getHiperplane().getData(dim)>0){
      sc.push_back(sct[i]);
    }
  }
}

template <class TAffineSpace>
unsigned int Delaunay<TAffineSpace>::getSizePoints()const{
  // retorna el tamaño de los puntos
  return m_pointsUp.size();
}

template <class TAffineSpace>
Delaunay<TAffineSpace>::IterPointDelaunay Delaunay<TAffineSpace>::getPoint(const unsigned int& i){
  // Obtiene el i-esimo punto
  return m_pointsUp[i];
}

template <class TAffineSpace>
Delaunay<TAffineSpace>::IterPoint Delaunay<TAffineSpace>::getOriginalPoint(const unsigned int& i){
  return m_pointsUp[i]->getPointDown();
}

template <class TAffineSpace>
void Delaunay<TAffineSpace>::getResult(vector<IterPoint>& vect,unsigned int** simplicial,unsigned int& size){
  unsigned int i=0,j=0,k=0,l=0;
  vector<Delaunay<TAffineSpace>::IterSimplex> sc;
  IterPoint p=0;
  getResult(sc);    
  
  for(i=0;i<m_pointsUp.size();i++){
      p=new TPoint();
      for(j=0;j<dim;j++){
       (*p)[j]=m_pointsUp[i]->getData(j);
      }
      vect.push_back(p);
  }

  if(sc.size()==0){
    return;
  }

  (*simplicial)=new unsigned int[TAffineSpace::dim*(TAffineSpace::dim+1)*sc.size()];

  l=0;
  for(i=0;i<sc.size();i++){
    for(j=1;j<sc[i]->getInsertPoints();j++){
      for(k=1;k<sc[i]->getInsertPoints();k++){
        if(j!=k){          
          (*simplicial)[l++]=((VertConvexHull<TAffineSpace>*)(sc[i]->getVert(k)))->getId();
        }
      }
    }
  }  
  size=(TAffineSpace::dim+1)*sc.size();
}


/** Imprime los puntos en diferentes formatos en un ostream
*/
template <class TAffineSpace>
ostream& operator<<(ostream& os,Delaunay<TAffineSpace>& d){
  int i=0,j=0;
  vector<Delaunay<TAffineSpace>::IterSimplex> sc;
  d.getResult(sc);  
  if(d.getModeDraw()==Delaunay<TAffineSpace>::MXD){
    for(i=0;i<sc.size();i++){      
      for(j=0;j<sc[i]->getInsertPoints();j++){
        os<<sc[i]->getVert(j)<<endl;
      }      
    }
  }else if(d.getModeDraw()==Delaunay<TAffineSpace>::OBJ){

  }
  return os;  
}


CGL_END

#endif



  