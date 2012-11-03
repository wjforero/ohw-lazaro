#ifndef MESHMDD_H
#define MESHMDD_H

#include "kernel/affinespace.h"
using namespace ohw::cgl;

/** Modela una malla d-dimensional
*/
template <class TAffineSpace>
class MeshMdd {
public:
  typedef TAffineSpace::TAffinePoint* IterPoint;
public:    
  MeshMdd():m_sizePoints(-1), m_size(-1), m_index(0), m_vectPoints(0){
  }

  MeshMdd(vector<IterPoint>* vectPoints,unsigned int* vertices=0,const unsigned int& sizeMesh=0){
    m_vertices=vertices;
    m_vectPoints=vectPoints;
    m_sizeMesh=sizeMesh;		
  }
  
  unsigned int* getVertices() const{
    return m_vertices;
  }

  unsigned int getSizeMesh() const{
    return m_sizeMesh;
  }

  unsigned int getSizePoints() const{
    return m_vectPoints->size();
  }

  vector<IterPoint>* getVectorPoints() const{
    return m_vectPoints;
  }

	void setTypeMesh(const unsigned int& typeMesh){
		m_typeMesh=typeMesh;
	}

	unsigned int getTypeMesh(){
		return m_typeMesh;
	}

private:
  unsigned int* m_vertices;  
  unsigned int m_sizeMesh;
  vector<IterPoint>* m_vectPoints;
	unsigned int m_typeMesh;	 
};

#endif