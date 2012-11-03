#ifndef SLICERECONSTRUCTION_H
#define SLICERECONSTRUCTION_H

// Inclusiones especificas
#include "base.h"
#include "slice.h"
#include "../../algorithms/delaunay.h"

#include <list>
using namespace std;

CGL_BEGIN  


template <class TAffineSpace>
class SliceReconstruction{
public:
  enum{
    dim=TAffineSpace::dim
  };
  typedef typename CGAffineSpace<typename TAffineSpace::TData,dim-1> TAffineSpaceDown;
  typedef typename TAffineSpaceDown::TAffinePoint TPointDown;
  typedef typename TAffineSpaceDown::TAffinePoint* IterPointDown;
  typedef Delaunay<TAffineSpaceDown> TDelaunayDown;
  typedef VertSliceReconstruction<TAffineSpaceDown>* IterVertSliceReconstructionDown;
  typedef VertSliceReconstruction<TAffineSpaceDown> TVertSliceReconstructionDown;
  typedef typename Delaunay<TAffineSpaceDown>::IterSimplex IterSimplexDel;
  


  typedef TAffineSpace::TData TData;
  typedef Slice<TAffineSpace> TSlice;
  typedef Slice<TAffineSpace>* IterSlice;
  typedef list<IterSlice> TListSlice;
  typedef list<IterSlice>* IterListSlice;
  typedef typename TSlice::IterPoint IterPoint;
  typedef typename TSlice::TPoint TPoint;

  typedef typename TSlice::TVertSliceReconstruction TVertSliceReconstruction;
  typedef typename TSlice::IterVertSliceReconstruction IterVertSliceReconstruction;

  typedef typename TAffineSpaceDown::TAffinePoint* IterPointDown;
  typedef typename TAffineSpaceDown::TAffinePoint TPointDown;


public:  
  /** Construye una estructura de reconstrucción con size slices
  */
  SliceReconstruction(const unsigned int& size);


  /** Destructor
  */
  ~SliceReconstruction();

  /** Fija el slice i
  */
  void addSliceInLevel(IterSlice is, const unsigned int& level);

  /** Retorna el slice i
  */
  IterListSlice getLevel(const unsigned int& level);

  /** Reconstruye entre niveles vecinos
  */
  void reconstruct();

  unsigned int getSizeLevel()const;

  vector<IterPoint> m_triangles;

  void findCircle(TData &cx,TData &cy,TData x1,TData y1,TData x2,TData y2,TData x3,TData y3);

	/**
  	retorna -1 si el punto esta dentro del poligono
		retorna 0 en otro caso
	*/
  int getPosition(IterSlice slice,TData cx,TData cy);
  
  int orientation(TData p1x,TData p1y,TData p2x,TData p2y,TData cx,TData cy);

  void simpleReconstructionBanching(IterListSlice listSliceA,IterListSlice listSliceB);

  vector<IterPointDown> vectorPointVoronoi;
  
  TDelaunayDown delDown;

  /** Obtiene el resultado
  */
  void getResult(vector<IterPoint>& vect,unsigned int** simplicial,unsigned int& size);


protected:
  void initialize(const unsigned int& size);

  void destroy();  



protected:
  /** Reconstruye dos slices con la tecnica de Forero, Aranzazu, Florez
  *   Ingenieria e investigación 48 Reconstrucción 3d usando superficies
  *   trianguladas dadas contornos paralelos
  */
  void simpleReconstruction(IterSlice s1,IterSlice s2);

  /** Retorna la distancia entre dos puntos
  */
  TData cuadDist(IterPoint p1,IterPoint p2);

  /** Retorna true si un triangulo esta formado por puntos en diferentes slices
  */
  bool isConectDiferentSlices(IterSimplexDel sd);

  /** Cuando hay vertices de voronoi
  */
  bool isConectDiferentSlicesVoronoi(IterSimplexDel sd);

private:  
  // Vector con los slices
  vector<IterListSlice> m_slices;

  // Numero de puntos en los slices
  unsigned int m_numberPoints;
  
};

template <class TAffineSpace>
SliceReconstruction<TAffineSpace>::SliceReconstruction(const unsigned int& size):m_slices(size),m_numberPoints(0){
  initialize(size);
}

