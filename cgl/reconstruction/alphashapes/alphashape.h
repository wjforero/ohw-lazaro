#ifndef ALPHASHAPE_H
#define ALPHASHAPE_H

// Inclusiones especificas
#include "../../algorithms/delaunay.h"
#include "simplexalpha.h"
#include "../../iodata/formatoutput.h"
#include "../../iodata/callbakupdate.h"

CGL_BEGIN

/** Construye el alpha-shape del conjunto de puntos
*/  
template <class TAffineSpace>
class AlphaShape : public FormatOutput
{    
  public:            
    /** Iterador para los simplexes de la triangulación
    */
    typedef typename Delaunay<TAffineSpace>::IterSimplex IterSimplex;      
    /** Tipo de dato del espacio afín
    */
    typedef typename TAffineSpace::TData TDataType;
    /** Tipo Iterador para el tipo punto en el espacio afín
    */
    typedef typename TAffineSpace::TAffinePoint TPoint;      
    /** Tipo Iterador para el tipo punto en el espacio afín
    */
    typedef typename TAffineSpace::TAffinePoint* IterPoint;      
    /** Tipo de dato para un simplex alpha
    */
    typedef SimplexAlpha<TAffineSpace> TSimplexAlpha;
    /** Tipo Iterador para un simplex alpha
    */
    typedef SimplexAlpha<TAffineSpace>* IterSimplexAlpha;
    /** Tipo iterador para el punto de Delalaunay
    */
    typedef Delaunay<TAffineSpace>::IterPointDelaunay IterPointDelaunay;
  public:
    /** Constructor por omisión, inicializa la triangulación de Delaunay y fija el valor del alpha
    */
    AlphaShape();
    /** Destructor de la clase libera la memoria de la triangulación de delaunay
    */
    ~AlphaShape();
    /** Inserta un punto del espacio afín para la cosntrucción del alpha-shape.
    *   @param p Punto a insertar.
    */
    void insertPoint(IterPoint p);
    /** Inserta el vector de puntos vect al alpha shape
    *   @param vect Vector con los puntos a insertar
    */
    void insertPoints(vector<IterPoint>& vect);
    /** Realiza el calculo de los intervalos del alpha-shape.
    */
    void alpha();      
    /** Fija el valor del alpha con el cual se hace el barrido.
    *   @valalpha valor del alpha.
    */
    void setValAlpha(float valalpha);
    /** Retorna el valor del alpha.
    *   @return retorna el valor del alpha actual.
    */ 
    float getValAlpha();
    /** Almacena en un vector de iteradores de simplex alpha el resultado del barrido
    *   @param vout vector de simplexes
    */
    void getAlpha(vector<IterSimplexAlpha>& vout);
    /** Retorna la triangulación de delaunay asociada con este alpha shape
    *   @return Triangulación de Delaunay
    */
    Delaunay<TAffineSpace>* getDel();    

    /** Obtiene el resultado
    */
    void getResult(vector<IterPoint>& vect,unsigned int** simplicial,unsigned int& size);
public:
	CallBackUpdate* m_ou;
	void setObjectUpdate(CallBackUpdate* ou){
		m_ou=ou;
	}	

  private:
    // Triangulación de delaunay del espacio
    Delaunay<TAffineSpace>* m_del;
    // Dimensión en la cual se calcula el alpha-shape
    int m_dimension;
    // Vector con los simplex alpha
    vector<IterSimplexAlpha> m_alphaSimplex;
    // Valor de alpha
    float m_valalpha;
};


template <class TAffineSpace>
AlphaShape<TAffineSpace>::AlphaShape(){
  // Fija los valores iniciales
  m_valalpha=0;
  m_del=new Delaunay<TAffineSpace>();
  m_dimension=TAffineSpace::dim;

	// Fija los valores del object update
	m_ou=0;
}

template <class TAffineSpace>
AlphaShape<TAffineSpace>::~AlphaShape(){
  delete m_del;
}

template <class TAffineSpace>
void AlphaShape<TAffineSpace>::insertPoint(IterPoint p){
  // Inserta el punto en la triangulación de Delaunay
  m_del->insertPoint(p);
}

