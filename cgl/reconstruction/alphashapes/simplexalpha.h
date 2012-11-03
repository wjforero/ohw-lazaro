#ifndef SIMPLEXALPHA_H
#define SIMPLEXALPHA_H

// Inclusiones especificas
#include "base.h"
#include "../../algorithms/delaunay.h"
#include "../../iodata/formatoutput.h"
#include "../../kernel/sphere.h"

CGL_BEGIN

/** Representa un simplex de Delaunay junto con su esfera circunscrita
*/
template <class TAfinSpace>
class SimplexAlpha : public FormatOutput
{  
  public:
    /** Tipo de dato de la triangulación de Delaunay del espacio afín
    */
    typedef Delaunay<TAfinSpace> TDelaunay;

    /** Tipo Iterador de los simplexes de la triangulación de Delaunay
    */
    typedef typename Delaunay<TAfinSpace>::IterSimplex IterSimplex;
    /** Tipo Iterador para un punto dentro de la triangulación de Delaunay
    */
    typedef Delaunay<TAfinSpace>::IterPointDelaunay IterPointDelaunay;
    /** Tipo Iterador del punto afin del espacio afín
    */
    typedef typename TAfinSpace::TAffinePoint* IterPoint;
    /** Tipo Iterador para un simplex Alpha
    */        
    typedef typename SimplexAlpha<TAfinSpace>* IterSimplexAlpha;
    /** Tipo de una esferea afín
    */
    typedef Sphere<TAfinSpace> TSphere;
    
  public:
    /** Construye un simplex alpha a partir de un simplex en la triangulación y unos indices de definición
    *   @param s Simplex en la triangulación de Delaunay
    *   @param defPoints indice de los puntos de definición en el simplex
    *   @param sizedefpoints tamaño del vector de puntos de definición
    *   @param vop vertice opuesto para encontrar el hiperplano y verificar si el simplex pertenece al Convex Hull
    */
    SimplexAlpha(IterSimplex s,int* defPoints,const unsigned int& sizedefpoints,const unsigned int& vop);    
    /** Destructor
    */
    ~SimplexAlpha();
    /** Retorna el simplex asociado
    *   @return El simplex asociado
    */
    IterSimplex getSimplex();        
    /** Verifica si la esfera asociada al simplex alpha contiene el conjunto de puntos vp
    *   @param vp Apuntador de los puntos a verificar
    *   @param sizepoints Puntos a verificar
    */
    void setEmptySphereCircum(IterPoint* vp,const unsigned int& sizepoints);
    /** Calcula los intervalos Singular, Regular e Interior a partir de otros dos simplex
    *   @param sa0 Simplex 1 para la evaluación
    *   @param sa1 Simplex 2 para la evaluación
    */
    void setInterval(IterSimplexAlpha sa0,IterSimplexAlpha sa1);
    /** Obtiene el radio al cuadrado de la circunferencia
    *   @return radio al cuadrado de la circunferencia asociada
    */
    float getCuadRadioSphereCircuns();
    /** Verifica que el simple pertenezca al conv del alpha-shape
    *   @param alpha
    */
    bool isConvAlpha(const float& alpha);
    /** Puntos de definición
    *   @return Puntos de definición
    */
    int* getDefPoints();   
    /** Retorna el tamaño de los puntos de definición
    *   @return tamaño de los puntos de definición
    */
    int getSizeDefPoints() const;

    /** Almacena el resultado en una estructura simplicial
    */
    void getResult(unsigned int** simplicial,unsigned int& sizeResult);
  private:
    /** Encuentra los intervalos
    */
    void findIntervals();
    /** Retorna true si el simplex asociado pertence al convex hull
    *   @return true si el simplex pertenece al convex hull, false en caso contrario
    */
    bool isCH() const;    
    // Iterador del simplex asociado
    IterSimplex m_simplex;
    // Bandera para verificar si el simplex asociado pertenece al convex hull
    bool m_isCH;
    // Intervalo singular
    float* m_intervalSingular;
    // Intervalo regular
    float* m_intervalRegular;
    // Intervalo interior
    float* m_intervalInterior;
    // Esfera asociada
    TSphere* m_circumSphere;
    // Puntos de definción  en el simplex
    int* m_defPoints;
    // Tamaño de los puntos de definición en el simplex
    int m_sizedefPoints;
    // La esfera esta vacia
    bool m_valEmpty;    
};