template <class TAffineSpace>
SliceReconstruction<TAffineSpace>::~SliceReconstruction(){
  destroy();
}

template <class TAffineSpace>
void SliceReconstruction<TAffineSpace>::initialize(const unsigned int& size){
  m_slices.resize(size);
  unsigned int i=0;
  for(i=0;i<m_slices.size();i++){
    m_slices[i]=new TListSlice();
  }
}

template <class TAffineSpace>
void SliceReconstruction<TAffineSpace>::destroy(){
  unsigned int i=0;
  for(i=0;i<m_slices.size();i++){
    delete m_slices[i];
  }
}
  
template <class TAffineSpace>
void SliceReconstruction<TAffineSpace>::addSliceInLevel(IterSlice is, const unsigned int& level){
  is->setLevel(level);
  m_slices[level]->push_back(is);
}

template <class TAffineSpace>
unsigned int SliceReconstruction<TAffineSpace>::getSizeLevel() const{
  return m_slices.size();
}


template <class TAffineSpace>
SliceReconstruction<TAffineSpace>::IterListSlice SliceReconstruction<TAffineSpace>::getLevel(const unsigned int& level){
  return m_slices[level];
}
  
template <class TAffineSpace>
void SliceReconstruction<TAffineSpace>::reconstruct(){
  unsigned int i=0,js1=0,js2=0;  
  for(i=0;i<m_slices.size()-1;i++){    
    js1=m_slices[i]->size();
    js2=m_slices[i+1]->size();
    if(js1==1 && js1==1){
      simpleReconstruction(*(m_slices[i]->begin()),*(m_slices[i+1]->begin()));
    }

    if((js1>=2 && js1>=1) ||  (js1>=1 && js1>=2)) {
      simpleReconstructionBanching(m_slices[i],m_slices[i+1]);
    }
  }
}

template <class TAffineSpace>
void SliceReconstruction<TAffineSpace>::simpleReconstruction(IterSlice A,IterSlice B){
  int i=0,j=0,k=0;
  unsigned int indexB0=0;
  unsigned int indexA0=0;
  unsigned char bandA=0,bandB=0;
  TData minDist=0,t=0;
  // Encontrar el punto más cercano en B0 a A0  
  for(i=0;i<B->getSize();i++){
    t=cuadDist(A->getPoint(0),B->getPoint(i));
    if(i==0){
      minDist=t;
      indexB0=0;      
    }
    if(t<minDist){
      indexB0=i;
      minDist=t;
    }
  }

  // Se aplica el criterio de selección
  // también debe aplicarse en el otro sentido solo que hay
  // que tener cuidado con la orientación de los triangulos
  // El triangulo escogido es A0,B0,B1
  // El siguiente triangulo tiene como primer edge A10 B1

  // Se continua la escogencia del mejor triangulo
  // de acuerdo a las condiciones topológicas

  j=indexA0;  
  k=indexB0;
  
  while(bandA==0 || bandB==0){
    if(cuadDist(A->getPoint(j),B->getPoint(k+1)) <
      cuadDist(A->getPoint(j+1),B->getPoint(k)) ){
      // El triangulo es j,k,k+1      
            
      // triangulo 1      
      m_triangles.push_back(A->getPoint(j));
      m_triangles.push_back(B->getPoint(k));
      m_triangles.push_back(B->getPoint(k+1));

      // El siguiente es j,k+1,j+1
      // triangulo 2
      m_triangles.push_back(A->getPoint(j));
      m_triangles.push_back(B->getPoint(k+1));
      m_triangles.push_back(A->getPoint(j+1));
    }else{

      // El triangulo es j,k,j+1
      // triangulo 1      
      m_triangles.push_back(A->getPoint(j));
      m_triangles.push_back(B->getPoint(k));
      m_triangles.push_back(A->getPoint(j+1));

      // El siguiente es k,j+1,k+1
      // triangulo 2
      m_triangles.push_back(B->getPoint(k));
      m_triangles.push_back(A->getPoint(j+1));
      m_triangles.push_back(B->getPoint(k+1));

    }

    //  Si el primer punto de cada corte es nuevamente
    //  un vertice de un triangulo sale    
    //if((j+1)%(3*A->getSize()+indexA0)==0){
    if((j+1)%(A->getSize()+indexA0)==0){
      bandA=1;
      j++;      
    }
    if(bandA==0)
      j++;    
    
    if((k+1)%(B->getSize()+indexB0)==0){
      k++;
      bandB=1;      
    }
    if(bandB==0)
      k++;

    
  }  
}


