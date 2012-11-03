#ifndef KOHONENRECONSTRUCTION_H
#define KOHONENRECONSTRUCTION_H

// Inclusiones especificas
#include "base.h"
#include "kohonenneuron.h"
#include "../../algorithms/utils.h"

CGL_BEGIN

/** Modela la reconstucción usando mapas autoorganizativos de kohonen
*/
template <class TAffineSpace>
class KohonenReconstruction
{
  public:
    /** Dimensión del espacio euclideo sobre el cual se hacen los calculos
    */
    enum{
      dim=TAffineSpace::dim
    };
    /** Tipo de dato de la neurona
    */
    typedef typename KohonenNeuron<TAffineSpace> TKohonenNeuron;
    /** Tipo iterador para la neurona
    */
    typedef typename KohonenNeuron<TAffineSpace>* IterKohonenNeuron;
    /** Tipo de punto para este espacio afin
    */
    typedef typename TAffineSpace::TAffinePoint TPoint;
    /** Tipo iterador para el punto en este espacio afin
    */
    typedef typename TAffineSpace::TAffinePoint* IterPoint;
    /** Tipo de dato
    */
    typedef typename TAffineSpace::TData TData;
  public:    
    /** Constructor por omisión
    */
    KohonenReconstruction();

    /** Destructor
    */
    ~KohonenReconstruction();

    /** Reconstruye a partir de un vector de puntos
    */
    void reconstruct(vector<IterPoint>& points,const unsigned int& maxnInter);

    /** Fija los tamaños de la salida
    */
    void setSizeOutput(const unsigned int& sizew,const unsigned int& sizeh);    

    /** Fija los parametros iniciales para la reconstrucción
    *   @param nIterations Número máximo de iteraciones
    *   @param nRadiusIni Radio inicial
    */
    void setInitParameters(const unsigned int& nIterations,const unsigned int& nRadiusIni);    

    /** Reconstruye con valores por omisión para la reconstrucción
    *   @param points Puntos en la reconstrucción
    *   @maxnInter Número máximo de iteraciones
    */
    void reconstructDefaultParameters(vector<IterPoint>& points,const unsigned int& maxnInter);

    /** Obtiene el resultado
    */
    void getResult(vector<IterPoint>& vect,unsigned int** simplicial,unsigned int& size);
  
  protected:
    /** Retorna el radio la vecindad
    */
    TData getRadius(const unsigned int& t,const unsigned int& nNeurons);
  
    /** Retorna el valor de la función de ganancia
    */
    TData getGainFunction(unsigned int t);    

    /** Fija la topologia de la capa de salida
    */
    void setNetTopology(Matrix<unsigned int>& mdist,const unsigned int& sizew,const unsigned int& sizeh);      
    
    /** Fija el valor de atencuación para la función de ganancia
    */
    void setAtteGain(const TData& atteGainFunction);

    /** Fija el valor de atencuación para la función de vecindad
    */
    void setAtteNeighbor(const TData& atteNeighbor);
  
  private:
    // Valor del radio
    TData m_radiusValue;
    // Número máximo de iteraciones
    unsigned int m_maxT;
    // Número de neuronas
    unsigned int m_nNeurons;
    // Ancho de la capa de salida
    unsigned int m_sizew;
    // Alto de la capa de salida
    unsigned int m_sizeh;
    // Vector con las neuronas
    vector<IterKohonenNeuron> m_outputLayer;
    // Valor de atenuación de la función de ganacia
    TData m_atteGainFunction;
    // Valor de atenuación de la función de vecindad
    TData m_atteNeighbor;
};

template <class TAffineSpace>
KohonenReconstruction<TAffineSpace>::KohonenReconstruction(){      
  m_atteGainFunction=1000000;  
  m_atteNeighbor=40;
}

template <class TAffineSpace>
KohonenReconstruction<TAffineSpace>::~KohonenReconstruction(){      
}

template <class TAffineSpace>
void KohonenReconstruction<TAffineSpace>::setSizeOutput(const unsigned int& sizew,const unsigned int& sizeh){
  m_sizew=sizew;
  m_sizeh=sizeh;
}

