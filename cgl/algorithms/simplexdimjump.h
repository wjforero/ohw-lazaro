#ifndef SIMPLEXDIMJUMP_H
#define SIMPLEXDIMJUMP_H

// Inclusiones especificas
#include "base.h"
#include "../kernel/affinespace.h"

CGL_BEGIN

template <class TAffineSpace>
class Simplex;

template <class TAffineSpace>
class ConvexHull;


/** Representa un simplex en un cambio de dimensión, se utiliza cuando se construye la envolvente convexa y se
*   se genera un punto que no esta en el affine hull
*/
template <class TConvexHull>
class SimplexDimJump{
public:
  typedef typename TConvexHull::TAffineSpace TAffineSpace;
  typedef typename Simplex<TAffineSpace> TSimplex;
  typedef typename Simplex<TAffineSpace>* IterSimplex;
  typedef typename PointAffine<TAffineSpace>* IterPoint;
  typedef typename TConvexHull* IterConvexHull;
    

public:
  SimplexDimJump(IterSimplex father,IterPoint p,const unsigned int& cDim,IterPoint anti,IterConvexHull m_ch);  
  void setNeighbour1();
  void setNeighbour2();
  IterSimplex getSonUnbounded();
  IterSimplex getFather();


private:
  IterSimplex m_father;
  IterSimplex m_son;
  IterSimplex newSonUnbounded();
  void setNeighbour(IterSimplex S1, const unsigned int& v1,IterSimplex S2,const unsigned int& v2);
  void insertPoint();
  IterPoint m_newPoint;
  IterPoint m_anti;
  char m_cDim; 
  IterConvexHull m_ch;
};

template <class TConvexHull>
SimplexDimJump<TConvexHull>::SimplexDimJump(IterSimplex father,IterPoint p,const unsigned int& cDim,IterPoint anti,IterConvexHull m_ch){
	m_newPoint = p;
	m_cDim = cDim;
	m_anti = anti;
	m_father = father;
  m_ch=m_ch;
	insertPoint();
}


template <class TConvexHull>
void SimplexDimJump<TConvexHull>::setNeighbour(IterSimplex S1, const unsigned int& v1,IterSimplex S2,const unsigned int& v2){
  if(S1!=0 && S2!=0){
	  //S2 es opuesto al vertice v1
	  S1->setOpposite(S2,v1,v2);
	  //S1 es opuesto al vertice v2
	  S2->setOpposite(S1,v2,v1);
  }else{
    EXCEPTION("Falla SimplexDimJump<TConvexHull>::setNeighbour ap nulo");
  }
}

template <class TConvexHull>
void SimplexDimJump<TConvexHull>::insertPoint(){
	m_father->setVert(m_newPoint,m_cDim);
	if(m_ch->isBounded(m_father)){
		m_son=newSonUnbounded();
		setNeighbour(m_father,m_cDim,m_son,0);
	}
}

template <class TConvexHull>
void SimplexDimJump<TConvexHull>::setNeighbour2(){
	unsigned int i=0;
  if(m_ch->isBounded(m_father)){
		//En la nueva triangulación representa un nuevo unbounded simplex
		//Con base facet el mismo y peak el antiorigen
		for(i=0;i<SimplexDimJump::m_cDim-1;i++){
			IterSimplex fp=m_father->getOpposite(i);
    	if(m_ch->isBounded(fp))
				setNeighbour(m_son,i+1,(fp->getOpposite(m_cDim)),m_father->indexOpposite(i)+1);
			else
				setNeighbour(m_son,i+1,fp,m_cDim);
		}
	}
}

template <class TConvexHull>
void SimplexDimJump<TConvexHull>::setNeighbour1(){
	//Es unbounded	
	if(!m_ch->isBounded(m_father)){
		//fp es el vecino opuesto al antiorigen en la triangulacion vieja
		IterSimplex fp=m_father->getOpposite(0);
		//fp es bounded
		//El nuevo vertice es vecino del nuevo hijo bounded de fp es decir fp
		setNeighbour(m_father,m_cDim,(fp->getOpposite(SimplexDimJump::m_cDim)),m_father->indexOpposite(0)+1);
	}
}

template <class TConvexHull>
SimplexDimJump<TConvexHull>::IterSimplex SimplexDimJump<TConvexHull>::newSonUnbounded(){
	unsigned  int i=0;
	IterSimplex son = new TSimplex();
	son->setVert(m_anti,0);
	for(i=0 ; i<SimplexDimJump::m_cDim; i++)
		son->setVert(m_father->getVert(i), i+1);
	return son;
}

template <class TConvexHull>
SimplexDimJump<TConvexHull>::IterSimplex SimplexDimJump<TConvexHull>::getSonUnbounded(){
	return m_son;
}

template <class TConvexHull>
SimplexDimJump<TConvexHull>::IterSimplex SimplexDimJump<TConvexHull>::getFather(){
	return m_father;
}



CGL_END

#endif