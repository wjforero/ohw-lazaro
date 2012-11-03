#ifndef KOHONENNEURON_H
#define KOHONENNEURON_H

// Inclusiones especificas
#include "base.h"
#include "../../kernel/affinespace.h"

CGL_BEGIN

/** Modela una neurona para los mapas de kohonen
*/
template <class TAffineSpace>
class KohonenNeuron{
  public:
    /** Dimensión del espacio euclideo
    */
    enum{
      dim=TAffineSpace::dim
    };
    /** Tipo de dato de la neurona
    */
    typedef typename TAffineSpace::TAffineVector TVectorNeouron;
    /** Tipo de dato del iterador
    */
    typedef typename TAffineSpace::TAffinePoint* IterPoint;
    /** Tipo de dato
    */
    typedef typename TAffineSpace::TData TData;    
  public:
    /** Construye una neurona de kohonen cuyo vector de pesos tiene pequeños valores aleatorios alrrededor
    *   del centroide.
    */
    KohonenNeuron(IterPoint centroid);
    
    /** Calcula las distancias para la evaluación de la celda ganadora.
    */
    TData getCuadDistance(IterPoint x);         

    /** Actualiza la i-esima coordenada del peso
    */
    void updateWeights(IterPoint x, const TData& nabla);

    /** Fija el id de la neurona
    */
    void setId(const unsigned int& id);

    /**
    */
    unsigned int getId()const;

    /** Retorna una referencia a los pesos
    */
    const TVectorNeouron& getWeights()const;

  private:
    // Vector con los pesos
    TVectorNeouron m_weights;
    // Id de la neurona
    unsigned int m_id;
};

template <class TAffineSpace>
KohonenNeuron<TAffineSpace>::KohonenNeuron(IterPoint centroid){
  unsigned int i=0;
  for(i=0;i<dim;i++){        
    m_weights[i]=centroid->getData(i)+(rand()/(float)RAND_MAX)*0.1*centroid->getData(i);
  }      
}
        
template <class TAffineSpace>
KohonenNeuron<TAffineSpace>::TData KohonenNeuron<TAffineSpace>::getCuadDistance(IterPoint x){
  unsigned int i=0;
  TData resul;
  resul=0;
  for(i=0;i<dim;i++){
    resul+=(m_weights.getData(i)-x->getData(i))*(m_weights.getData(i)-x->getData(i));
  }
  return resul;
}    

template <class TAffineSpace>
void KohonenNeuron<TAffineSpace>::updateWeights(IterPoint x, const TData& nabla){
  unsigned int i=0;      
  for(i=0;i<dim;i++){
    m_weights[i]=m_weights[i]+nabla*(x->getData(i) - m_weights[i]);
  }      
}

template <class TAffineSpace>
void KohonenNeuron<TAffineSpace>::setId(const unsigned int& id){
  m_id=id;
}

template <class TAffineSpace>
unsigned int KohonenNeuron<TAffineSpace>::getId() const{
  return m_id;
}

template <class TAffineSpace>
const KohonenNeuron<TAffineSpace>::TVectorNeouron& KohonenNeuron<TAffineSpace>::getWeights() const{
  return m_weights;
}


template <class TAffineSpace>
ostream& operator <<(ostream& os, const KohonenNeuron<TAffineSpace>& kn){
  unsigned int i=0;
  for(i=0;i<TAffineSpace::dim;i++){
     os<<kn.getWeights().getData(i)<<" ";
  }
  os<<endl;
  return os;
}

CGL_END


#endif