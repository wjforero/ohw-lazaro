/**
 * $Revision$
 * $Date$
 */

#ifndef HOPPE_H
#define HOPPE_H

#include "base.h"
#include "enum.h"
#include "algebra.h"
#include "hoppevertex.h"
#include "volumetricspace.h"
#include "structures/graph.h"


CGL_BEGIN

/** Metodo de reconstrucción de Hugues Hoppe et'al
 */
template<class TVolumetricSpace>
class Hoppe {
public:
  // Tipos de datos
  /**
   */
  typedef Hoppe<TVolumetricSpace> TSelf;
  /**
   */
  typedef typename TVolumetricSpace::TAffineSpace TAffineSpace;
  /** Tipo de dato escalar
   */
  typedef typename TVolumetricSpace::TAffineSpace::TData TData;
  /** Marcos de referencia (planos tangentes)
   */
  typedef typename TVolumetricSpace::TAffineSpace::TAffineFrame TFrame;
  /** Matriz
   */
  typedef typename TVolumetricSpace::TAffineSpace::TMatrix TMatrix;
  /** Puntos de entrada/salida
   */
  typedef typename TVolumetricSpace::TAffineSpace::TAffinePoint TPoint;
  /** Vectores para los calculos
   */
  typedef typename TVolumetricSpace::TAffineSpace::TAffineVector TVector;
  /**
   */
  typedef typename TVolumetricSpace::TMarchingCells TMarchingCells;
  /** Estructura de datos espacial
   */
  typedef typename TVolumetricSpace::TSpatialStructure TSpatialTree;
  /** Vertices auxiliares para las busquedas
   */
  typedef typename TVolumetricSpace::TVertex TVertex;
  /** Grafo
   */
  typedef Graph<GraphNode,GraphEdge,TVertex*> TGraph;
  /** Clase utilitaria. Hace el test de orientación cuando se recorre
   * el EMST en depth first order.
   */
  template <class T> 
  class Orientator : public PrePostVisitor<T> {
  public:
    ofstream myOutput;

  public:
    /**
     */
    virtual T& preVisit(T& data) {
      T& parent = getData();
      // Se calcula el test de orientación
      TData val;
      TVector& ni = parent.getData()->getNormal();
      TVector& nj = data.getData()->getNormal();
      TVector::innerProduct(ni,nj,val);
      if(val>(-0.2) && val<0.1)
        cout<<"Producto pùnto: "<<val<<endl;
      
      //if( !(parent.getId()==data.getId()) && !(val>(-0.4) && val<0.4) && val < 0 ) 
      if( !(parent.getId()==data.getId()) && val < 0 ) {
        //cout<<"se invierte la normal1: "<<parent.getId()<<" "<<data.getId()<<", val: "<<val<<endl;
        //cout<<"ni antes: "<<ni<<" ";
        //cout<<"nj antes: "<<nj<<endl;
        //nj = -nj; 
        //data.getData()->getNormal()=-data.getData()->getNormal();
        data.getData()->invNormal();
        //cout<<"nj despues: "<<nj<<endl;
      }
      if(val<0.1 && val>-0.1) {
        myOutput<<"centroid: "<<parent.getData()->getPoint()<<endl;
        myOutput<<"normal: "<<ni<<endl;
        myOutput<<"centroid: "<<data.getData()->getPoint()<<endl;
        myOutput<<"normal: "<<nj<<endl;
      }
      setData(data);
      return parent;
    }
    /**
     */
    virtual T& postVisit(T& data) {
      setData(data);
      return getData();
    }
  }; 

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
      //cout<<*data<<endl;
      for( unsigned int i=0; i<TAffineSpace::dim; i++ ) {
        //cout<<"compara la coordenada "<<i<<endl;
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
      TPoint& p2 = data->getData()->getPoint();
      if(TAffineSpace::TPredicates::squaredDistance(point,p2) <= m_sampling)
        return false;
      return true;
    }
  };
