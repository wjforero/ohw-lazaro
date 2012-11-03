#ifndef CGLDOCUMENT3D_H
#define CGLDOCUMENT3D_H

#include <string>
#include <vector>
using std::string;
using std::vector;

#include "cgldocument.h"
#include "iodata/inputslices.h"
#include "iodata/inputmesh.h"
#include "iodata/inputcloudpoints3d.h"
#include "reconstruction/alphashapes/alphashape.h"
#include "algorithms/delaunay.h"
#include "algorithms/convexhull.h"
#include "reconstruction/kohonen/kohonenreconstruction.h"
#include "reconstruction/hoppe/newhoppe.h"
#include "meshmdd.h"
#include "progressdialog.h"
#include "kernel2d/affinespace2d.h"
#include "kernel3d/affinespace3d.h"
#include "kernel3d/predicates3d.h"
//#include "reconstruction/hoppe/hoppe.h"

// Para medidas de tiempo
#include <time.h>



/**
 */
class CglDocument3d :public CglDocument{
public:
	int* some_int;
	enum{
		// Técnicas de reconstrucción
		ID_ALPHASHAPE=1,
		ID_KOHONEN,
		ID_SLICERECONSTRUCTION,
		ID_HOPPE,
		ID_OCDEL,
		// Tecnicas para la extracción de normales de un conjunto de puntos
		ID_NORMALHOPPE,
		// Técnicas de geometría computacional
		ID_DELAUNAY,
		ID_CONVEXHULL,
		// Técnicas de grafos
		ID_RIEMANNIAN,
		ID_EMST,
		// Técnicas de volumetría
		ID_OCTREES,
		ID_POINTS,
		ID_MESH,
		ID_SLICES,		
		ID_NORMALS,
	};
public:
	/**
	*/
	typedef float TypeSpace;
	/** Espacio afín euclideo generico para cualquier dimensión
	*/
	typedef ohw::cgl::CGAffineSpace<TypeSpace,3> Tfloat3DSpace;
	/** Espacio d-dimensional especializado
	*/
	typedef AffineSpace3d<TypeSpace> TSpace;
	
	typedef NHoppe<TSpace> TNHoppe;

	/** Iterador del punto
	*/
	typedef Tfloat3DSpace::TAffinePoint* IterPoint;
	/** Vector de puntos
	*/
  typedef vector<IterPoint> TVectorPoint;
	/** Vector de slices
	*/
	typedef ohw::cgl::InputSlices::TVectorSlices TVectorSlices;
	/** Tipo de dato del alpha-shape
	*/
	typedef ohw::cgl::AlphaShape<Tfloat3DSpace> TAlphaShape;	
	/** Tipo de dato de kohonen
	*/
	typedef ohw::cgl::KohonenReconstruction<Tfloat3DSpace> TKohonen;
	/** Tipo de dato de reconstrucción por slices
	*/
	typedef ohw::cgl::SliceReconstruction<Tfloat3DSpace> TSliceReconstruction;
	/** Tipo de dato de la triangulación de Delaunay
	*/
	typedef ohw::cgl::Delaunay<Tfloat3DSpace> TDelaunay;
	/** Tipo de dato del convex-hull
	*/
	typedef ohw::cgl::ConvexHull<Tfloat3DSpace> TConvexHull;
	/** Tipo de malla 3d
	*/
	typedef MeshMdd<Tfloat3DSpace> TMeshMdd;

  typedef ohw::cgl::CGAffineSpace<float,3> Tfloat3DSpace;
  typedef Tfloat3DSpace::TAffinePoint* IterPoint;
	typedef Tfloat3DSpace::TAffinePoint TPoint;
  typedef ohw::cgl::InputCloudPoints3d Input3d;
  typedef MeshMdd<Tfloat3DSpace> TMeshMdd;	

public:
	/**
	*/
	CglDocument3d();
	/** Abre la malla 3d
	*/
	bool openMesh3d(const string& file);
	
	/** Abre la malla 3d
	*/
	bool openMesh3ds(const string& file);
	
	/** Abre la malla 3d
	*/
	bool openSlices(const string& file);

