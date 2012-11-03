#ifndef CGLDOCUMENTMDD_H
#define CGLDOCUMENTMDD_H

#include "cgldocument.h"
#include "meshmdd.h"
#include "iodata/inputcloudpoints3d.h"
#include "iodata/inputcloudpoints3d.h"

class CGLView;

class CglDocumentMdd: public CglDocument {
public:
  typedef ohw::cgl::CGAffineSpace<float,3> Tfloat3DSpace;
  typedef Tfloat3DSpace::TAffinePoint* IterPoint;
  typedef ohw::cgl::InputCloudPoints3d Input3d;
  typedef MeshMdd<Tfloat3DSpace> TMeshMdd;
  
public:
    enum FileTypes{TYPE_M3D = -1, TYPE_3DS, TYPE_M2D};

public:
  /**
   */
  CglDocumentMdd() {
  }
  /**
   */
  ~CglDocumentMdd() {
  }
    
  /**
   */
  virtual bool onNewDocument(){
    return true;
  }

  /**
   */
  virtual void onOpen(const string& pathFile){
    // Abre el documento
    switch(m_type){
      case TYPE_M3D:break;
      case TYPE_3DS:break;
      case TYPE_M2D:break;
    }
  }

  /**
   */
  virtual bool onCloseDocument(){
    // Cierra el documento
    return true;
  }

  /**
   */
  virtual void onSaveDocument(){
    // Serializa el documento
  }

public:    

  void setTypeFile( const unsigned int& type){
    m_type=type;
  }

  bool openPoints3d(const string& file){		
    bool cargo=false;
    vector<IterPoint>* points=new vector<IterPoint>();
    Input3d pointsio(Input3d::FORMAT_3DS);      
    pointsio.setVector(points);
    cargo=pointsio.load(file);      
    TMeshMdd* mdd=new TMeshMdd(points);
    m_meshDd.push_back(mdd);		
    if(cargo)
      setModified(true);
    return cargo;
  }

  bool openMesh3d(const string& file){
    bool cargo=false;
    vector<IterPoint>* points=new vector<IterPoint>();
    Input3d pointsio(Input3d::FORMAT_3DS);      
    pointsio.setVector(points);
    cargo=pointsio.load(file);      
    m_activeMeshDd=new TMeshMdd(points);
		m_meshDd.push_back(m_activeMeshDd);
    if(cargo)
      setModified(true);
    return cargo;
  }

  bool openPoints2d(const string& file){      
    return true;
  }

  bool openMesh2d(const string& file){
    return true;
  }

  bool alphaReconstruct(const float& val){
    // cargar con alpha shape
    setModified(true);
    return true;
  }

  bool hoppeReconstruct(const float& val){
    // reconstruir con hoppe
    setModified(true);
    return true;
  }

  bool octDelReconstruct(const float& val){
    // reconstruir con octdel
    setModified(true);
    return true;
  }

  bool kohonenReconstruct(const float& val){
    // reconstruir con kohonen
    setModified(true);
    return true;
  }

  bool slicesReconstruct(const float& val){
    // reconstruir con slices
    setModified(true);
    return true;
  }

  bool slicesBranchingReconstruct(const float& val){
    // reconstruir problema del branching
    setModified(true);
    return true;
  }

  bool delaunay(){
    // reconstruir problema del branching
    setModified(true);
    return true;
  }

  bool convexHull(){
    // reconstruir problema del branching
    setModified(true);
    return true;
  }

  bool Voronoi(){      
    setModified(true);
    return true;
  }

  TMeshMdd* getSelectMesh(){
    m_meshDd.back();
  }
protected:  
  unsigned int m_type;
};

#endif