template <class TAffineSpace>
void AlphaShape<TAffineSpace>::insertPoints(vector<IterPoint>& vect){
  unsigned int i=0;
	if(m_ou){
		m_ou->setIniVal(0);
		m_ou->setLastVal(vect.size());
	}
  for(i=0;i<vect.size();i++){
    insertPoint(vect[i]);
		if(m_ou)
			m_ou->updateVal(i);
  }
}

template <class TAffineSpace>
void AlphaShape<TAffineSpace>::alpha(){  
  unsigned int a=0, i=0, j=0;
  IterSimplexAlpha saDimd;
  vector<IterSimplex> m_listSimp;  
  m_del->getResult(m_listSimp);    
  

  int* vint=new int[m_dimension+1];
  for(i=0;i<m_dimension+1;i++)
    vint[i]=i+1;

  
  
  // Calcula los simplices para la d+1
  for(i=0;i<m_listSimp.size();i++){    
    saDimd=new TSimplexAlpha(m_listSimp[i],vint,m_dimension+1,0);
  }  

  // Escribe los simplices que pertenecen al CH del conjunto de puntos
  for(int ij=0;ij<m_listSimp.size();ij++){    
    IterSimplex s=m_listSimp[ij];

    for(i=1;i<=TAffineSpace::dim+1;i++){
      IterSimplex T=s->getOpposite(i);
      int* vintDimLess=new int[TAffineSpace::dim];
      int k=0;
      for(j=1;j<=TAffineSpace::dim+1;j++)
        if(j!=i)
          vintDimLess[k++]=j;      
      
      IterSimplexAlpha saDimLessd=new TSimplexAlpha(s,vintDimLess,TAffineSpace::dim,i);
      IterPoint vp[2];      
      
      vp[0] = Delaunay<TAffineSpace>::getOriginalPoint(s->getVert(i));
      vp[1] = Delaunay<TAffineSpace>::getOriginalPoint(s->getVert(s->indexOpposite(i)));            

      // Verifica que la esfera del simplex alpha sea vacia
      saDimLessd->setEmptySphereCircum(vp,2);
      IterSimplexAlpha vsa[2];
      vsa[0]=s->sa;
      vsa[1]=T->sa;      
      saDimLessd->setInterval(s->sa,T->sa);
      m_alphaSimplex.push_back(saDimLessd);
    }
  }

  delete []vint;
}

template <class TAffineSpace>
void AlphaShape<TAffineSpace>::setValAlpha(float valalpha){
  // fija el valor del alpha
  m_valalpha=valalpha;
}

template <class TAffineSpace>
float AlphaShape<TAffineSpace>::getValAlpha(){
  // retorna el valor del alpha
  return m_valalpha;
}


template <class TAffineSpace>
void AlphaShape<TAffineSpace>::getAlpha(vector<IterSimplexAlpha>& vout){  
  // Almacena en el vector vout los simplexes que quedan en el barrido
  int i=0;
  for(i=0;i<m_alphaSimplex.size();i++){
    if(m_alphaSimplex[i]->isConvAlpha(m_valalpha))
        vout.push_back(m_alphaSimplex[i]);
  }	
}

template <class TAffineSpace>
Delaunay<TAffineSpace>* AlphaShape<TAffineSpace>::getDel(){
  // retorna la triangualación de Delaunay
  return m_del;
}

template <class TAffineSpace>
void AlphaShape<TAffineSpace>::getResult(vector<IterPoint>& vect,unsigned int** simplicial,unsigned int& size){
  int i=0,j=0,k=0;
  unsigned int sizeResult=0;
  vector<AlphaShape<TAffineSpace>::IterSimplexAlpha> vout;
  IterPoint p;
  getAlpha(vout);  

  Delaunay<TAffineSpace>* delAlpha=getDel();  
  vect.clear();
  unsigned int* resultSimplex;

  for(i=0;i<delAlpha->getSizePoints();i++){
    p=new TPoint();
    for(j=0;j<TAffineSpace::dim;j++){      
      (*p)[j]=(delAlpha->getOriginalPoint(i))->getData(j);      
    }
    vect.push_back(p);

  }

  if(vout.size()==0){
    return;
  }

  (*simplicial)=new unsigned int[TAffineSpace::dim*vout.size()];

  k=0;
  for(i=0;i<vout.size();i++){
    vout[i]->getResult(&resultSimplex,sizeResult);
    for(j=0;j<sizeResult;j++){
      (*simplicial)[k++]=resultSimplex[j];      
    }
    delete[] resultSimplex;
  }
  size=vout.size();
}