template <class TAffineSpace>
void SliceReconstruction<TAffineSpace>::simpleReconstructionBanching(IterListSlice listSliceA,IterListSlice listSliceB){  
  unsigned int ic=0,j=0,k=0;
  bool isValidVoronoiNewVertex = false;			
	bool m=true;
  TData cx=0,cy=0;
  TData vs1=0,vs2=0;
  // Slice adicional
  IterSlice sliceVoronoi=new TSlice();
  vector<IterVertSliceReconstructionDown> pointsDelDown;
  
  TDelaunayDown::TVectorResult resultDelDown;
  vector<IterSlice> allSlices;  
  TListSlice::iterator is;
  IterVertSliceReconstructionDown p1,p2,p3;

  // Obtiene los puntos de los slices
  IterListSlice isl=listSliceA;
  m_numberPoints=0;
  for(is=isl->begin();is!=isl->end();is++){                
    IterSlice sv=*is;    
    allSlices.push_back(sv);
    for(j=0;j<sv->getSize();j++){            
      IterPointDown pd=new TPointDown();
      ((IterVertSliceReconstruction)sv->getPoint(j))->setSlice(sv);      
      for(k=0;k<TAffineSpace::dim-1;k++){
        (*pd)[k]=sv->getPoint(j)->getData(k);
      }
      vs1=(*pd)[TAffineSpace::dim-1]=sv->getPoint(j)->getData(k);
      IterVertSliceReconstructionDown vsr=new TVertSliceReconstructionDown(pd);
      IterVertSliceReconstruction vsrup=(IterVertSliceReconstruction)sv->getPoint(j);
      vsr->setSliceUp(sv);
      pointsDelDown.push_back(vsr);
      vsr->m_pointUp=vsrup;
    }
  }

  isl=listSliceB;
  for(is=isl->begin();is!=isl->end();is++){            
    IterSlice sv=*is;    
    allSlices.push_back(sv);
    for(j=0;j<sv->getSize();j++){      
      ((IterVertSliceReconstruction)sv->getPoint(j))->setSlice(sv);
      IterPointDown pd=new TPointDown();
      for(k=0;k<TAffineSpace::dim-1;k++){
        (*pd)[k]=sv->getPoint(j)->getData(k);                  
      }      
      vs2=(*pd)[TAffineSpace::dim-1]=sv->getPoint(j)->getData(k);
      IterVertSliceReconstructionDown vsr=new TVertSliceReconstructionDown(pd);
      IterVertSliceReconstruction vsrup=(IterVertSliceReconstruction)sv->getPoint(j);
      vsr->setSliceUp(sv);
      pointsDelDown.push_back(vsr);
      vsr->m_pointUp=vsrup;
    }
  }

  cout<<"Empieza delaunay para "<<pointsDelDown.size()<<" puntos "<<endl;
  for(ic=0;ic<pointsDelDown.size();ic++){    
    if(ic%10==0)
      cout<<"Inserta el punto "<<ic<<endl;
    delDown.insertPoint(pointsDelDown[ic]);
  }

  delDown.getResult(resultDelDown);  
  
  // Solo para los triangulos que conectan puntos en slices distintos
  for(TDelaunayDown::TVectorResult::iterator i=resultDelDown.begin();i!=resultDelDown.end();i++){
    IterSimplexDel sd=*i;    
		if(isConectDiferentSlices(sd)){      
    	// Calcula el vertice de voronoi
			IterPointDown pointVoronoi = new TPointDown();						
			/////////////////////////////////////////////////
      cx=0;
      cy=0;
			
      p1=(IterVertSliceReconstructionDown)TDelaunayDown::getOriginalPoint(sd->getVert(1));
      p2=(IterVertSliceReconstructionDown)TDelaunayDown::getOriginalPoint(sd->getVert(2));
      p3=(IterVertSliceReconstructionDown)TDelaunayDown::getOriginalPoint(sd->getVert(3));

      
      findCircle(cx,cy,p1->getData(0),p1->getData(1),p2->getData(0),p2->getData(1),p3->getData(0),p3->getData(1));
			
			(*pointVoronoi)[0]=cx;
			(*pointVoronoi)[1]=cy;           

			// Escoger los puntos que hagan parte del nuevo slide
			isValidVoronoiNewVertex = false;			
			m=true;
      
      // Los puntos que hacen parte del nuevo slice
      // deben estar dentro del poligono
			for(j=0;j<allSlices.size() && m ;j++){
      	if(allSlices[j]!=NULL ){
          // Si esta dentro del poligono del slice
          // por ejemplo esta a la derecha de todos sus segmentos
          // pero no esta a la derecha de los segmentos de otro slice
					if((getPosition(allSlices[j],pointVoronoi->getData(0),pointVoronoi->getData(1))==-1)){
						isValidVoronoiNewVertex=true;
						for(k=0;k<allSlices.size();k++)
							if(k!=j)								
								if(getPosition(allSlices[k],pointVoronoi->getData(0),pointVoronoi->getData(1))==-1){
									isValidVoronoiNewVertex=false;
									m=false;								
									break;
								}    						                  
					}
				}
			}
     
			if(isValidVoronoiNewVertex){
				vectorPointVoronoi.push_back(pointVoronoi);				
			}
		}
	}	

  cout<<"Inserta los vertices adicionales "<<endl;

  IterSlice isn=new TSlice(vectorPointVoronoi.size());
  m_slices[0]->push_back(isn);
	for(ic=0;ic<vectorPointVoronoi.size();ic++){
    IterPoint pp=new TPoint();
    (*pp)[0]=vectorPointVoronoi[ic]->getData(0);
    (*pp)[1]=vectorPointVoronoi[ic]->getData(1);
    (*pp)[2]=(vs1+vs2)/2;

    isn->setPoint(pp,ic);
    IterVertSliceReconstruction ivsrup=(IterVertSliceReconstruction)isn->getPoint(ic);
    
    IterVertSliceReconstructionDown vsr=new TVertSliceReconstructionDown(vectorPointVoronoi[ic]);
    vsr->setSliceUp(NULL);

    vsr->m_pointUp=ivsrup;    

    if(ic%10==0)
      cout<<"Inserta el punto "<<ic<<endl;
    delDown.insertPoint(vsr);
	}

	// Ahora se encuentran los triangulos de la superficie
	// Son todos los triangulos de la dt que se conectan los puntos de voronoi
	// insertados y puntos en slices adyacentes
  resultDelDown.clear();
  delDown.getResult(resultDelDown);    
	
  cout<<"Empieza en los triangulos "<<resultDelDown.size()<<endl;
	// Solo para los triangulos que conectan puntos en slices distintos
	for(TDelaunayDown::TVectorResult::iterator id=resultDelDown.begin();id!=resultDelDown.end();id++){
    if(isConectDiferentSlicesVoronoi(*id)){
      IterSimplexDel sd=*id;
      IterVertSliceReconstructionDown vsr1=(IterVertSliceReconstructionDown)TDelaunayDown::getOriginalPoint(sd->getVert(1));
      IterVertSliceReconstructionDown vsr2=(IterVertSliceReconstructionDown)TDelaunayDown::getOriginalPoint(sd->getVert(2));
      IterVertSliceReconstructionDown vsr3=(IterVertSliceReconstructionDown)TDelaunayDown::getOriginalPoint(sd->getVert(3));

      // Inserta el triangulo a la reconstruccion
      m_triangles.push_back(vsr1->m_pointUp);
      m_triangles.push_back(vsr2->m_pointUp);
      m_triangles.push_back(vsr3->m_pointUp);	
		}
	}
}


