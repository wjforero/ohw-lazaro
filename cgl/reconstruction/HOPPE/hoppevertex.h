/**
 * $Revision: 1.2 $
 * $Date: 2003/03/18 12:36:05 $
 */

#ifndef HOPPEVERTEX_H
#define HOPPEVERTEX_H

#include "base.h"
#include "graph.h"

using namespace ohw::structures;

CGL_BEGIN

template<class TVolumetricSpace>
class HoppeVertex {
public:
  /**
   */
  typedef HoppeVertex<TVolumetricSpace> TSelf;
	/**
	 */
	typedef typename TVolumetricSpace::TAffineSpace::TData TData;
	/**
	 */
	typedef typename TVolumetricSpace::TAffineSpace::TAffinePoint TPoint;
	/**
	 */
	typedef typename TVolumetricSpace::TAffineSpace::TAffineVector TVector;
	/**
	 */
	typedef typename TVolumetricSpace::TAffineSpace::TPredicates TPredicates;
	/**
	 */
	typedef typename TVolumetricSpace::TSpatialStructure TSpatialStructure;
  /** Grafo
   */
  typedef Graph<GraphNode,GraphEdge,TSelf*> TGraph;
public:
	/**
	 */
	HoppeVertex() {
		m_point = m_node = m_normal = 0;
    m_inv = false;
	};
	/**
	 */
	HoppeVertex(TPoint& point) {
    m_point = &point;
    m_node = 0;
		m_normal = 0;
    m_inv = false;
	};
	/**
	 */
	~HoppeVertex() {
		if(m_normal!=0)
			delete m_normal;
	};
public:
	/**
	 */
	TPoint& getPoint()  {
    if(m_point==0) cout<<"error";
		return *m_point;
	}
	/**
	 */
	TSpatialStructure& getNode() const {
		return *m_node;
	}
	/**
	 */
	void setNode(TSpatialStructure& node) {
		m_node = &node;
	}
	/**
	 */
  TGraph::TNode& getGraphNode() const {
		return *m_graphnode;
	}
	/**
	 */
  void setGraphNode(TGraph::TNode& node) {
		m_graphnode = &node;
	}
	/**
	 */
	TVector& getNormal() {
    //cout<<"se pide la normal"<<endl;
    return *m_normal;
	}

  void invNormal() {
    //if(!m_inv) 
    {
      *m_normal=-*m_normal;
      m_inv = true;
    /*} else {
      cout<<"con que tratando de invertirla no!!!"<<endl;*/
    }
	}
	/**
	 */
	void setNormal(TVector& normal) {
		if (m_normal!=0)
			delete m_normal;
		m_normal = &normal;
	}
public:
	/** Inserción de un vertice en un arbol
	 */
	static unsigned char locate(HoppeVertex<TVolumetricSpace>* pVertex, TSpatialStructure* pNode){
		unsigned char code = 0x00;
		const TVector& size = pNode->getSize();
		const TPoint& position = pNode->getPosition();
		for (unsigned int i=0; i<size.getDim(); i++){
			TData length = size.getHomogeneous(i)/2;
			if(pVertex->getPoint().getCartesian(i) >= position.getCartesian(i)+length)
				code = code | 0x01<<i;
		}
		return code;
	}
	/** verifica que los dos puntos sean vecinos
	 */
	static bool isNeighbor(HoppeVertex<TVolumetricSpace>* vertex1, HoppeVertex<TVolumetricSpace>* vertex2) {
    if (vertex1==0 || vertex2 ==0) {
      cout<<"como se le ocurre que apuntadores nulos puedan ser vecinos!!!"<<endl;
			return false;
    }
		TPoint& p1 = (vertex1->getPoint());
		TPoint& p2 = (vertex2->getPoint());
		TData dist = TPredicates::squaredDistance(p1,p2);
    TData ff=(p1[0]-p2[0])*(p1[0]-p2[0])+(p1[1]-p2[1])*(p1[1]-p2[1])+(p1[2]-p2[2])*(p1[2]-p2[2]);

    if (sqrt(ff)<=(m_maxDist)) {
			return true;
		}
		return false;
	}
private:
	/** 
	 */
	TPoint* m_point;
	/** 
	 */
	TSpatialStructure* m_node;
  /**
   */
  TGraph::TNode* m_graphnode;
	/**
   */
  TVector* m_normal;
  /**
   */
  bool m_inv;
public:
	/**
	 */
	static TData m_maxDist;
  
};

template <class TVolumetricSpace>
HoppeVertex<TVolumetricSpace>::TData HoppeVertex<TVolumetricSpace>::m_maxDist = 0.1f;

/** 
 */	
template <class TVolumetricSpace>
ostream& operator<<(ostream& os, HoppeVertex<TVolumetricSpace>& vertex){    
  os<<"v "<<vertex.getPoint();
  return os;
}

CGL_END

#endif