	/** Exporta la ultima malla
	*/
	bool exportMesh3d(const string& file);
	/** Exporta la ultima malla a vrml
	*/
	bool exportMesh3dVrml(const string& file);
	/** Exporta la ultima malla a obj
	*/
	bool exportMesh3dObj(const string& file);
  /**
   */
  void setModified(const bool& mod) ;
public:
  /**
   */
  virtual bool onNewDocument();
  /**
   */
  virtual void onOpen(string pathFile);
  /**
   */
  virtual bool onCloseDocument();
  /**
   */
  virtual void onSaveDocument();
	/**
	*/
	void alpha(TypeSpace valpha);

	double getAlpha();

	/**
	*/
	void kohonen(TypeSpace vscale);

	/**
	*/
	void alpha(TypeSpace valpha,TVectorPoint* vp);
	/**
	*/
	void kohonen(TypeSpace valScale,TVectorPoint* vp);

	void hoppe(TypeSpace density);
	/**
	*/
	void hoppe(TypeSpace density,TVectorPoint* vp);
	/**
	*/
	void ocdel();
	/**
	*/
	void slices();
	/**
	*/
	void slices(TVectorSlices* vectorSlices);
	/**
	*/
	void normalHoppe(TypeSpace );
	/**
	*/
	void delaunay();
	/**
	*/
	void delaunay(TVectorPoint* vp);

	/**
	*/
	void convexhull();

	/**
	*/
	void convexhull(TVectorPoint* vp);

	/**
	*/
	void riemannian(TypeSpace radius);

	/**
	*/
	void emst(TypeSpace radius);
	/**
	*/
	void octress();
	TMeshMdd* getMeshResult();

	/** Calcula las normales del conjunto de puntos
	*/
	void computeNormals(float sampling);
	void normalize(TVectorPoint& vp);
	time_t getTimeElapsed1();

	time_t getTimeElapsed2();

private:
  /** Apuntador al alpha shape
   */ 
	TAlphaShape* m_alphaShape;
	/** Apuntador a kohonen
	*/
	TKohonen* m_kohonen;
	/** Apuntador a slice reconstruction
	*/
	TSliceReconstruction* m_sliceReconstruction;	
	/**
	*/
	vector<TMeshMdd*> m_meshDd;
  TMeshMdd* m_activeMeshDd;	
	TMeshMdd* m_activeMeshDdPoint;	
	TVectorSlices* m_activeSlice;	
	time_t m_timeelapsed1;
	time_t m_timeelapsed2;
};

/**
*/
CglDocument3d::CglDocument3d() : CglDocument(){
	m_alphaShape=0;		
	m_activeSlice=0;
}

/** Abre la malla 3d
*/
bool CglDocument3d::openMesh3d(const string& file){
	bool cargo=false;
  vector<IterPoint>* points=new vector<IterPoint>();
	unsigned int* vertices;
	unsigned int sizeMesh;
	InputMesh* mesh=new InputMesh(Input3d::FORMAT_M3D);		
	mesh->setVector(points);
	cargo=mesh->load(file);    		
	vertices=mesh->getVerticesMesh();
//	normalize(*points);
	m_activeMeshDd=new TMeshMdd(points,vertices,mesh->getSizeMesh());
	m_activeMeshDdPoint=m_activeMeshDd;
	m_meshDd.push_back(m_activeMeshDd);
	if(mesh->getSizeMesh()==0)
		m_activeMeshDd->setTypeMesh(ID_POINTS);
	else
		m_activeMeshDd->setTypeMesh(ID_MESH);
	setModified(true);
  return cargo;
}

/** Abre la malla 3ds
*/
bool CglDocument3d::openMesh3ds(const string& file){
	bool cargo=false;
  vector<IterPoint>* points=new vector<IterPoint>();
	unsigned int* vertices;
	unsigned int sizeMesh;
	InputMesh* mesh=new InputMesh(Input3d::FORMAT_3DS);		
	mesh->setVector(points);
	cargo=mesh->load(file);    		
	vertices=mesh->getVerticesMesh();
	normalize(*points);
	m_activeMeshDd=new TMeshMdd(points);
	m_activeMeshDdPoint=m_activeMeshDd;
	m_meshDd.push_back(m_activeMeshDd);
	m_activeMeshDd->setTypeMesh(ID_POINTS);
	setModified(true);
  return cargo;
}