template <class TAffineSpace>
void KohonenReconstruction<TAffineSpace>::reconstruct(vector<IterPoint>& points,const unsigned int& maxnInter){
  IterPoint p;
  TData resMin=0,tv=0,gainfuntion=0;
  unsigned int i=0,j=0,n=points.size(),minIndex=0,t=0;  
  unsigned int id;      
  IterKohonenNeuron winCell=0,tCell;
  vector<IterKohonenNeuron> neighborhood;
  m_nNeurons=m_sizew*m_sizeh;      
  Matrix<unsigned int> mdist(m_nNeurons,m_nNeurons,0);  

  m_outputLayer.clear();
  
  // Calcula el centroide de los puntos
  TPoint centroid;
  centroid.setData(0);
  for(j=0;j<dim;j++){
    for(i=0;i<n;i++){
      centroid[j]=centroid[j]+points[i]->getData(j);
    }
    centroid[j]=centroid[j]/n;
  }
    

  // Crea las neuronas  
  m_outputLayer.clear();  

  for(i=0;i<m_nNeurons;i++){
    IterKohonenNeuron ikn=new TKohonenNeuron(&centroid);
    ikn->setId(i);
    m_outputLayer.push_back(ikn);      
  }  
  
  setNetTopology(mdist,m_sizew,m_sizeh);  

  // Barajar los puntos
  shuffle(points,1);

  // Cliclo principal del algoritmo
  t=0;
  int in=0;    
  while(t<=m_maxT){
    // Escoger un nuevo vector de entrada    
    if(in==points.size())
      in=0;
    p=points[in++];    

    // Calcular la distancia de cada neurona de la salida a la entrada
    // Selección de la celda ganadora
    resMin=-1;
    for(j=0;j<m_nNeurons;j++){
      tv=m_outputLayer[j]->getCuadDistance(p);          
      if(j==0){
        resMin=tv;
        minIndex=0;
      }
      if(tv<resMin){
        resMin=tv;
        minIndex=j;
      }          
    }    

    winCell=m_outputLayer[minIndex];
    // Calculo del conjunto de vecindades para la celda ganadora
    tv=getRadius(t,m_nNeurons);        
    
    neighborhood.clear();
    id=winCell->getId();                
    for(j=0;j<m_nNeurons;j++){
      if(mdist[id][j] <=  tv){
        neighborhood.push_back(m_outputLayer[j]);            
      }
    }

    if(neighborhood.size()==1)
      break;
    
    // Actualización de los pesos de las neuronas para las vecindades
    gainfuntion=getGainFunction(t);    
    
    for(i=0;i<neighborhood.size();i++){
      tCell=neighborhood[i];
      tCell->updateWeights(p,gainfuntion);
    }
    t++;    
  }
  char nombresito[50];
  sprintf(nombresito,"sali%d.m3d",t);

  fstream fs("salidita.m3d",ios::out);
  fs<<m_outputLayer.size()<<" 0"<<endl;
  for(i=0;i<m_nNeurons;i++){
    fs<<(*m_outputLayer[i]);
  }
}

template <class TAffineSpace>
KohonenReconstruction<TAffineSpace>::TData KohonenReconstruction<TAffineSpace>::getRadius(const unsigned int& t,const unsigned int& m_nNeurons){  
  m_radiusValue=m_radiusValue*(1-(t/(m_atteNeighbor*m_maxT)));
  return m_radiusValue;
}

template <class TAffineSpace>
KohonenReconstruction<TAffineSpace>::TData KohonenReconstruction<TAffineSpace>::getGainFunction(unsigned int t){  
  return 0.4*exp(-0.5*t*t/m_atteGainFunction);
}


template <class TAffineSpace>
void KohonenReconstruction<TAffineSpace>::setInitParameters(const unsigned int& nIterations,const unsigned int& nRadiusIni){
  m_radiusValue=nRadiusIni;
  m_maxT=nIterations;
}


template <class TAffineSpace>
void KohonenReconstruction<TAffineSpace>::setNetTopology(Matrix<unsigned int>& mdist,const unsigned int& sizew,const unsigned int& sizeh){
  unsigned int i=0,j=0,k=0,l=0;
  unsigned int sizeneu=sizew*sizeh;
  mdist.newsize(sizeneu,sizeneu);
  for(i=0;i<sizew;i++){
    for(j=0;j<sizeh;j++){
      for(k=0;k<sizew;k++){
        for(l=0;l<sizeh;l++){              
          mdist[i*sizew+j][k*sizeh+l]=(unsigned int)(abs(i-k)+abs(j-l));
          mdist[k*sizew+l][i*sizeh+j]=(unsigned int)(abs(i-k)+abs(j-l));
        }
      }
    }
  } 
}

template <class TAffineSpace>
void KohonenReconstruction<TAffineSpace>::reconstructDefaultParameters(vector<IterPoint>& points,const unsigned int& maxnInter){  
  m_sizew=m_sizeh=(int)(2*sqrt(points.size()));    
  setInitParameters(maxnInter,(m_sizew+m_sizew)/3);
  reconstruct(points,maxnInter);
}

template <class TAffineSpace>
void KohonenReconstruction<TAffineSpace>::setAtteGain(const TData& atteGainFunction){
  m_atteGainFunction=atteGainFunction;
}

template <class TAffineSpace>
void KohonenReconstruction<TAffineSpace>::setAtteNeighbor(const TData& atteNeighbor){
  m_atteNeighbor=atteNeighbor;
}


template <class TAffineSpace>
void KohonenReconstruction<TAffineSpace>::getResult(vector<IterPoint>& vect,unsigned int** simplicial,unsigned int& size){
  int i=0,j=0,k=0;
  unsigned int sizeResult=0;
  IterPoint p;

  // Carga los puntos de la red de salida
  for(i=0;i<m_outputLayer.size();i++){
    p=new TPoint();
    for(j=0;j<TAffineSpace::dim;j++){      
      (*p)[j]=m_outputLayer[i]->getWeights().getData(j);      
    }
    vect.push_back(p);
  }
    m_sizew;
    m_sizeh;

  (*simplicial)=new unsigned int[(m_sizew-1)*(m_sizeh-1)*6];
  k=0;
  
  for(i=1;i<m_sizew;i++){
    for(j=0;j<m_sizeh-1;j++){
      (*simplicial)[k++]=(i-1)*m_sizew+j+1;
      (*simplicial)[k++]=(i-1)*m_sizew+j+2;
      (*simplicial)[k++]=(i)*m_sizew+j+1;
      
      (*simplicial)[k++]=(i-1)*m_sizew+j+2;
      (*simplicial)[k++]=(i)*m_sizew+j+1;
      (*simplicial)[k++]=(i)*m_sizew+j+2;
    }
  }

  size=(m_sizew-1)*(m_sizeh-1)*2;    
}


CGL_END


#endif