template <class TAffineSpace>
bool SliceReconstruction<TAffineSpace>::isConectDiferentSlicesVoronoi(IterSimplexDel sd){
  typedef TVertSliceReconstructionDown::IterSlice IterSliceVor;
	IterVertSliceReconstructionDown ivsd1=(IterVertSliceReconstructionDown)TDelaunayDown::getOriginalPoint(sd->getVert(1));
  IterVertSliceReconstructionDown ivsd2=(IterVertSliceReconstructionDown)TDelaunayDown::getOriginalPoint(sd->getVert(2));
  IterVertSliceReconstructionDown ivsd3=(IterVertSliceReconstructionDown)TDelaunayDown::getOriginalPoint(sd->getVert(3));
  IterSlice s1=ivsd1->getSliceUp();
  IterSlice s2=ivsd2->getSliceUp();
  IterSlice s3=ivsd3->getSliceUp();

  if(s1==NULL || s2==NULL || s3==NULL){
    
  }

	if(s1==NULL){    
		if((s2!=NULL||s3!=NULL))
			return true;
	}

	if(s2==NULL){
		if((s3!=NULL||s1!=NULL))
			return true;
	}

	if(s3==NULL){
		if((s2!=NULL||s1!=NULL))
			return true;
	}
	return false;	
}