/** Abre la malla 3d
*/
bool CglDocument3d::openSlices(const string& file){    
	bool cargo=false;
	vector<IterPoint>* points=new vector<IterPoint>();
	InputSlices is;
	if(m_activeSlice){
		delete m_activeSlice;
	}
	m_activeSlice = new TVectorSlices();
	
  is.setVectorSlices(m_activeSlice);
  cargo=is.load(file);
  
  for(int i=0;i<m_activeSlice->size();i++){
    for(int j=0;j<(*m_activeSlice)[i]->getSize();j++)
      points->push_back((*m_activeSlice)[i]->getPoint(j));          
  }
	m_activeMeshDd=new TMeshMdd(points);
	m_activeMeshDd->setTypeMesh(ID_SLICES);
	m_meshDd.push_back(m_activeMeshDd);
	setModified(true);
  return cargo;
}

bool CglDocument3d::exportMesh3d(const string& file){    
	unsigned i=0;
	if(m_activeMeshDd){						
		ofstream fileOut(file.data());
		fileOut<<m_activeMeshDd->getSizePoints()<<" "<<m_activeMeshDd->getSizeMesh()<<endl;
		TVectorPoint* vp=m_activeMeshDd->getVectorPoints();
		for(i=0;i<vp->size();i++){				
			fileOut<<(*vp)[i]->getData(0)<<" "<<(*vp)[i]->getData(1)<<" "<<(*vp)[i]->getData(2)<<endl;
		}
		unsigned int* ver=m_activeMeshDd->getVertices();
		for(i=0;i<m_activeMeshDd->getSizeMesh();i++){				
			fileOut<<ver[i*3]<<" "<<ver[i*3+1]<<" "<<ver[i*3+2]<<endl;
		}
	}
	return true;		
}

/** Exporta la ultima malla
*/
bool CglDocument3d::exportMesh3dObj(const string& file){    
	unsigned i=0;
	if(m_activeMeshDd){						
		ofstream fileOut(file.data());
		//fileOut<<m_activeMeshDd->getSizePoints()<<" "<<m_activeMeshDd->getSizeMesh()<<endl;
		TVectorPoint* vp=m_activeMeshDd->getVectorPoints();
		for(i=0;i<vp->size();i++){				
			fileOut<<"v "<<(*vp)[i]->getData(0)<<" "<<(*vp)[i]->getData(1)<<" "<<(*vp)[i]->getData(2)<<endl;
		}
		unsigned int* ver=m_activeMeshDd->getVertices();
		for(i=0;i<m_activeMeshDd->getSizeMesh();i++){				
			fileOut<<"f "<<ver[i*3]<<" "<<ver[i*3+1]<<" "<<ver[i*3+2]<<endl;
		}
	}
	return true;		
}

/** Exporta la ultima malla
*/
bool CglDocument3d::exportMesh3dVrml(const string& file){    
	unsigned i=0;
	if(m_activeMeshDd){						
		ofstream fileOut(file.data());
		TVectorPoint* vp=m_activeMeshDd->getVectorPoints();		
		unsigned int* ver=m_activeMeshDd->getVertices();
		fileOut<<"#VRML V2.0 utf8 \n Background {skyColor [ 0 0 0 ]}";
		fileOut<<"Shape { appearance Appearance {material Material {ambientIntensity 0.50196 diffuseColor 1 1 1 specularColor 0 0 0 emissiveColor 0 0 0 shininess 1 transparency 0}}";
		fileOut<<"geometry IndexedFaceSet { \n ccw    TRUE \n 	convex TRUE \n 	solid  FALSE  \n coordIndex [";
		for(i=0;i<m_activeMeshDd->getSizeMesh();i++){
			fileOut<<(ver[i*3]-1)<<", "<<(ver[i*3+1]-1)<<", "<<(ver[i*3+2]-1)<<",";
			if(i==m_activeMeshDd->getSizeMesh()-1)
				fileOut<<"-1";
			else
				fileOut<<"-1,";
		}
		fileOut<<"] \n  coord Coordinate { point [";
		for(i=0;i<vp->size();i++){				
			fileOut<<""<<(*vp)[i]->getData(0)<<" "<<(*vp)[i]->getData(1)<<" "<<(*vp)[i]->getData(2);
			if(i!=vp->size()-1)
				fileOut<<",";
		}
		fileOut<<"] }}}";		
	}
	return true;		
}