template <class TAfinSpace>
SimplexAlpha<TAfinSpace>::SimplexAlpha(IterSimplex s,int* defPoints,const unsigned int& sizedefPoints,const unsigned int& vop){
  m_simplex=s;
  // Inicialmente los intervalos estan vacios
  m_intervalSingular=0;
  m_intervalRegular=0;
  m_intervalInterior=new float[1];   

  if(vop!=0 && s->getOpposite(vop)->getHiperplane().getData(TAfinSpace::dim)<0)
    m_isCH=true;  
  
  m_sizedefPoints=sizedefPoints;
  m_defPoints=new int[m_sizedefPoints];
  for(int i=0;i<m_sizedefPoints;i++){
    m_defPoints[i]=defPoints[i];    
  }

  m_circumSphere=0;
  
  findIntervals();

  if(TAfinSpace::dim+1==m_sizedefPoints){
    s->sa=this;
  }
  m_valEmpty = false;
  setModeDraw(FormatOutput::MXD);  
}

template <class TAfinSpace>
SimplexAlpha<TAfinSpace>::IterSimplex SimplexAlpha<TAfinSpace>::getSimplex(){
  // retorna el simplex
  return m_simplex;
}

template <class TAfinSpace>
SimplexAlpha<TAfinSpace>::~SimplexAlpha(){
  delete m_intervalSingular;
  delete m_intervalRegular;
  delete m_intervalInterior;
  delete m_circumSphere;
}

template <class TAfinSpace>
bool SimplexAlpha<TAfinSpace>::isCH() const{
  return m_isCH;
}

template <class TAfinSpace>
void SimplexAlpha<TAfinSpace>::findIntervals(){  
  int i=0;
  IterPoint* defpoints=new IterPoint[m_sizedefPoints];  
  for(i=0;i<m_sizedefPoints;i++){   
    defpoints[i]=Delaunay<TAfinSpace>::getOriginalPoint(m_simplex->getVert(m_defPoints[i]));    
  }  
  
  m_circumSphere=new TSphere(defpoints,m_sizedefPoints);

  // construye el intervalo correspondiente para los simplices interiores si es un d-simplex
  if(m_sizedefPoints-1==TAfinSpace::dim){
    m_intervalInterior[0]=m_circumSphere->getCuadRadius();
  }    
}


template <class TAfinSpace>
void SimplexAlpha<TAfinSpace>::setEmptySphereCircum(IterPoint* vp,const unsigned int& sizepoints){
	unsigned int i=0,j=0;
	double d;
	bool isEmpty;
	IterPoint center = m_circumSphere->getCenter();
	if(vp[0]->getDim() != center->getDim()){		
    EXCEPTION("Dimensiones no coinciden en SimplexAlpha::setPointsEvalForEmpty\n");
  	return;
	}
	// Calcula las distancias
	isEmpty = true;
	for(i=0;i<sizepoints && isEmpty==true;i++){
		d = 0;
		for(j=0;j<center->getDim();j++)
  		d+= (center->getData(j)-vp[i]->getData(j)) * (center->getData(j)-vp[i]->getData(j));

		if( d < m_circumSphere->getCuadRadius() ){
    	isEmpty=false;
			break;
		}		
	}
	m_valEmpty=isEmpty;
}

template <class TAfinSpace>
float SimplexAlpha<TAfinSpace>::getCuadRadioSphereCircuns(){
  return m_circumSphere->getCuadRadius();
}


