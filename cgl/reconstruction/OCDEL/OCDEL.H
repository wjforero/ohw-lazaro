#ifndef OCDEL_H
#define OCDEL_H

#include "base.h"
#include "enum.h"
#include "algebra.h"
#include "hoppevertex.h"
#include "volumetricspace.h"
#include "structures/graph.h"

CGL_BEGIN

/** Implementación del ocdel
*/
template<class TAffineSpace>
class Ocdel {
public:
  // Tipos de datos   
  /** Tipo de dato escalar
   */
  typedef typename TAffineSpace::TData TData;
  /** Marcos de referencia (planos tangentes)
   */
  typedef typename TAffineSpace::TAffineFrame TFrame;
  /** Matriz
   */
  typedef typename TAffineSpace::TMatrix TMatrix;
  /** Puntos de entrada/salida
   */
  typedef typename TAffineSpace::TAffinePoint TPoint;
  /** Vectores para los calculos
   */
  typedef typename TAffineSpace::TAffineVector TVector;  
  /** Estructura de datos espacial
   */
  typedef typename Octree<TAffineSpace,TPoint*> TOctree;
  
	/** Esta clase localiza
	*/
	template <class T>
  class Locator : public Visitor<T> {    
  public:
    TPoint* m_point;
    bool isDone(){
      return true;
    }
    bool visit(const T& data) {
      if(data==0)
        return false;
      TPoint& point = *(m_point);
      TPoint& pos = data->getPosition();
      TVector& size = data->getSize();
      //cout<<*data<<endl;
      for( unsigned int i=0; i<TMySpace::dim; i++) {
        //cout<<"compara la coordenada "<<i<<endl;
        if( point[i] < pos[i] || point[i] > pos[i]+size[i] )
          return false;
      }
      return true;
    }
  };
public:
	/**
	*/
	Ocdel(){
		m_delaunay=0;
	}