template <class TAffineSpace>
bool SliceReconstruction<TAffineSpace>::isConectDiferentSlices(IterSimplexDel sd){
  IterVertSliceReconstructionDown ivsd1=(IterVertSliceReconstructionDown)TDelaunayDown::getOriginalPoint(sd->getVert(1));
  IterVertSliceReconstructionDown ivsd2=(IterVertSliceReconstructionDown)TDelaunayDown::getOriginalPoint(sd->getVert(2));
  IterVertSliceReconstructionDown ivsd3=(IterVertSliceReconstructionDown)TDelaunayDown::getOriginalPoint(sd->getVert(3));
  IterSlice s1=ivsd1->getSliceUp();
  IterSlice s2=ivsd2->getSliceUp();
  IterSlice s3=ivsd3->getSliceUp();

	if(s1!=NULL && s2!=NULL && s3!=NULL){
		if(s2!=s3 && s2!=s1 && s1!=s3 )
			return true;
    
    if(s1->getLevel()!=s2->getLevel() || s1->getLevel()!=s3->getLevel() || s2->getLevel()!=s3->getLevel())
      return true;

    if(s1==s2 && s3!=s1)
      return true;
    
    if(s2==s3 && s3!=s1)
      return true;

    if(s1==s3 && s3!=s2)
      return true;

    else
      return false;
  }else{
    EXCEPTION("Alguno de los slices es NULO");    
  }
  return false;
}


template <class TAffineSpace>
SliceReconstruction<TAffineSpace>::TData SliceReconstruction<TAffineSpace>::cuadDist(IterPoint p1,IterPoint p2){
  unsigned int i=0;
  TData rdist=0;
  if(p1!=0 && p2!=0){
    for(i=0;i<TAffineSpace::dim;i++)
      rdist+=(p1->getData(i)-p2->getData(i))*(p1->getData(i)-p2->getData(i));
  }else{
    cout<<"Falla rara "<<endl;
    EXCEPTION("PUNTOS NULOS PARA LA DISTANCIA");
  }
  return rdist;

}

template <class TAffineSpace>
void SliceReconstruction<TAffineSpace>::findCircle(TData &cx,TData &cy,TData x1,TData y1,TData x2,TData y2,TData x3,TData y3){
	TData a=(y2-y3)*(x2-x1)-(y2-y1)*(x2-x3);
	TData a1=(x1+x2)*(x2-x1)+(y2-y1)*(y1+y2);
	TData a2=(x2+x3)*(x2-x3)+(y2-y3)*(y2+y3);
	cx=(a1*(y2-y3)-a2*(y2-y1))/a/2;
	cy=(a2*(x2-x1)-a1*(x2-x3))/a/2;
}


/**
  retorna -1 si el punto esta dentro del poligono
	retorna 0 en otro caso
*/
template <class TAffineSpace> 
int SliceReconstruction<TAffineSpace>::getPosition(IterSlice slice,TData cx,TData cy){
	IterPoint p1,p2;
	int v=-1;
	//for(unsigned int i=0;i<=slice->getSize();i++){
  for(unsigned int i=slice->getSize();i>0;i--){
		p1 = slice->getPoint(i);
		p2 = slice->getPoint(i-1);    

		v = orientation((*p1)[0],(*p1)[1],(*p2)[0],(*p2)[1],cx,cy);    
		if(v==1){
			break;
		}
	}  	

	return v;
}

