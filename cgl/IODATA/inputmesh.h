#ifndef INPUTMESH_H
#define INPUTMESH_H

// Inclusiones especificas
#include "input.h"
#include "formats/format3ds/scene3ds.h"
#include "formats/utilformat/readdatafilebinary.h"
#include "formats/mxd/formatmxd.h"
#include "kernel/affinespace.h"

CGL_BEGIN

class InputMesh: public InputCloudPoints{
  public:
    enum{FORMAT_NONE,FORMAT_ALIASWAVEFRONT,FORMAT_3DS,FORMAT_M3D};
  public:
    typedef CGAffineSpace<float,3> Tfloat3DSpace;
    typedef Tfloat3DSpace::TAffinePoint TPoint;
    typedef Tfloat3DSpace::TAffinePoint* IterPoint;
    typedef vector<IterPoint> TVectorPoints;
    typedef vector<IterPoint>* IterVectorPoints;

    typedef FormatMXD<Tfloat3DSpace> TFormatM3D;

  public:
    /** Constructor por omisión
    */
    InputMesh();    
    /** Crea una clase para carga de puntos con el formato especificado
    */
    InputMesh(const int& m_typeFormat);
    /** Destructor
    */
    ~InputMesh();
    /** Retorna la referencia a los puntos
    */
    IterVectorPoints getPoints();    
    /** Fija el vector en el cual se almacenan los puntos
    */
    void setVector(IterVectorPoints points);

		/** Fija el vector en el cual se los indices de la malla
    */
    void setVectorMesh(unsigned int* vertices){
			m_vertices=vertices;
		}

		/** Fija el vector en el cual se los indices de la malla
    */
    void setSizeVectorMesh(unsigned int sizeMesh){
			m_sizeMesh=sizeMesh;
		}

		unsigned int* getVerticesMesh(){
			return m_vertices;
		}

		unsigned int getSizeMesh(){
			return m_sizeMesh;
		}		

    /** Fija el tipo de formato de apertura
    */
    void setTypeFormat(const int& typeFormat);
  protected:
    /** Carga los datos de los puntos
    */
    bool loadData(const string& file);
  private:
    // Tipo de formato con el que se carga
    int m_typeFormat;
    // Apuntador a los puntos
    IterVectorPoints m_points;
		// Tamaño de la malla
		unsigned int m_sizeMesh;
		// Apuntador con los vertices
		unsigned int* m_vertices;
};

InputMesh::InputMesh():m_typeFormat(FORMAT_NONE),m_points(0),m_sizeMesh(0),m_vertices(0){
  
}

InputMesh::InputMesh(const int& m_typeFormat):m_typeFormat(m_typeFormat),m_points(0){
  
}

void InputMesh::setVector(IterVectorPoints points){
  m_points=points;
}

InputMesh::~InputMesh(){  
}

InputMesh::IterVectorPoints InputMesh::getPoints(){
  return m_points;
}

bool InputMesh::loadData(const std::string& file){
  int i=0;
  if(m_points==0){
    EXCEPTION("El vector de almacenamiento es NULL");
    return false;
  }
  m_points->clear();
  if(m_typeFormat==FORMAT_ALIASWAVEFRONT){
    return false;
  }else if(m_typeFormat==FORMAT_3DS){    
		
    ReadDataFileBinary rdb(file);
    if(!rdb.ready()){
      EXCEPTION("El archivo no existe");
      return false;
    }
    Scene3ds sc3ds(&rdb);    
    sc3ds.loadModel();
    Scene3ds::H3dsScene* s=sc3ds.getScene();
		cout<<"s->meshobjs "<<s->meshobjs;
    if(s->meshobjs>=1){            
      m_points->clear();

      for(i=0; i<s->meshobjlist[0].verts; i++){
        IterPoint p=new TPoint();
        (*p)[0]=s->meshobjlist[0].vertlist[i].x;
        (*p)[1]=s->meshobjlist[0].vertlist[i].y;
        (*p)[2]=s->meshobjlist[0].vertlist[i].z;
        m_points->push_back(p);
      }      
    }
    return true;
  }else if(m_typeFormat==FORMAT_M3D){
    TFormatM3D::loadModel(file,*m_points,&m_vertices,m_sizeMesh);
    return false;
  }
  return false;
}

void InputMesh::setTypeFormat(const int& typeFormat){
  m_typeFormat=typeFormat;
}

CGL_END

#endif