	/** Carga una triangulación a un grafo
	*/
	void load(vector<IterPoint>& vect,unsigned int** simplicial,unsigned int& size){
		unsigned int i=0;
		if(m_delaunay!=0)
      delete m_delaunay;
		m_delaunay = new TGraph;
		TGraph& delaunay = *m_delaunay;
		
		for(i=0;i<vectPoints.size();i++){
			TGraph::TNode& gnode = delaunay.newNode();
			delaunay.setData(//***//);
		}
		for(i=0;i<size;i++){			
			delaunay.addEdge(riemannian.getNode(simplicial[i*3]-1),riemannian.getNode(simplicial[i*3+1]-1));
			delaunay.addEdge(riemannian.getNode(simplicial[i*3]-1),riemannian.getNode(simplicial[i*3+2]-1));
			delaunay.addEdge(riemannian.getNode(simplicial[i*3+1]-1),riemannian.getNode(simplicial[i*3+2]-1));
		}		
	}

	/** Elimina triangulos que no cumplen con el criterio
	*/
	void OctDel::eliminateTriangles(){
		long int i;
		// vector con los triangulos
		vector<Triangle3d<float>*> vectTriangles;
		for (i=0; i<m_triangles.size(); i++){
			// Revisa que al menos una de las dos esferas que rodean al triangulo son vacias
			if(!revisarEsfera(m_triangles[i])){
				// Borra el triangulo de la lista
				delete m_triangles[i];
			}
			else{
				// Si la revisión de la esfera es verdadera cumple con el criterio y se inserta
				vectTriangles.push_back(m_triangles[i]);				
			}
		}		
		m_triangles=cosa;
	}

	/** Revisa la esfera para un triangulo
	*/
	bool OctDel::revisarEsfera(Triangle3d<float>* v){
		// Tomar el centro del triangulo y buscarlo en el octree, 
		// luego mirar sin en la esfera hay otros puntos
		// Incialmente crea los dos centros circunscritos al triangulo
		TPoint* center1 = new TPoint(0,0,0);
		TPoint* center2 = new TPoint(0,0,0);

		// Obtiene los centros de la esfera
		bool flag = getCenterSphere(center1,center2,m_radio,&(*v)[0],&(*v)[1],&(*v)[2]);
		if(flag) {
			// Obtiene el nodo que contiene el centro1
			Locator<TOctree*> miLocator;
			miLocator.m_point = center1;
			m_octree->locateData(center1, miLocator);
			TOctree* nodo1 = miLocator.getData(); //m_octree->find3dBlock(center1);
			// Obtiene el nodo que contiene el centro2
			miLocator.m_point = center2;
			m_octree->locateData(center2, miLocator);
			TOctree* nodo2 = miLocator.getData(); //m_octree->find3dBlock(center2);
			// Realiza una busqueda bajando hasta estos niveles
			int n1 = (int)m_radio/nodo1->getSize()[0]; //getCube().getLength();
			int n2 = (int)m_radio/nodo2->getSize()[0]; //getCube().getLength();
			// Corre dos funciones recursivas que realizan la busqueda en la estructura de datos
			// Si por lo menos una de las bolas no contiene puntos el triangulo pertenece 
			// a la superficie
			return (funcionRecursiva(nodo1,v,n1) || funcionRecursiva(nodo2,v,n2));		
		}
		else {
			return false;
		}
	}

	/** Retorna true si no encuentra puntos vecinos
	*/
	bool OctDel::funcionRecursiva(TOctree* node, Triangle3d<float>* t,int n){
		// Si el nodo no contiene puntos y recorre n pasos retorna true
		if(n==0){
			if(node->getData()==0){
				return true;
		}else{
				// Si el contiene puntos que contiene el nodo es mayor a 3, es decir contiene más puntos que los que define el triangulo
				// Pregunta si el nodo en el octree contiene más puntos a parte de los del triangulo
				int contvert=(int)(node->hasPoint((Vertex3d<float>*)&(*t)[0]))+(int)node->hasPoint((Vertex3d<float>*)&(*t)[1])+(int)node->hasPoint((Vertex3d<float>*)&(*t)[2]);
				if(node->getSizePoints()>contvert){
					// Si es asi retorna true
					return false;
				}else{
					// Si no retorna false
					return true;
				}
			}
		}		

		// Realiza la busqueda recursiva un conjunto de pasos n
		for(int i=0;i<6;i++){
			OctreeNode* temp=node->locateEqFaceNeighbor((oFACE)i);
			// Llamada a la función de busqueda
			if(temp!=0 && funcionRecursiva(temp,t,n--))
				return true;
		}
		return false;
	}


	/** Retorna los centros de las dos esferas
	*/
	bool OctDel::getCenterSphere(Vertex3d<float>* & center1,Vertex3d<float>* & center2,float r,Point3d<float>* p1,Point3d<float>* p2,Point3d<float>* p3){
		vector<TPoint>* vp=new vector<TPoint>();
		TPoint p1t(3);
		p1t[0]=p1->getx();
		p1t[1]=p1->gety();
		p1t[2]=p1->getz();	
		TPoint p2t(3);
		p2t[0]=p2->getx();
		p2t[1]=p2->gety();
		p2t[2]=p2->getz();	

		TPoint p3t(3);
		p3t[0]=p3->getx();
		p3t[1]=p3->gety();
		p3t[2]=p3->getz();	

		vp->push_back(&p1t);
		vp->push_back(&p2t);
		vp->push_back(&p3t);	

		SphereDim sd(*vp);	
		Vector<float>* vcenter=sd.getCenter();
		if(sd.getCuadRadius()>r*r)
			return false;
		
		float rpeque=sqrt((r*r-sd.getCuadRadius()));
	
		Hiperplane* h=new Hiperplane();
		Point porig(3);
		porig[0]=p2->getx();
		porig[1]=p2->gety();
		porig[2]=0;	
		h->setParam(*vp,porig,3);	
		float nx=h->getValuePlane(0);
		float ny=h->getValuePlane(1);
		float nz=h->getValuePlane(2);
		float norm=sqrt(nx*nx+ny*ny+nz*nz);
		nx=(nx*rpeque)/norm;
		ny=(ny*rpeque)/norm;
		nz=(nz*rpeque)/norm;

		center1->setx((*vcenter)[0]+nx);
		center1->sety((*vcenter)[1]+ny);
		center1->setz((*vcenter)[2]+nz);
		center2->setx((*vcenter)[0]-nx);
		center2->sety((*vcenter)[1]-ny);
		center2->setz((*vcenter)[2]-nz);
		delete h;
		return true;
	}

private:
	TGraph* m_delaunay;
};


CGL_END

#endif