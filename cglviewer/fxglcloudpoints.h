#ifndef FXGLCLOUDPOINTS_H
#define FXGLCLOUDPOINTS_H

#ifndef FXGLSHAPE_H
#include "FXGLShape.h"
#endif

#include <vector>
using std::vector;

#include "kernel/affinespace.h"
#include "base.h"


/** Construye una malla 3d con puntos y vertices
*/
class FXGLCloudPoints : public FXGLShape {
  FXDECLARE(FXGLCloudPoints)
public:
  typedef ohw::cgl::CGAffineSpace<float,3> Tfloat3DSpace;
  typedef Tfloat3DSpace::TAffinePoint* IterPoint;
  typedef vector<IterPoint> VectorPoints;
  typedef vector<IterPoint>* IterVectorPoints;  

public:  
  /** Construye una Malla de triangulos con los puntos, y los indices de cada triangulo, sobre un origen
  *   @param x centro x
  *   @param y centro y
  *   @param z centro z
  */
  FXGLCloudPoints(const FXfloat& x,const FXfloat& y,const FXfloat& z,IterVectorPoints points,FXfloat *n=0);
  
  /** Constructor de copia
  */
  FXGLCloudPoints(const FXGLCloudPoints& mesh);  

  /** Destructor
  */
  virtual ~FXGLCloudPoints();

  /** Copia este objeto
  */
  virtual FXGLObject* copy();

  /** Cambia los puntos de la malla
  */
  void setVertex(IterVectorPoints points);  
  
  /** Almacena en un stream la malla
  */
  virtual void save(FXStream& store) const;

  /** Carga la malla de un stream
  */
  virtual void load(FXStream& store);
  
protected:
  /** Constructor por omisión
  */
  FXGLCloudPoints();
  /** Dibuja el shape
  */
  virtual void drawshape(FXGLViewer* viewer);
  /** Recalcula las normas
  */
  virtual void recomputerange();
  /** Genera las normales
  */
  virtual void generatenormals();  

private:
  IterVectorPoints m_vertexBuffer;  
  FXfloat *m_normalBuffer;  
};

// Implementación del objeto
FXIMPLEMENT(FXGLCloudPoints,FXGLShape,NULL,0)

FXGLCloudPoints::FXGLCloudPoints()
:m_vertexBuffer(0),m_normalBuffer(0){
  // Crea un bounding box de los puntos
  range[0][0]=0.0; range[0][1]=0.0;
  range[1][0]=0.0; range[1][1]=0.0;
  range[2][0]=0.0; range[2][1]=0.0;
}

FXGLCloudPoints::FXGLCloudPoints(const FXfloat& x,const FXfloat& y,const FXfloat& z,IterVectorPoints points,FXfloat *n)
//:FXGLShape(x,y,z,SHADING_SMOOTH|STYLE_SURFACE),m_vertexBuffer(points),m_normalBuffer(n){
:FXGLShape(x,y,z,SHADING_FLAT|STYLE_POINTS),m_vertexBuffer(points),m_normalBuffer(n){
  if(!m_normalBuffer){ 
    generatenormals(); 
  }
  recomputerange();
}

FXGLCloudPoints::FXGLCloudPoints(const FXGLCloudPoints& orig):FXGLShape(orig){
  FXMEMDUP(&m_normalBuffer,FXfloat,orig.m_normalBuffer,3*orig.m_vertexBuffer->size());
  m_vertexBuffer=orig.m_vertexBuffer;  
}

FXGLCloudPoints::~FXGLCloudPoints(){      
  FXFREE(&m_normalBuffer);  
}

void FXGLCloudPoints::setVertex(IterVectorPoints points){
  m_vertexBuffer=points;
  recomputerange();
}

void FXGLCloudPoints::recomputerange(){
  // Recalcula los rangos
  register FXint n;
  register FXfloat t;
  IterPoint tp=0;
  range[0][0]=0.0; range[0][1]=0.0;
  range[1][0]=0.0; range[1][1]=0.0;
  range[2][0]=0.0; range[2][1]=0.0;  
  if(m_vertexBuffer && m_vertexBuffer->size()>0){
    tp=(*m_vertexBuffer)[0];
    range[0][0]=range[0][1]=tp->getData(0);
    range[1][0]=range[1][1]=tp->getData(1);
    range[2][0]=range[2][1]=tp->getData(2);

    for(n=0;n<m_vertexBuffer->size();n++){      
      tp=(*m_vertexBuffer)[n];
      t=tp->getData(0);
      if(t<range[0][0]) range[0][0]=t;
      if(t>range[0][1]) range[0][1]=t;
      t=tp->getData(1);
      if(t<range[1][0]) range[1][0]=t;
      if(t>range[1][1]) range[1][1]=t;
      t=tp->getData(2);
      if(t<range[2][0]) range[2][0]=t;
      if(t>range[2][1]) range[2][1]=t;
    }           
  }

}

void FXGLCloudPoints::drawshape(FXGLViewer*){
  // Dibuja el objeto
  if(!m_vertexBuffer || m_vertexBuffer->size()==0) 
     return;  
#ifdef HAVE_OPENGL
  register FXint i;  
  
  glBegin(GL_POINTS);
  glColor3f(0.0,0.0,1.0);
  for(i=0; i<m_vertexBuffer->size(); i++){          
    glPointSize(8);
    glVertex3f((*m_vertexBuffer)[i]->getData(0),
               (*m_vertexBuffer)[i]->getData(1),
               (*m_vertexBuffer)[i]->getData(2));
    
  }
  glEnd();
    
#endif
}

void FXGLCloudPoints::generatenormals(){
  // Calcula las normales a cada punto
  /*register FXint i;
  FXASSERT(m_vertexBuffer);
  IterPoint v1,v2,v3;
  if(!m_normalBuffer) 
    FXMALLOC(&m_normalBuffer,FXfloat,3*orig.m_vertexBuffer->size());

  for(i=0; i<orig.m_vertexBuffer->size(); i++){
    v1=(*m_vertexBuffer)[m_vertices[i*3]-1];
    v2=(*m_vertexBuffer)[m_vertices[i*3+1]-1];
    v3=(*m_vertexBuffer)[m_vertices[i*3+2]-1];

    FXVec a(v1->getData(0),v1->getData(1),v1->getData(2));
    FXVec b(v2->getData(0),v2->getData(1),v2->getData(2));
    FXVec c(v3->getData(0),v3->getData(1),v3->getData(2));    

    c-=b;
    b-=a;

    FXVec normal=b^c;

    normal=normalize(normal);

    m_normalBuffer[i]=normal[0];
    m_normalBuffer[i+1]=normal[1];
    m_normalBuffer[i+2]=normal[2];    
  }*/
}

// Copy this object
FXGLObject* FXGLCloudPoints::copy(){
  return new FXGLCloudPoints(*this);
}

// Save object to stream
void FXGLCloudPoints::save(FXStream& store) const {
}


// Load object from stream
void FXGLCloudPoints::load(FXStream& store){
}

#endif