/**
  */
void CglDocument3d::setModified(const bool& mod) {
  unsigned int i=0;      
  for(i=0;i<m_vectView.size();i++) {
    m_vectView[i]->updateView();
  }
}    

/**
  */
bool CglDocument3d::onNewDocument() {
  return true;
}
/**
  */
void CglDocument3d::onOpen(string pathFile) {
}
/**
  */
bool CglDocument3d::onCloseDocument() {
  return true;
}
/**
  */
void CglDocument3d::onSaveDocument() {  
}


/**
*/
void CglDocument3d::alpha(TypeSpace valpha){
	if(m_activeMeshDd){			
		alpha(valpha,m_activeMeshDdPoint->getVectorPoints());
	}		
}

double CglDocument3d::getAlpha(){
	if(m_alphaShape){
		return m_alphaShape->getValAlpha();
	}
	return 0;
}

/**
*/
void CglDocument3d::kohonen(TypeSpace vscale){
	if(m_activeMeshDd){
		kohonen(vscale,m_activeMeshDdPoint->getVectorPoints());
	}		
}	

/**
*/
void CglDocument3d::alpha(TypeSpace valpha,TVectorPoint* vp){
	vector<IterPoint>* vectResultAlpha=new vector<IterPoint>();
	unsigned int* indexResultAlpha;
	unsigned int sizeResultAlpha;
	if(m_alphaShape){
		delete m_alphaShape;			
	}
	m_alphaShape=new TAlphaShape();				
	// Inserta los puntos
	
	time_t timeIni,timeEnd;
	time(&timeIni);
	m_alphaShape->insertPoints(*vp);		
	// Ejecuta el alpha shape
	m_alphaShape->alpha();
	
	time(&timeEnd);
	m_timeelapsed1 = timeEnd - timeIni;
	time(&timeIni);
	// Fija el valor del alpha
	m_alphaShape->setValAlpha(valpha);		
	time(&timeEnd);
	m_timeelapsed2 = timeEnd - timeIni;
	

	m_alphaShape->getResult(*vectResultAlpha,&indexResultAlpha,sizeResultAlpha);
	m_activeMeshDd=new TMeshMdd(vectResultAlpha,indexResultAlpha,sizeResultAlpha);
	m_activeMeshDd->setTypeMesh(ID_ALPHASHAPE);
	m_meshDd.push_back(m_activeMeshDd);
	setModified(true);
	
}
/**
*/
void CglDocument3d::kohonen(TypeSpace valScale,TVectorPoint* vp){
	vector<IterPoint>* vectResultKohonen=new vector<IterPoint>();
	unsigned int* indexResultKohonen;
	unsigned int sizeResultKohonen;
	// Construye kohonen
	TKohonen koh; 
	// Fija los puntos y un valor de escala
	time_t timeIni,timeEnd;
	time(&timeIni);
	koh.reconstructDefaultParameters(*vp,valScale);		
	time(&timeEnd);
	m_timeelapsed1 = timeEnd - timeIni;
	koh.getResult(*vectResultKohonen,&indexResultKohonen,sizeResultKohonen);		
	m_activeMeshDd=new TMeshMdd(vectResultKohonen,indexResultKohonen,sizeResultKohonen);
	m_activeMeshDd->setTypeMesh(ID_KOHONEN);
	m_meshDd.push_back(m_activeMeshDd);
	setModified(true);
}