/** Imprime los puntos en diferentes formatos en un ostream
*/
template <class TAffineSpace>
ostream& operator<<(ostream& os,AlphaShape<TAffineSpace>& as){
  int i=0,j=0;
  if(as.getModeDraw()==SimplexAlpha<TAffineSpace>::MXD){
    vector<AlphaShape<TAffineSpace>::IterSimplexAlpha> vout;
    as.getAlpha(vout);  

    Delaunay<TAffineSpace>* delAlpha=as.getDel();
    os<<delAlpha->getSizePoints()<<" "<<vout.size()<<endl;

    for(i=0;i<delAlpha->getSizePoints();i++){
      for(j=0;j<TAffineSpace::dim;j++){
        os<<(delAlpha->getOriginalPoint(i))->getData(j)<<" ";
      }
			os<<"0";
      os<<endl;    
    }

    for(i=0;i<vout.size();i++){
      os<<*(vout[i]);
    }
  }else if(as.getModeDraw()==FormatOutput::OBJ){
		vector<AlphaShape<TAffineSpace>::IterPoint>* vectResultAlpha=new vector<AlphaShape<TAffineSpace>::IterPoint>();
		unsigned int* indexResultAlpha;
		unsigned int sizeResultAlpha;
    as.getResult(*vectResultAlpha,&indexResultAlpha,sizeResultAlpha);				
		for(i=0;i<vectResultAlpha->size();i++){			
			os<<"v ";
			for(j=0;j<TAffineSpace::dim;j++){
				os<<(*vectResultAlpha)[i]->getData(j)<<" ";
			}			
			os<<endl;
		}		

		int k=0;		
		for(i=0;i<sizeResultAlpha;i++){
			os<<"f ";
			for(j=0;j<TAffineSpace::dim;j++){				
				os<<(indexResultAlpha[k])<<" ";
				k++;				
			}			
			os<<endl;
		}		
	}else if(as.getModeDraw()==FormatOutput::XML){
		vector<AlphaShape<TAffineSpace>::IterPoint>* vectResultAlpha=new vector<AlphaShape<TAffineSpace>::IterPoint>();
		unsigned int* indexResultAlpha;
		unsigned int sizeResultAlpha;
    as.getResult(*vectResultAlpha,&indexResultAlpha,sizeResultAlpha);
		os<<"<model>"<<endl;
		os<<"<dim>"<<TAffineSpace::dim<<"</dim>";
		os<<"<sizepoints>"<<vectResultAlpha->size()<<"</sizepoints>";
		os<<"<sizemesh>"<<sizeResultAlpha<<"</sizemesh>";
		
		os<<"<points>"<<endl;
		for(i=0;i<vectResultAlpha->size();i++){
			os<<"<point>";
			for(j=0;j<TAffineSpace::dim;j++){
				os<<"<data>"<<(*vectResultAlpha)[i]->getData(j)<<"</data>";
			}
			os<<"</point>";
			os<<endl;
		}
		os<<"</points>"<<endl;
		
		int k=0;
		os<<"<mesh>"<<endl;
		for(i=0;i<sizeResultAlpha;i++){			
			os<<"<polygon>";
			for(j=0;j<TAffineSpace::dim;j++){				
				os<<"<index>"<<indexResultAlpha[k]<<"</index>";
				k++;
			}
			os<<"</polygon>";
		}
		os<<"</mesh>"<<endl;
		os<<"</model>"<<endl;
  }	
	cout<<"Pasa algo\n";
  return os;  
}

CGL_END

#endif