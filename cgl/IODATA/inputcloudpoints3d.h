#ifndef INPUTCLOUDPOINTS3D_H
#define INPUTCLOUDPOINTS3D_H

// Inclusiones especificas
#include "inputcloudpoints.h"
#include "formats/format3ds/scene3ds.h"
#include "formats/utilformat/readdatafilebinary.h"
#include "formats/mxd/formatmxd.h"
#include "kernel/affinespace.h"

CGL_BEGIN

class InputCloudPoints3d: public InputCloudPoints{
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
    InputCloudPoints3d();    
    /** Crea una clase para carga de puntos con el formato especificado
    */
    InputCloudPoints3d(const int& m_typeFormat);
    /** Destructor
    */
    ~InputCloudPoints3d();
    /** Retorna la referencia a los puntos
    */
    IterVectorPoints getPoints();    
    /** Fija el vector en el cual se almacenan los puntos
    */
    void setVector(IterVectorPoints points);

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
};

InputCloudPoints3d::InputCloudPoints3d():m_typeFormat(FORMAT_NONE),m_points(0){
  
}

InputCloudPoints3d::InputCloudPoints3d(const int& m_typeFormat):m_typeFormat(m_typeFormat),m_points(0){
  
}

void InputCloudPoints3d::setVector(IterVectorPoints points){
  m_points=points;
}

InputCloudPoints3d::~InputCloudPoints3d(){  
}

InputCloudPoints3d::IterVectorPoints InputCloudPoints3d::getPoints(){
  return m_points;
}

bool InputCloudPoints3d::loadData(const std::string& file){
  unsigned int i=0;
  if(m_points==0){
    EXCEPTION("El vector de almacenamiento es NULL");
    return false;
  }
  m_points->clear();
  if(m_typeFormat==FORMAT_ALIASWAVEFRONT){
    return false;
  }else if(m_typeFormat==FORMAT_3DS){    
		ofstream ff;
		ff.open("hola.txt");
		ff<<"hace algo "<<endl;
    ReadDataFileBinary rdb(file);
    if(!rdb.ready()){
      EXCEPTION("El archivo no existe");
      return false;
    }
    Scene3ds sc3ds(&rdb);    
    sc3ds.loadModel();
    Scene3ds::H3dsScene* s=sc3ds.getScene();
    if(s->meshobjs>=1){            
      m_points->clear();

			for(int j=0; j<s->meshobjs; j++){
				for(i=0; i<s->meshobjlist[j].verts; i++){
					IterPoint p=new TPoint();
					(*p)[0]=s->meshobjlist[j].vertlist[i].x;
					(*p)[1]=s->meshobjlist[j].vertlist[i].y;
					(*p)[2]=s->meshobjlist[j].vertlist[i].z;
					m_points->push_back(p);
				}
			}
    }		
    return true;
  }else if(m_typeFormat==FORMAT_M3D){
    TFormatM3D::loadModelPoints(file,*m_points);
    return false;
  }
  return false;
}

void InputCloudPoints3d::setTypeFormat(const int& typeFormat){
  m_typeFormat=typeFormat;
}

CGL_END

#endif