#ifndef SLICE_H
#define SLICE_H

// Inclusiones especificas
#include "base.h"
#include "../../algorithms/delaunay.h"

CGL_BEGIN


template <class TAffineSpace>
class Slice;

template <class TAffineSpace>
  class VertSliceReconstruction : public TAffineSpace::TAffinePoint
  {
    public:
      enum{
        dim=TAffineSpace::dim
      };
      typedef typename CGAffineSpace<typename TAffineSpace::TData,dim+1> TAffineSpaceUp;
      typedef typename Slice<TAffineSpaceUp> TSliceUp;
      typedef typename Slice<TAffineSpaceUp>* IterSliceUp;
      typedef typename Slice<TAffineSpaceUp>::IterPoint IterPointUp;

      typedef typename TAffineSpace::TAffinePoint TPoint;
      typedef typename TAffineSpace::TAffinePoint* IterPoint;
      typedef Slice<TAffineSpace>* IterSlice;
      typedef Slice<TAffineSpace> TSlice;
      

    public:
      VertSliceReconstruction():Point(){
        m_point=0;        
        m_slice=0;
      }
      VertSliceReconstruction(IterPoint p) : TPoint(*p){
        m_point=p;
        m_slice=0;
      }
      
      long getId() const{
        return m_id;
      }

      void setId(long id){
        m_id=id;
      }

      IterSlice getSlice() const{
        return m_slice;
      }

      void setSlice(IterSlice slice){
        m_slice=slice;
      }

      IterSliceUp getSliceUp() const{
        return m_sliceup;
      }

      void setSliceUp(IterSliceUp sliceup){
        m_sliceup=sliceup;
      }

      IterPoint getPoint(){        
        return m_point;
      }

      IterPointUp m_pointUp;
    private:
      IterPoint m_point;      
      long m_id;
      IterSlice m_slice;
      IterSliceUp m_sliceup;
      
  };


/** Esta clase representa un slice para la recosntrucción
*/
template <class TAffineSpace>
class Slice{
public:
  /** Tipo de dato del punto
  */
  typedef TAffineSpace::TAffinePoint TPoint;
  /** Iterador del punto
  */
  typedef TAffineSpace::TAffinePoint* IterPoint;

  typedef VertSliceReconstruction<TAffineSpace> TVertSliceReconstruction;
  typedef VertSliceReconstruction<TAffineSpace>* IterVertSliceReconstruction;
public:
  /** Constructor por omisión (por omisión el slice tiene un punto Forero, Aranzazu, Flórez)
  */
  Slice();

  /** Constructor un slice para size puntos  
  */
  Slice(const unsigned int& size);

  /** Destructor
  */
  ~Slice();

  /** Función de acceso a los datos, el punto final coincide con el inicial.
  *   @param i indice del punto.
  */
  IterPoint getPoint(const unsigned int& i);

  /** Función para fijar los datos, se asume que los datos tienen
  *   orientación antihoraria
  */
  void setPoint(IterPoint p,const unsigned int& i);

  /** Retorna los puntos del slice
  */
  unsigned int getSize() const;

  /** Redimensiona el tamaño del slice al size
  */
  void newSize(const unsigned int& size);

  /** Fija el nivel en el que se encuentra el slice
  */
  void setLevel(const unsigned int& level);

  /** Retorna el nivel en el que se encuentra un slice
  */
  unsigned int getLevel()const;

protected:
  /** Carga a memoria el slice
  */
  void initialize(const unsigned int& size);

  /** Libera memoria del slice
  */
  void destroy();

private:
  // Vector con los puntos del slice
  IterPoint* m_points;
  // Numero de puntos en el slice
  unsigned int m_size;
  // Nivel del slice
  unsigned int m_level;
};

template <class TAffineSpace>
Slice<TAffineSpace>::Slice():m_points(0),m_size(1){
  initialize(m_size);
}

template <class TAffineSpace>
Slice<TAffineSpace>::Slice(const unsigned int& size):m_points(0),m_size(0){
    initialize(size);
}

template <class TAffineSpace>
Slice<TAffineSpace>::~Slice(){
    destroy();
}

template <class TAffineSpace>
void Slice<TAffineSpace>::initialize(const unsigned int& size){  
  unsigned int i=0;
  m_size=size;
  if(m_points!=0)
    destroy();
  if(m_size!=0)
    m_points=new IterPoint[m_size];

  for(i=0;i<size;i++){
    m_points[i]=0;
  }
}

template <class TAffineSpace>
void Slice<TAffineSpace>::destroy(){  
  if(m_points!=0)
    delete []m_points;  
  m_points=0;
}


template <class TAffineSpace>
Slice<TAffineSpace>::IterPoint Slice<TAffineSpace>::getPoint(const unsigned int& i){
  return m_points[i%m_size];
}

template <class TAffineSpace>
void Slice<TAffineSpace>::setPoint(IterPoint p,const unsigned int& i){
  IterVertSliceReconstruction vs=new TVertSliceReconstruction(p);


  m_points[i%m_size]=vs;
}

template <class TAffineSpace>
void Slice<TAffineSpace>::newSize(const unsigned int& size){
  if(size!=0)
    initialize(size);
}

template <class TAffineSpace>
void Slice<TAffineSpace>::setLevel(const unsigned int& level){
  m_level=level;
}

template <class TAffineSpace>
unsigned int Slice<TAffineSpace>::getLevel()const{
  return m_level;
}

template <class TAffineSpace>
unsigned int Slice<TAffineSpace>::getSize() const{
  return m_size;
}

template <class TAffineSpace>
ostream& operator <<(ostream& os, Slice<TAffineSpace>& s){
  unsigned int i=0;
  for(i=0;i<s.getSize();i++){
    if(s.getPoint(i)!=0)
      os<<*(s.getPoint(i))<<endl;
  }
  return os;
}


CGL_END

#endif