void CglDocument3d::hoppe(TypeSpace density){
	if(m_activeMeshDd)
		hoppe(density,m_activeMeshDd->getVectorPoints());
	
}
/**
*/
void CglDocument3d::hoppe(TypeSpace density,TVectorPoint* vp){
	vector<IterPoint>* vectResultHoppe=new vector<IterPoint>();
	unsigned int* indexResultHoppe;
	unsigned int sizeResultHoppe;

	TSpace::TAffinePoint *p=0;
	unsigned int i=0,size=0;
	TNHoppe hoppe(density);
	for(i=0;i<vp->size();i++){						
		p=new TSpace::TAffinePoint((*vp)[i]->getData(0),(*vp)[i]->getData(1),(*vp)[i]->getData(2));
		hoppe.addInputData(*p);			
	}
	hoppe.calculateTangentPlanes();
	hoppe.orientTangentPlanes();
	hoppe.doContourTracing2();
	size=hoppe.getOutputDataSize();	
	for(i=0;i<size;i++){
		TSpace::TAffinePoint& p3d=hoppe.getOutputPoint(i);			
		IterPoint ip=new TPoint();
		(*ip)[0]=p3d[0];(*ip)[1]=p3d[1];(*ip)[2]=p3d[2];
		vectResultHoppe->push_back(ip);
	}
	indexResultHoppe=new unsigned int[size];
	sizeResultHoppe=size/3;
	for(i=0;i<size;i++){
		indexResultHoppe[i]=i+1;
	}


	m_activeMeshDd=new TMeshMdd(vectResultHoppe,indexResultHoppe,sizeResultHoppe);
	m_activeMeshDd->setTypeMesh(ID_HOPPE);
	m_meshDd.push_back(m_activeMeshDd);
	setModified(true);
}
/**
*/
void CglDocument3d::ocdel(){

}
/**
*/
void CglDocument3d::slices(){
	if(m_activeSlice){
		slices(m_activeSlice);
	}
}
/**
*/
void CglDocument3d::slices(TVectorSlices* vectorSlices){
	vector<IterPoint>* vectResultSlices=new vector<IterPoint>();
	unsigned int* indexResultSlices;
	unsigned int sizeResultSlices;
	unsigned int i=0;
	TSliceReconstruction sr(vectorSlices->size());
	for(i=0;i<vectorSlices->size();i++){
		sr.addSliceInLevel((*vectorSlices)[i],i);    
	}
	time_t timeIni,timeEnd;
	time(&timeIni);		
	sr.reconstruct();
	time(&timeEnd);
	m_timeelapsed1 = timeEnd - timeIni;
	sr.getResult(*vectResultSlices,&indexResultSlices,sizeResultSlices);
	m_activeMeshDd=new TMeshMdd(vectResultSlices,indexResultSlices,sizeResultSlices);
	m_activeMeshDd->setTypeMesh(ID_SLICERECONSTRUCTION);		
	m_meshDd.push_back(m_activeMeshDd);
	setModified(true);
}	
/**
*/
void CglDocument3d::normalHoppe(TypeSpace ){
	
}
/**
*/
void CglDocument3d::delaunay(){
	if(m_activeMeshDd){
		delaunay(m_activeMeshDd->getVectorPoints());
	}
}
/**
*/
void CglDocument3d::delaunay(TVectorPoint* vp){
	vector<IterPoint>* vectResultDelaunay=new vector<IterPoint>();
	unsigned int* indexResultDelaunay;
	unsigned int sizeResultDelaunay;
	TDelaunay del;
	del.insertPoints(*vp);
	del.getResult(*vectResultDelaunay,&indexResultDelaunay,sizeResultDelaunay);
	m_activeMeshDd=new TMeshMdd(vectResultDelaunay,indexResultDelaunay,sizeResultDelaunay);
	m_activeMeshDd->setTypeMesh(ID_DELAUNAY);
	m_meshDd.push_back(m_activeMeshDd);
	setModified(true);
}

/**
*/
void CglDocument3d::convexhull(){
	if(m_activeMeshDd){
		convexhull(m_activeMeshDd->getVectorPoints());
	}
}

/**
*/
void CglDocument3d::convexhull(TVectorPoint* vp){
	vector<IterPoint>* vectResultConvexHull=new vector<IterPoint>();
	unsigned int* indexResultConvexHull;
	unsigned int sizeResultConvexHull;
	TConvexHull ch;
	ch.insertPoints(*vp);
	ch.getResult(*vectResultConvexHull,&indexResultConvexHull,sizeResultConvexHull);
	m_activeMeshDd=new TMeshMdd(vectResultConvexHull,indexResultConvexHull,sizeResultConvexHull);
	m_activeMeshDd->setTypeMesh(ID_CONVEXHULL);
	m_meshDd.push_back(m_activeMeshDd);
	setModified(true);		
}