public:
  // Constructores
  /** Constructor por omision
   */
  Hoppe() {
    this->m_mst = 0;
    this->m_riemannian = 0;
    this->m_sampling = 1;
    this->m_spaceTree = 0;
  }
  /** Construye un objeto de tipo Hoppe con el parametro especificado
   */
  Hoppe(TData sampling) : m_sampling(sampling){
    this->m_mst = 0;
    this->m_riemannian = 0;
    this->m_spaceTree = 0;
    TVertex::m_maxDist = sampling;
  }
public:
  // Operadores
public:
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
    m_tangentPlanes.push_back(0);
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
  /** Asigna un plano tangente para un punto de entrada especifico
   */
  void setTangentPlane(const unsigned int& i, TFrame& tp) {
    m_tangentPlanes[i] = &tp;
  }
  /** Retorna el plano tangente asociado al i-esimo punto de entrada
   */
  TSpatialTree& getCell(const unsigned int& i) const {
    return *m_cells[i];
  }
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

    // Se crea el arbol de busqueda
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
    vector<TVertex*>& vertices = *new vector<TVertex*>;
    Locator<TSpatialTree*> locator;
    for(unsigned int i=0; i<m_inputData.size(); i++) {
      cout<<"insertando el punto "<<i<<endl;
      TVertex* vertex = new TVertex(getInputData(i));
      locator.m_point = &vertex->getPoint();
      m_spaceTree->locateData(vertex,locator);
		  vertex->setNode(*locator.getData());
      vertex->getNode().setData(vertex);
		  vertices.push_back(vertex);
    }

    // Se calculan el plano tangente para cada uno de los puntos
    FindNeighbour<TSpatialTree*> neigbourFinder;
    neigbourFinder.m_sampling = m_sampling*m_sampling;
    for (unsigned int i=0; i<vertices.size(); i++) {
			//Traer los vecinos
			vector<TVertex*> neighbors;
      vector<TSpatialTree*> oneighbors;
			TVertex* vertex = vertices[i];
      neigbourFinder.m_point = &vertex->getPoint();
      m_spaceTree->findNeighbors(neigbourFinder, oneighbors);
      
      //vertex->getNode().findNeighbors(neighbors,TVertex::isNeighbor);
			for(unsigned int j=0; j<vertices.size(); j++) {
				TVertex* vertex1 = vertices[j];
				if(TVertex::isNeighbor(vertex,vertex1) && vertex!=vertex1)
					neighbors.push_back(vertex1);
			}
      cout<<"se encontraron "<<oneighbors.size()<<" ovecinos"<<endl;
      cout<<"se encontraron "<<neighbors.size()<<" vecinos"<<endl;
			
			//Calcular el centroid
		  TVector val;	
			for (unsigned int j=0; j<neighbors.size(); j++){

				val = val+ (neighbors[j]->getPoint());
			}
			TPoint centroid; centroid=centroid+val;
			for(j=0; j<TAffineSpace::dim; j++) {
				centroid[j] = centroid[j]/neighbors.size();
			}
      centroid[TAffineSpace::dim] = 1;
			
			// Calcular la matriz de covarianza
			Algebra<TData>::TMatrix cv(TAffineSpace::dim+1);
			Algebra<TData>::TMatrix cvi(TAffineSpace::dim+1);
			TVector dif;
			cv.setIdentity();
			for(j=0; j<TAffineSpace::dim+1; j++)
				cv[j][j]=0.0;
			for (j=0; j<neighbors.size(); j++) {
				TPoint &pp =neighbors[j]->getPoint();
				dif = (pp)-centroid;
				TVector::outerProduct(dif,dif,cvi);
				cv = cv+cvi;
			}

			// Calcula los vectores propios
			int nrot;
			TMatrix evectors(TAffineSpace::dim+1);
			evectors.setIdentity();
			Algebra<TData>::TVector evalues(TAffineSpace::dim+1);
			Algebra<TData>::jacobi(cv,evalues,evectors,nrot,TAffineSpace::dim);
			Algebra<TData>::eigsrt(evalues,evectors,TAffineSpace::dim);
			TVector* basis[TAffineSpace::dim];
      for(j=0; j<TAffineSpace::dim; j++) {
				basis[j] = new TVector;
				for(int h=0; h<TAffineSpace::dim; h++){
					basis[j]->operator[](h)=evectors[h][j];
				}
			}
      if(m_tangentPlanes[i]!=0)
        delete m_tangentPlanes[i];
      m_tangentPlanes[i] = new TFrame(centroid,basis);
      m_tangentPlanes[i]->makeRightHand();
			for(j=0; j<TAffineSpace::dim; j++) {
				delete basis[j];
			}
		}
    for(unsigned int i=0; i<vertices.size(); i++) {
      //vertices[i]->getNode().clearData();
      delete vertices[i];
    }
    delete &vertices;

    ofstream myCentroids;
    myCentroids.open("centroids.m3d");
    myCentroids<<m_tangentPlanes.size()<<" 0"<<endl;
    for(unsigned int i=0; i<m_tangentPlanes.size(); i++) {
      TPoint &origen = m_tangentPlanes[i]->getOrigin();
      myCentroids<<origen[0]<<" "<<origen[1]<<" "<<origen[2]<<" "<<endl;
      delete &origen;
    }

    myCentroids.close();

    ofstream myNormals;
    myNormals.open("normales.m3d");
    myNormals<<m_tangentPlanes.size()*3<<" "<<m_tangentPlanes.size()<<endl;
    float escala=m_sampling;
    for(unsigned int i=0; i<m_tangentPlanes.size(); i++) {
      TPoint &origen = m_tangentPlanes[i]->getOrigin();
      TVector &normal = getTangentPlane(i).getBasis(TAffineSpace::dim-1);
      TPoint cosa; cosa = origen + (normal*escala);
      myNormals<<origen[0]<<" "<<origen[1]<<" "<<origen[2]<<" "<<endl;
      myNormals<<cosa[0]<<" "<<cosa[1]<<" "<<cosa[2]<<" "<<endl;
      myNormals<<origen[0]<<" "<<origen[1]<<" "<<origen[2]<<" "<<endl;
      delete &origen;
    }
    for(unsigned int i=1; i<=m_tangentPlanes.size()*3; i+=3) {
      myNormals<<i<<" "<<i+1<<" "<<i+2<<endl;
    }
    myNormals.close();
  }
  /** Orientación consistente de los planos tangentes
   */
  void orientTangentPlanes() {
#if OHWDEBUG 
    cout<<"orientando los planos tangentes"<<endl; 
#endif

    // cout<< "Se crea el arbol de busqueda" <<endl;
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
    
    vector<TVertex*>& vertices = m_vertices;
    // cout<< "Se insertan los puntos en el arbol" <<endl;
    for(unsigned int i=0; i<m_inputData.size(); i++) { 
      TVertex& vertex = *new TVertex(/*getInputData(i));//*/getTangentPlane(i).getOrigin());
      vertex.setNode(*(m_spaceTree->locateData(&vertex,TVertex::locate)));
      vertex.getNode().addData(&vertex);
      //cout<<"vertex: "<<vertex.getNode().getData()<<endl;
      vertex.setNormal(getTangentPlane(i).getBasis(TAffineSpace::dim-1));
      vertices.push_back(&vertex);
		  m_cells.push_back(&(vertex.getNode()));
      //cout<<(m_cells.back())<<endl;
    }

    // cout<< "Se crea el grafo de Riemannian" <<endl;
    if(m_riemannian!=0)
      delete m_riemannian;
    m_riemannian = new TGraph;
    TGraph& riemannian = *m_riemannian;

     cout<< "Se insertan los puntos como nodos del grafo de riemannian" <<endl;
    unsigned int root; width = 0;
    for(unsigned int i=0; i<m_inputData.size(); i++) { 
      TVertex& vertex = *vertices[i];
      TGraph::TNode& gnode = riemannian.newNode();
      gnode.setData(&vertex);
      vertex.setGraphNode(gnode);
      if(vertex.getPoint()[TAffineSpace::dim-1]>width) {
        root = i;
        width = vertex.getPoint()[TAffineSpace::dim-1];
      }
    }

    unsigned int nfaces =0;
    ofstream myRiemannian;
    myRiemannian.open("riemannian.m3d");
    for(unsigned int i=0; i<riemannian.getSizeOfNodes(); i++) {
      TVertex& vertex = *vertices[i];
      TPoint &origen = vertex.getPoint();
      myRiemannian<<origen[0]<<" "<<origen[1]<<" "<<origen[2]<<" "<<endl;
    }
    
     cout<< "Se insertan los arcos del grafo de riemannian" <<endl;
    for(unsigned int i=0; i<m_cells.size(); i++) { 
      TVertex& vertex = *vertices[i];
      TVector& ni = vertex.getNormal();
      // Se buscan los vecinos a este vertice
      vector<TVertex*> neighbors;
      //cout<<"con datoll: : "<<(*getCell(i).getData()[0])<<endl;
		  //vertex.getNode().findNeighbors(neighbors,TVertex::isNeighbor);
      for(unsigned int j=0; j<m_cells.size(); j++) {
			  TVertex* vertex1 = vertices[j];
        if(TVertex::isNeighbor(&vertex,vertex1) && &vertex!=vertex1){
				  neighbors.push_back(vertex1);
          //cout<<"se adiciona a la vecindad el v : "<<j<<endl;
          //cout<<"con dato: : "<<(*vertex1)<<endl;
        }
		  }
      if(neighbors.size()==0)
        cout<<"la vecindad esta mal... ayayayay"<<endl;
      int jj=0;
		  for(unsigned int j=0; j<neighbors.size(); j++){
        TVertex& neighborvertex = *neighbors[j];
        //cout<<"vamos en p: "<<neighborvertex<<endl;
        unsigned int k = neighborvertex.getGraphNode().getId();
        TData distance = TAffineSpace::TPredicates::squaredDistance( *m_inputData[i], *m_inputData[k] );
        if( //(TData)(distance) <= (m_sampling*m_sampling) &&
            !riemannian.isEdge(riemannian.getNode(i),neighborvertex.getGraphNode()) ) {
              jj++;
          riemannian.addEdge(riemannian.getNode(i),neighborvertex.getGraphNode());
          TVector& nk = neighborvertex.getNormal();
          TVector::innerProduct(ni,nk,distance);
          riemannian.getEdge(riemannian.getNode(i),neighborvertex.getGraphNode()).setWeight((TData)1.0f-(TData)abs(distance));
          riemannian.getEdge(neighborvertex.getGraphNode(),riemannian.getNode(i)).setWeight((TData)1.0f-(TData)abs(distance));

          myRiemannian<<i+1<<" "<<k+1<<" "<<i+1<<endl;
          nfaces++;

        }
		  }

      if(jj==0){
        cout<<"No inserta que raro... ayayayay"<<endl;
      }
    }

    myRiemannian<<riemannian.getSizeOfNodes()<<" "<<nfaces<<endl;
    myRiemannian.close();

    // cout<<"Se construye el EMST para la orientación"<<endl;
    cout<<"Mayor coordedenada en z: "<<root<<endl;
    cout<<"punto:      "<<m_vertices[root]->getPoint()<<endl;
    cout<<"normal:     "<<m_vertices[root]->getNormal()<<endl;
    TVector& normal = m_vertices[root]->getNormal();
    cout<<"normal:     "<<m_vertices[root]->getNormal()<<endl;
    if(normal[TAffineSpace::dim-1]<0)
      normal = -normal;
    m_mst = &TGraph::primsAlgorithm(riemannian,root);
    TGraph& mst = *m_mst;
    Orientator<TGraph::TNode> preorient;
    preorient.myOutput.open("normalesquecambian.m3d");
    preorient.setData(m_mst->getNode(root));
    m_mst->depthFirstTraversal(root,preorient);
    preorient.myOutput.close();
    /*preorient.setData(m_mst->getNode(root));
    m_mst->depthFirstTraversal(root,preorient);
    /*preorient.setData(m_mst->getNode(root));
    m_mst->depthFirstTraversal(root,preorient);
    preorient.setData(m_mst->getNode(root));
    m_mst->depthFirstTraversal(root,preorient);*/

    nfaces =0;
    ofstream myMST;
    myMST.open("mst.m3d");
    for(unsigned int i=0; i<mst.getSizeOfNodes(); i++) {
      TVertex& vertex = *vertices[i];
      TPoint &origen = vertex.getPoint();
      myMST<<origen[0]<<" "<<origen[1]<<" "<<origen[2]<<" "<<endl;
    }
    for(unsigned int i=0; i<mst.getSizeOfNodes(); i++) {
      list<TGraph::TEdge*>& edges = mst.getEdges(i);
      for(list<TGraph::TEdge*>::iterator j = edges.begin(); j!= edges.end(); j++) {
        myMST<<i+1<<" "<<(*(*j)).mate(mst.getNode(i)).getId()+1<<" "<<i+1<<endl;
        nfaces++;
      }
    }
    myMST<<mst.getSizeOfNodes()<<" "<<nfaces<<endl;
    myMST.close();

    ofstream myNormals;
    myNormals.open("normales.m3d");
    myNormals<<m_tangentPlanes.size()*3<<" "<<m_tangentPlanes.size()<<endl;
    float escala=m_sampling;
    for(unsigned int i=0; i<m_tangentPlanes.size(); i++) {
      TPoint &origen = getInputData(i);
      TVector &normal = m_vertices[i]->getNormal();
      TPoint cosa; cosa = origen + (normal*escala);
      myNormals<<origen[0]<<" "<<origen[1]<<" "<<origen[2]<<" "<<endl;
      myNormals<<cosa[0]<<" "<<cosa[1]<<" "<<cosa[2]<<" "<<endl;
      myNormals<<origen[0]<<" "<<origen[1]<<" "<<origen[2]<<" "<<endl;
      //delete &origen;
    }
    for(unsigned int i=1; i<=m_tangentPlanes.size()*3; i+=3) {
      myNormals<<i<<" "<<i+1<<" "<<i+2<<endl;
    }
    myNormals.close();

  }
  /** Función de distancia signada
   */
  OrientedSide signedDistanceFunction(const TPoint& p, TSpatialTree* container) const {
#if OHWDEBUG 
    //cout<<"calculando la función de distancia signada"<<endl; 
#endif
    TPoint p1(p);
		
		// Se busca la celda correspondiente al putno		
		TVertex * vertex = new TVertex(p1);
		TSpatialTree* node = m_spaceTree->locateData(vertex,TVertex::locate);
		
		// Se buscan los vecinos a esta celda
    vector<TVertex*> neighbors;
		//if(node->getData()!=0) neighbors.push_back(node);
		//node->findNeighbors(neighbors,TVertex::isNeighbor);
    for(unsigned int j=0; j<m_cells.size(); j++) {
      //TVertex* vertex1 = new TVertex(getTangentPlane(j).getOrigin());//m_vertices[j];
      TVertex* vertex1 = m_vertices[j];
			if(TVertex::isNeighbor(vertex,vertex1) && vertex!=vertex1)
				neighbors.push_back(vertex1);
		}
    
		// Se busca el plano cuyo centroide sea mas cercano
		TData dist=1.0f;
		TVertex* pos=0;
		for(unsigned int i=0; i<neighbors.size(); i++){
			TVertex* vertex1 = neighbors[i];
      TData dist1 = TAffineSpace::TPredicates::squaredDistance( p,vertex1->getPoint() );
      if(vertex1!=0 && dist1 < dist) {
				pos=vertex1;
        dist = dist1;
      }
		}
    //pos = container->getData();
		// Se calcula la distancia signada
		if ( pos == 0  ) {
			//cout<<"error al obtener el frame"<<endl;
			return ON_ORIENTED_BOUNDARY;
		}
		TVector& ni = pos->getNormal();
		TPoint& oi = pos->getPoint();
		TVector dif; dif = (p-oi);
    //ni = -ni;
		TData dp; TVector::innerProduct(dif,ni,dp);
    /*cout<<"La resta: "<<dif<<endl;
    cout<<"La normal: "<<ni<<endl;
    cout<<"El producto punto: "<<dp<<endl;*/
    for( unsigned int i=0; i<TAffineSpace::dim; i++ ) {
      dif[i] = ni[i]*dp;
    }
    TPoint z; z = p - dif;

    if(sqrt(TAffineSpace::TPredicates::squaredDistance(z,oi))<(TVertex::m_maxDist*2)) 
    {
      if (dp<0){
			  return ON_NEGATIVE_SIDE;
		  } else if(dp==0){
			  return ON_ORIENTED_BOUNDARY;
		  } else{
			  return ON_POSITIVE_SIDE;
		  }
    } 
    return ON_ORIENTED_BOUNDARY;
  }
  /** Trazado de contornos
   */
  void doContourTracing() {
#if OHWDEBUG 
    cout<<"extayendo la isosuperficie (contour tracing)"<<endl; 
#endif

    TMarchingCells::myOutput.open("salihopee2d.m3d");
		// se obtienen las celdas que son interceptadas por la curva 
		for (unsigned int i=0; i<m_inputData.size(); i++) {
			//cout<<"calculando para el vertice "<<i<<endl;
			unsigned char code = 0x00;
      TSpatialTree & cell = *(m_cells[i]);//->father()->father());
			//cout<<"node visitado: "<<cell<<endl;
			//se calcula la distancia signada para cada vertice de la celda
			code=0x00;
			TVector size = TVector(cell.getSize());
      //cout<<"size :"<<size<<endl;
			TPoint pos = TPoint(cell.getPosition());
			TPoint val;       
      for(int j=0; j<TVolumetricSpace::dimc; j++) {
        val=pos;
        for(int k=0; k<TAffineSpace::dim; k++) { 
          if( (j&(0x04>>k)) != 0x00 )
			      val[k]=pos[k]+size[k];
          else
            val[k]=pos[k];
        }
        //cout<<"El vertice es "<<val<<endl;
        //if(val[2]<0.1) 
        {
			    if(signedDistanceFunction(val,&cell) == ON_POSITIVE_SIDE)
				    code=code|(0x01<<j);
        }

      }
      //code = 15;
			TMarchingCells::processCell(pos,size,code,m_outputData);
      //cout<<"el caso obtenido fue: "<<((unsigned int)code)<<endl;
		}
    for(long i=1; i<=TMarchingCells::npoints; i++) {
      TMarchingCells::myOutput<<i<<" ";
      if( (i%3) == 0)
        TMarchingCells::myOutput<<endl;
    }
    TMarchingCells::myOutput<<TMarchingCells::npoints<<" "<<TMarchingCells::npoints/3<<endl;
		TMarchingCells::myOutput.close();

  }
private:
  // Atributos de entrada/salida
  /** Puntos de entrada
   */
  vector<TPoint*> m_inputData;
  /** Puntos de salida
   */
  vector<TPoint*> m_outputData;
  /** Puntos de salida
   */
  vector<TVertex*> m_vertices;
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
  // Atributos. Parametros de entrada
  /**
   */
  TData m_sampling;

};

CGL_END

#endif