template <class TAffineSpace>
int SliceReconstruction<TAffineSpace>::orientation(TData p1x,TData p1y,TData p2x,TData p2y,TData cx,TData cy){
		TData v=(p1x*(p2y-cy) - p1y*(p2x-cx) + p2x*cy - cx*p2y);
		if( v >0 )
			return 1;
		else if(v==0)
			return 0;
		return -1;
}

template <class TAffineSpace>
void SliceReconstruction<TAffineSpace>::getResult(vector<IterPoint>& vect,unsigned int** simplicial,unsigned int& size){
  typedef SliceReconstruction<TAffineSpace> TSliceReconstruction;
  typedef TSliceReconstruction::IterListSlice IterListSlice;
  typedef TSliceReconstruction::TListSlice TListSlice;
  typedef TSliceReconstruction::IterSlice IterSlice;
  typedef TSliceReconstruction::IterVertSliceReconstruction IterVertSliceReconstruction;
    
  unsigned int i=0,j=0,k=0,cont=1;
  unsigned int sizePoints=0;

  IterPoint p=0;

  for(i=0;i<getSizeLevel();i++){
    IterListSlice isl=getLevel(i);  
    for(TListSlice::iterator is=isl->begin();is!=isl->end();is++){
      IterSlice sv=*is;
      sizePoints+=sv->getSize();
    }
  }
  
  for(i=0;i<getSizeLevel();i++){
    IterListSlice isl=getLevel(i);  
    for(TListSlice::iterator is=isl->begin();is!=isl->end();is++){            
      IterSlice sv=*is;
      for(j=0;j<sv->getSize();j++){
        ((IterVertSliceReconstruction)sv->getPoint(j))->setId((cont++));
        p=new TPoint();
        for(k=0;k<TAffineSpace::dim;k++){          
          (*p)[k]=sv->getPoint(j)->getData(k);
        }
        vect.push_back(p);        
      }
    }
  }

  (*simplicial)=new unsigned int[m_triangles.size()*3];
 
  j=0;
  for(i=0;i<m_triangles.size();i=i+3){
    (*simplicial)[j++]=((IterVertSliceReconstruction)m_triangles[i])->getId();
    (*simplicial)[j++]=((IterVertSliceReconstruction)m_triangles[i+1])->getId();
    (*simplicial)[j++]=((IterVertSliceReconstruction)m_triangles[i+2])->getId();    
  }
  size=m_triangles.size()/3;
}


template <class TAffineSpace>
ostream& operator <<(ostream& os, SliceReconstruction<TAffineSpace>& sr){  
  typedef SliceReconstruction<TAffineSpace> TSliceReconstruction;
  typedef TSliceReconstruction::IterListSlice IterListSlice;
  typedef TSliceReconstruction::TListSlice TListSlice;
  typedef TSliceReconstruction::IterSlice IterSlice;
  typedef TSliceReconstruction::IterVertSliceReconstruction IterVertSliceReconstruction;
    
  unsigned int i=0,j=0,k=0,cont=1;
  unsigned int sizePoints=0;
  for(i=0;i<sr.getSizeLevel();i++){
    IterListSlice isl=sr.getLevel(i);  
    for(TListSlice::iterator is=isl->begin();is!=isl->end();is++){            
      IterSlice sv=*is;
      sizePoints+=sv->getSize();
    }
  }

  os<<sizePoints<<" "<<(sr.m_triangles.size()/3)<<endl;

  
  for(i=0;i<sr.getSizeLevel();i++){
    IterListSlice isl=sr.getLevel(i);  
    for(TListSlice::iterator is=isl->begin();is!=isl->end();is++){            
      IterSlice sv=*is;
      for(j=0;j<sv->getSize();j++){
        ((IterVertSliceReconstruction)sv->getPoint(j))->setId((cont++));
        for(k=0;k<TAffineSpace::dim;k++){
          os<<sv->getPoint(j)->getData(k)<<" ";          
        }
        os<<endl;        
      }
    }
  }  

  for(i=0;i<sr.m_triangles.size();i=i+3){
    os<<((IterVertSliceReconstruction)sr.m_triangles[i])->getId()<<" ";
    os<<((IterVertSliceReconstruction)sr.m_triangles[i+1])->getId()<<" ";
    os<<((IterVertSliceReconstruction)sr.m_triangles[i+2])->getId()<<endl;
  }
  return os;
}


CGL_END

#endif SLICERECONSTRUCTION_H