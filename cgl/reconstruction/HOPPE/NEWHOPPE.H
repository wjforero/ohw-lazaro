/**
 * $Revision$
 * $Date$
 */

#ifndef NHOPPE_H
#define NHOPPE_H

#include "base.h"
#include "enum.h"
#include "algebra.h"
#include "hoppevertex.h"
#include "volumetricspace.h"
#include "structures/graph.h"


CGL_BEGIN

/** Metodo de reconstrucción de Hugues Hoppe et'al
 */
template<class TAffineSpace>
class NHoppe {
public:
  // Tipos de datos
  /**
   */
  typedef NHoppe<TAffineSpace> TSelf;
  /**
   */
  typedef TAffineSpace TSpace;
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
  /** Vectores para los calculos
   */
  typedef typename TAffineSpace::TPredicates TPredicates;
  /**
   */
  //typedef typename TVolumetricSpace::TMarchingCells TMarchingCells;
  typedef MarchingCubes<TAffineSpace> TMarchingCells;
  /** Estructura de datos espacial
   */
  //typedef typename TVolumetricSpace::TSpatialStructure TSpatialTree;
  typedef Octree<TAffineSpace,TPoint*> TSpatialTree;
  /** Vertices auxiliares para las busquedas
   */
  //typedef typename TVolumetricSpace::TVertex TVertex;
  /** Grafo
   */
  typedef Graph<GraphNode,GraphEdge,TPoint*> TGraph;
  /**
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
      for( unsigned int i=0; i<TAffineSpace::dim; i++ ) {
        if( point[i] < pos[i] || point[i] > pos[i]+size[i] )
          return false;
      }
      return true;
    }
  };
  /**
   */
  template <class T>
  class FindNeighbour : public Visitor<T> {
  public:
    TPoint* m_point;
    TData m_sampling;
    bool isDone(){
      return true;
    }
    /** Retorna true cuando se alcanza un punto fuera de la frontera establecida
     */
    bool visit(const T& data) {
      if(data==0)
        return false;
      TPoint& point = *(m_point);
      TPoint& p2 = data->getData(0)->getPoint();
      if(TAffineSpace::TPredicates::squaredDistance(point,p2) <= m_sampling)
        return false;
      return true;
    }
  };
public:
  // Constructores
  /** Constructor por omision
   */
  NHoppe() {
    this->m_mst = 0;
    this->m_riemannian = 0;
    this->m_sampling = 1;
    this->m_spaceTree = 0;
  }
  /** Construye un objeto de tipo Hoppe con el parametro especificado
   */
  NHoppe(TData sampling) : m_sampling(sampling){
    this->m_mst = 0;
    this->m_riemannian = 0;
    this->m_spaceTree = 0;
    //TVertex::m_maxDist = sampling;
  }
public:
  // Operadores
public:

	/** Retorna el centroide
   */
	TPoint& getCentroid(const unsigned int& i){
		return *m_centroids[i];
	}
	/** Retorna el centroide
   */
	TVector& getNormal(const unsigned int& i){
		return *m_normals[i];
	}