/**
*/
void CglDocument3d::riemannian(TypeSpace radius){

}

/**
*/
void CglDocument3d::emst(TypeSpace radius){
	
}
/**
*/
void CglDocument3d::octress(){
	
}
/**
*/
CglDocument3d::TMeshMdd* CglDocument3d::getMeshResult(){
	return m_activeMeshDd;
}	

/**
*/
void CglDocument3d::normalize(TVectorPoint& vp){
	unsigned int i=0,j=0;
	TypeSpace* scale = new TypeSpace[3];
	TypeSpace* min = new TypeSpace[3];
	TypeSpace* max = new TypeSpace[3];
	
	// Para cada dimension encuentra el maximo
	for(i=0;i<3;i++){
		min[i] = vp[0]->getData(i);
		max[i] = vp[0]->getData(i);
	}
			
	for(i=0;i<vp.size();i++)
		for(j=0;j<3;j++){			
			if( vp[i]->getData(j) < min[j])
				min[j]=vp[i]->getData(j);
			if(vp[i]->getData(j)> max[j])
				max[j]=vp[i]->getData(j);
	}
	for(i=0;i<vp.size();i++)
		for(j=0;j<3;j++)
			if(min[j]!=max[j])
				(*(vp[i]))[j]=((*(vp[i]))[j]-min[j])/(max[j] - min[j]);
	delete[] min;
	delete[] max;
}

time_t CglDocument3d::getTimeElapsed1(){
	return m_timeelapsed1;
}

time_t CglDocument3d::getTimeElapsed2(){
	return m_timeelapsed2;
}

void CglDocument3d::computeNormals(float sampling){
	unsigned int i=0;
	if(m_activeMeshDd){
		vector<IterPoint>* vp=m_activeMeshDd->getVectorPoints();				
		TNHoppe hoppe(sampling);
		for(i=0; i<vp->size(); i++) {						
			TSpace::TAffinePoint *p = new TSpace::TAffinePoint((*vp)[i]->getData(0),(*vp)[i]->getData(1),(*vp)[i]->getData(2));					
			hoppe.addInputData(*p);			
		}		
		time_t timeIni,timeEnd;
		time(&timeIni);
		hoppe.calculateTangentPlanes();  
		hoppe.orientTangentPlanes();
		time(&timeEnd);
		m_timeelapsed1 = timeEnd - timeIni;		

		vector<IterPoint>* vectResultNormals=new vector<IterPoint>();
		unsigned int* indexResultNormals;
		unsigned int sizeResultNormals;

    for(i=0; i<vp->size(); i++) {			
			TNHoppe::TPoint& centroid = (hoppe.getCentroid(i));
			TNHoppe::TVector& normal = (hoppe.getNormal(i));
			IterPoint ip=0;
			ip=new TPoint();
			(*ip)[0]=( centroid[0]);(*ip)[1]=( centroid[1] );(*ip)[2]=( centroid[2] );
			vectResultNormals->push_back(ip);
			ip=new TPoint();
			(*ip)[0]=( centroid[0] + ( normal[0] * sampling ) );(*ip)[1]=( centroid[1] + ( normal[1] * sampling ) );(*ip)[2]=( centroid[2] + ( normal[2] * sampling ) );
			vectResultNormals->push_back(ip);			
			ip=new TPoint();
			(*ip)[0]=( centroid[0]);(*ip)[1]=( centroid[1] );(*ip)[2]=( centroid[2] );
			vectResultNormals->push_back(ip);
    }

		indexResultNormals=new unsigned int[vp->size()*3];
		sizeResultNormals=vp->size();
    for(i=0;  i<vp->size()*3; i++) {
      indexResultNormals[i]=i+1;
    }		

		m_activeMeshDd=new TMeshMdd(vectResultNormals,indexResultNormals,sizeResultNormals);
		m_activeMeshDd->setTypeMesh(ID_NORMALS);
		m_meshDd.push_back(m_activeMeshDd);
		setModified(true);
	}
}

#endif