template <class TAfinSpace>
void SimplexAlpha<TAfinSpace>::setInterval(IterSimplexAlpha sa0,IterSimplexAlpha sa1){
	double a = -2;
	double b = -2;

	if(m_valEmpty)
		a = getCuadRadioSphereCircuns();
	else{
		if(sa0!=NULL && sa1!=NULL)
			a =	(sa0->getCuadRadioSphereCircuns() < sa1->getCuadRadioSphereCircuns())?sa0->getCuadRadioSphereCircuns(): sa1->getCuadRadioSphereCircuns();
		else if(sa0!=NULL)
			a = sa0->getCuadRadioSphereCircuns();
		else if(sa1!=NULL)
			a = sa1->getCuadRadioSphereCircuns();		
	}
	if(isCH()){
  	b=-1;
	}else{
  	if(sa0!=NULL && sa1!=NULL)
			b =	(sa0->getCuadRadioSphereCircuns() < sa1->getCuadRadioSphereCircuns())?sa1->getCuadRadioSphereCircuns(): sa0->getCuadRadioSphereCircuns();
		else if(sa0!=NULL)
			b = sa0->getCuadRadioSphereCircuns();
		else if(sa1!=NULL)
			b = sa1->getCuadRadioSphereCircuns();
	}

	if(a==-2){  	
    EXCEPTION("Error en alpha shapes 1");
	}
	if(b==-2){		
  	EXCEPTION("Error en alpha shapes 2");
	}
	if(a>b && b!=-1){		
  	EXCEPTION("Error en alpha shapes 3");
		a=-3;	
		b=-3;
	}

	m_intervalRegular =  new float[2];
	m_intervalRegular[0] = a;
	m_intervalRegular[1] = b;  
}


template <class TAfinSpace>
bool SimplexAlpha<TAfinSpace>::isConvAlpha(const float& alpha){  
  if(m_intervalRegular[0]==-3&&m_intervalRegular[1]==-3){
    EXCEPTION("Valores invalidos");
		return false;
  }

	if(m_intervalRegular[0]==0)
		return false;

	if(m_intervalRegular==NULL)
		return false;	

	if(m_intervalRegular[1]==-1)
  	if(alpha>m_intervalRegular[0]){
			return true;
		}

	if(alpha>m_intervalRegular[0] && alpha<m_intervalRegular[1]){
		return true;
	}

	return false;
}


template <class TAfinSpace>
int* SimplexAlpha<TAfinSpace>::getDefPoints(){
  return m_defPoints;
}

template <class TAfinSpace>
int SimplexAlpha<TAfinSpace>::getSizeDefPoints() const{
  return m_sizedefPoints;
}

template <class TAfinSpace>
void SimplexAlpha<TAfinSpace>::getResult(unsigned int** simplicial,unsigned int& sizeResult){
  int i=0;
  SimplexAlpha<TAfinSpace>::IterSimplex simp=getSimplex();    
  typedef SimplexAlpha<TAfinSpace>::TDelaunay TDelaunay;
  typedef TDelaunay::TConvexHull TConvexHull;  
  (*simplicial)=new unsigned int[getSizeDefPoints()];
  sizeResult=getSizeDefPoints();
  for(i=0;i<getSizeDefPoints();i++){
   (*simplicial)[i]=TConvexHull::getVertexConvexHull(simp->getVert(getDefPoints()[i]))->getId();
  }
}


template <class TAfinSpace>
ostream& operator<<(ostream& os,SimplexAlpha<TAfinSpace>& s)
{ 
  if(s.getModeDraw()==SimplexAlpha<TAfinSpace>::MXD){
   int i=0;
   SimplexAlpha<TAfinSpace>::IterSimplex simp=s.getSimplex();    
   typedef SimplexAlpha<TAfinSpace>::TDelaunay TDelaunay;
   typedef TDelaunay::TConvexHull TConvexHull;   
   for(i=0;i<s.getSizeDefPoints();i++){     
     os<<TConvexHull::getVertexConvexHull(simp->getVert(s.getDefPoints()[i]))->getId()<<" ";

   }
   os<<endl;
 }else if(s.getModeDraw()==SimplexAlpha<TAfinSpace>::OBJ){
  
 }
 return os;
}



CGL_END

#endif



  