  // Metodos de acceso
  /** Retorna el numero de puntos de entrada actual
   */
  unsigned int getInputDataSize() const {
    return m_inputData.size();
  }
  /** Retorna el i-esimo punto de entrada
   */
  TPoint& getInputData(const unsigned int& i) const {
    return *m_inputData[i];
  }
  /** Adiciona un nuevo punto de entrada al algoritmo
   */
  void addInputData(TPoint& data) {
    m_inputData.push_back(&data);
    //m_tangentPlanes.push_back(0);
  }
  /** Retorna el numero de puntos de salida actual
   */
  unsigned int getOutputDataSize() const {
    return m_outputData.size();
  }
  /** Retorna el i-esimo punto de entrada
   */
  TPoint& getOutputPoint(const unsigned int& i) const {
    return *m_outputData[i];
  }
  /** Retorna el plano tangente asociado al i-esimo punto de entrada
   */
  TFrame& getTangentPlane(const unsigned int& i) const {
    return *m_tangentPlanes[i];
  }
	/** Retorna el centroide
   */
	/*TPoint& getCentroid(const unsigned int& i){
		return *m_centroids[i];
	}
	/** Retorna el centroide
   */
	/*TVector& getNormal(const unsigned int& i){
		return *m_normals[i];
	}
  /** Asigna un plano tangente para un punto de entrada especifico
   */
  void setTangentPlane(const unsigned int& i, TFrame& tp) {
    m_tangentPlanes[i] = &tp;
  }
  /** Retorna el plano tangente asociado al i-esimo punto de entrada
   */
  /*TSpatialTree& getCell(const unsigned int& i) const {
    return *m_cells[i];
  }*/
  /** Retorna el parametro de reconstrucción (densidad de muestreo)
   */
  TData getSampling() const {
    return m_sampling;
  }
  /**
   */
  void setSampling(TData& sampling) {
    m_sampling = sampling;
  }
public:
  // Pasos de la reconstrucción
  /** Estimación de los planos tangentes
   */
  void calculateTangentPlanes() {
#if OHWDEBUG 
    cout<<"estimando los planos tangentes"<<endl; 
#endif

    // Para cada uno de los puntos de entrada
    for( unsigned int i=0; i<getInputDataSize(); i++ ) {

      //cout<<"calculando para el vertice "<<i<<endl;

      // Se calcula la vecindad
      vector<TPoint*> nghb;
      for( unsigned int j=0; j<getInputDataSize(); j++ ) {
        if( sqrt(TPredicates::squaredDistance( getInputData(i), getInputData(j) ) ) < m_sampling )
          nghb.push_back(m_inputData[j]);
      }
      //cout<<"vecinos"<<nghb.size()<<endl;

      // Se calcula el centroide
      TPoint& centroid = *new TPoint();
      for( unsigned int j=0; j<nghb.size();  j++ ) {
        TPoint& ng = *nghb[j];
        for( unsigned int _axis=0; _axis < TSpace::dim; _axis++ ) {
          centroid[_axis] += ng[_axis];
        }
      }
      for( unsigned int _axis=0; _axis < TSpace::dim; _axis++ ) {
        centroid[_axis] = centroid[_axis]/nghb.size();
      }
      //cout<<"centroid "<<centroid<<endl;

      // Se calcula la matriz de covarianza
      TMatrix cv(TAffineSpace::dim+1);
			TMatrix cvi(TAffineSpace::dim+1);
			TVector dif;
			cv.setIdentity();
			for(j=0; j<TAffineSpace::dim+1; j++)
				cv[j][j]=0.0;
      for (j=0; j<nghb.size(); j++) {
				TPoint &nb = *nghb[j];
				dif = nb-centroid;
				TVector::outerProduct(dif,dif,cvi);
				cv = cv+cvi;
			}

			// Calcula los vectores propios
			int nrot;
			TMatrix evectors(TAffineSpace::dim);
			evectors.setIdentity();
			Algebra<TData>::TVector evalues(TAffineSpace::dim);
			Algebra<TData>::jacobi(cv,evalues,evectors,nrot,TAffineSpace::dim);
			Algebra<TData>::eigsrt(evalues,evectors,TAffineSpace::dim);

      TVector& normal = *new TVector();
      for( unsigned int _axis=0; _axis < TSpace::dim; _axis++ ) {
        normal[_axis] = evectors[_axis][TSpace::dim-1];
      }
      //cout<<"normal "<<normal<<endl;

      m_centroids.push_back(&centroid);
      m_normals.push_back(&normal);
    }

    ofstream normales;
    normales.open("salinormales.m3d");
    normales<<(getInputDataSize()*3)<<" "<<getInputDataSize()<<endl;
    for( unsigned int i=0; i<getInputDataSize(); i++) {
      TPoint& centroid = *m_centroids[i];
      normales<<centroid[0]<<" "<<centroid[1]<<" "<<centroid[2]<<" "<<endl;
      TVector& normal = *m_normals[i];
      normales<< ( centroid[0] + ( normal[0] * m_sampling ) ) << " " ;
      normales<< ( centroid[1] + ( normal[1] * m_sampling ) ) << " " ;
      normales<< ( centroid[2] + ( normal[2] * m_sampling ) ) << endl ;
      normales<<centroid[0]<<" "<<centroid[1]<<" "<<centroid[2]<<" "<<endl;
    }
    for(unsigned int i=0;  i<getInputDataSize()*3; i++) {
      normales<<i+1<<" ";
      if( i%3==0 ) normales<<endl;
    }
    normales.close();
    
  }
  /** Orientación consistente de los planos tangentes
   */
  void orientTangentPlanes() {
#if OHWDEBUG 
    cout<<"orientando los planos tangentes"<<endl; 
#endif

    // se construye el grafo de riemannian
    if(m_riemannian!=0)
      delete m_riemannian;
    m_riemannian = new TGraph;
    TGraph& riemannian = *m_riemannian;

     cout<< "Se insertan los puntos como nodos del grafo de riemannian" <<endl;
    vector<bool> visited;
    unsigned int root=0; TData width = 0;
    for(unsigned int i=0; i<m_centroids.size(); i++) { 
      visited.push_back(false);
      TPoint& centroid = *m_centroids[i];
      TGraph::TNode& gnode = riemannian.newNode();
      gnode.setData(&centroid);
      if(centroid[TAffineSpace::dim-1]>width) {
        root = i;
        width = centroid[TAffineSpace::dim-1];
      }
    }


    for(unsigned int i=0; i<m_centroids.size(); i++) { 
      TPoint& centroid = *m_centroids[i];
      TVector& ni = *m_normals[i];
      // Se buscan los vecinos a este vertice
      // Se calcula la vecindad
      vector<unsigned int> nghb;
      for( unsigned int j=i+1; j<m_centroids.size(); j++ ) {
        TPoint& p = *m_centroids[j];
        if( sqrt(TPredicates::squaredDistance( centroid, p ) ) < m_sampling )
          nghb.push_back(j);
      }
		  for( unsigned int j=0; j<nghb.size(); j++ ){
        TPoint& neighbor = *m_centroids[nghb[j]];
        unsigned int k = nghb[j];
        TData distance = (TData)sqrt(TPredicates::squaredDistance( centroid, neighbor ));
        TGraph::TEdge& edge1 = riemannian.addEdge(riemannian.getNode(i),riemannian.getNode(k));
        TGraph::TEdge& edge2 = riemannian.getEdge(riemannian.getNode(k),riemannian.getNode(i));
        TVector& nk = *m_normals[k];
        TVector::innerProduct(ni,nk,distance);
        edge1.setWeight(1.0f-abs(distance));
        edge2.setWeight(1.0f-abs(distance));
        
		  }
    }

    // Se calcula el MST
    m_mst = &TGraph::primsAlgorithm(riemannian,root);
    recursiveOrient(root,visited);

    ofstream normales;
    normales.open("salinormalesorient.m3d");
    normales<<(getInputDataSize()*3)<<" "<<getInputDataSize()<<endl;
    for( unsigned int i=0; i<getInputDataSize(); i++) {
      TPoint& centroid = *m_centroids[i];
      normales<<centroid[0]<<" "<<centroid[1]<<" "<<centroid[2]<<" "<<endl;
      TVector& normal = *m_normals[i];
      normales<< ( centroid[0] + ( normal[0] * m_sampling ) ) << " " ;
      normales<< ( centroid[1] + ( normal[1] * m_sampling ) ) << " " ;
      normales<< ( centroid[2] + ( normal[2] * m_sampling ) ) << endl ;
      normales<<centroid[0]<<" "<<centroid[1]<<" "<<centroid[2]<<" "<<endl;
    }
    for(unsigned int i=0;  i<getInputDataSize()*3; i++) {
      normales<<i+1<<" ";
      if( i%3==0 ) normales<<endl;
    }
    normales.close();

  }
  /** Método que orienta recursivamente las normales
   */
  void recursiveOrient(unsigned int node, vector<bool>& visited) {
    visited [node] = true;
    TVector& ni = *m_normals[node];
    list<TGraph::TEdge*>& edges = m_mst->getEdges(node);
    for(list<TGraph::TEdge*>::iterator p=edges.begin(); p!=edges.end(); p++){
      TData val;
      TGraph::TEdge& edge = *(*p);
      TGraph::TNode& from = m_mst->getNode(node);
      TGraph::TNode& to = edge.mate(from);
      TVector& nj = *m_normals[to.getId()];
      TVector::innerProduct(ni,nj,val);
      if(val>(-0.2) && val<0.1)
        cout<<"Producto punto: "<<val<<endl;
      if( node!=to.getId() && val < 0 ) {
        nj = -nj;
      }
      if( !visited[to.getId()] )
        recursiveOrient(to.getId(),visited);
    }
  }
  /** Función de distancia signada
   */
  bool signedDistanceFunction(const TPoint& p, OrientedSide& side) const {//, TSpatialTree* container) const {
#if OHWDEBUG 
    //cout<<"calculando la función de distancia signada"<<endl; 
#endif
		
    // Se busca el plano cuyo centroide sea mas cercano
		TData dist=1.0f;
		TPoint* pos=0;
    unsigned int posi = 0;
		for(unsigned int i=0; i<m_centroids.size(); i++){
      TPoint* vertex1 = m_centroids[i];
      TData dist1 = sqrt(TPredicates::squaredDistance( p, *vertex1 ));
      if(dist1 < dist) {
				pos=vertex1;
        posi = i;
        dist = dist1;
      }
		}
    // Se calcula la distancia signada
		if ( pos == 0  ) {
			return false;
		}
		TVector& ni = *m_normals[posi];
		TPoint& oi = *pos;
		TVector dif; dif = (p-oi);
    //ni = -ni;
		TData dp; TVector::innerProduct(dif,ni,dp);
    for( unsigned int i=0; i<TAffineSpace::dim; i++ ) {
      dif[i] = ni[i]*dp;
    }
    TPoint z; z = p - dif;

    if(sqrt(TPredicates::squaredDistance(p,oi))<m_sampling) 
    {
      if (dp<0){
			  side = ON_NEGATIVE_SIDE;
		  } else if(dp==0){
			  side = ON_ORIENTED_BOUNDARY;
		  } else{
			  side = ON_POSITIVE_SIDE;
		  }
      return true;
    } 
    return false;
  }
  /** Trazado de contornos. Recorrido de todo el espacio
   */
  void doContourTracing() {
#if OHWDEBUG 
    cout<<"extayendo la isosuperficie (contour tracing)"<<endl; 
#endif

    int n = 0; 
    float sampling = m_sampling/1.0;
    n=(1.0/(sampling));
    float xsize=0,ysize=0,zsize=0;
    int ncells=0;
    TVector size = TVector(sampling,sampling,sampling);
    for (unsigned int i=0; i<n; i++) {
      ysize = 0;
      for (unsigned int j=0; j<n; j++) {
        zsize = 0;
        for (unsigned int k=0; k<n; k++) {
          ncells++;

			    unsigned char code = 0x00;

          //se calcula la distancia signada para cada vertice de la celda
			    code=0x00;
			    TPoint pos = TPoint(xsize,ysize,zsize);
			    TPoint val;       
          bool valid = false;
          for(int a=0; a<8; a++) {
            val=pos;
            for(int b=0; b<TAffineSpace::dim; b++) { 
              if( (a&(0x04>>b)) != 0x00 )
			          val[b]=pos[b]+size[b];
              else
                val[b]=pos[b];
            }
            OrientedSide side;
            valid = signedDistanceFunction(val,side);
	          if( !valid )
              break;
            else if ( side == ON_POSITIVE_SIDE)
		          code=code|(0x01<<a);
          }
          if (valid)
            TMarchingCells::processCell(pos,size,code,m_outputData);

          zsize += sampling;
        }
        ysize += sampling;
      }
      xsize += sampling;
    }
      
  }
  /** Trazado de contornos. Recorrido de los voxeles cortados por la superficie
   */
  void doContourTracing2() {
#if OHWDEBUG 
    cout<<"extayendo la isosuperficie (contour tracing)"<<endl; 
#endif

    int n = 0; float width = m_sampling;
    while( width < 1 ) {
       width = width*2;
       n++;
    }
    float sampling = m_sampling/1.0;
    n=(1.0/(sampling));
    float xsize=0,ysize=0,zsize=0;
    int ncells=0;
    TVector size = TVector(sampling,sampling,sampling);
    for (unsigned int i=0; i<m_centroids.size(); i++) {
      ncells++;

			unsigned char code = 0x00;
      TPoint& centroid = *m_centroids[i];
      xsize = ((int)(centroid[0]/sampling))*sampling;
      ysize = ((int)(centroid[1]/sampling))*sampling;
      zsize = ((int)(centroid[2]/sampling))*sampling;

      //se calcula la distancia signada para cada vertice de la celda
			code=0x00;
			TPoint pos = TPoint(xsize,ysize,zsize);
			TPoint val;       
      bool valid = false;
      for(int a=0; a<8; a++) {
        val=pos;
        for(int b=0; b<TAffineSpace::dim; b++) { 
          if( (a&(0x04>>b)) != 0x00 )
			      val[b]=pos[b]+size[b];
          else
            val[b]=pos[b];
        }
        OrientedSide side;
        valid = signedDistanceFunction(val,side);
	      if( !valid )
          break;
        else if ( side == ON_POSITIVE_SIDE)
		      code=code|(0x01<<a);
      }
			if (valid)
        TMarchingCells::processCell(pos,size,code,m_outputData);

    }
      
  }
  /** Trazado de contornos. Recorrido de los voxeles cortados por la superficie
   *  usando un octree.
   */
  void doContourTracing3() {
#if OHWDEBUG 
    cout<<"extayendo la isosuperficie (contour tracing)"<<endl; 
#endif

    int n = 0; float width = m_sampling;
    while( width < 1 ) {
       width = width*2;
       n++;
    }
    TVector treesize; 
    for(unsigned int i=0; i<TAffineSpace::dim; i++)
      treesize[i] = width;
    if(m_spaceTree!=0)
      delete m_spaceTree;
		m_spaceTree = new TSpatialTree(TPoint(),treesize,n);

    // Se insertan los puntos en el arbol
    Locator<TSpatialTree*> locator;
    for(unsigned int i=0; i<m_centroids.size(); i++) {
      TPoint& centroid = *m_centroids[i];
      locator.m_point = &centroid;
      m_spaceTree->locateData(&centroid,locator);
      m_cells.push_back(locator.getData());
    }
    cout<<"numero de celdas: "<<m_cells.size()<<endl;
    cout<<"tamaño: "<<treesize<<endl;
    for (unsigned int i=0; i<m_cells.size(); i++) {

			unsigned char code = 0x00;
      TSpatialTree& cell = *m_cells[i];
      //se calcula la distancia signada para cada vertice de la celda
			code=0x00;
			TPoint& pos = cell.getPosition();
      TPoint val;       
      bool valid = false;
      for(int a=0; a<8; a++) {
        val=pos;
        for(int b=0; b<TAffineSpace::dim; b++) { 
          if( (a&(0x04>>b)) != 0x00 )
			      val[b]=pos[b]+treesize[b];
          else
            val[b]=pos[b];
        }
        OrientedSide side;
        valid = signedDistanceFunction(val,side);
	      if( !valid )
          break;
        else if ( side == ON_POSITIVE_SIDE)
		      code=code|(0x01<<a);
      }
			if (valid)
        TMarchingCells::processCell(pos,treesize,code,m_outputData);
    }      
  }

	
private:
  // Atributos de entrada/salida
  /** Puntos de entrada
   */
  vector<TPoint*> m_inputData;
  /** Puntos de salida
   */
  vector<TPoint*> m_outputData;
  /** centroides
   */
  vector<TPoint*> m_centroids;
  /** normales
   */
  vector<TVector*> m_normals;
  /** Planos tangentes
   */
  vector<TFrame*> m_tangentPlanes;
  /** Celdas visitadas por el algoritmo de contour tracing
   */
  vector<TSpatialTree*> m_cells;
  // Atributos. Estructuras de datos
  /** Grafo de Riemannian
   */
  TGraph* m_riemannian;
  /** Minimum spanning tree
   */
  TGraph* m_mst;
  /** Arbol de información espacial
   */
  TSpatialTree* m_spaceTree;
  /** Parametro de entrada. Densidad de muestreo + error (p+d)
   */
  TData m_sampling;

};

CGL_END

#endif