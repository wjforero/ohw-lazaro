#ifndef FXGTRIANGLEM3D_H
#define FXGTRIANGLEM3D_H

#ifndef FXGLSHAPE_H
#include "FXGLShape.h"
#endif

#include <vector>
using std::vector;


#include "kernel/affinespace.h"
#include "base.h"
#include "cgldocument3d.h"

/** Construye una malla 3d con puntos y vertices
*/
class FXGLTriangleM3D : public FXGLShape {
  FXDECLARE(FXGLTriangleM3D)
public:
  typedef ohw::cgl::CGAffineSpace<float,3> Tfloat3DSpace;
  typedef Tfloat3DSpace::TAffinePoint* IterPoint;
  typedef vector<IterPoint> VectorPoints;
  typedef vector<IterPoint>* IterVectorPoints;  
  typedef unsigned int TVertex;
  typedef unsigned int* IterVertices;

public:  
  /** Construye una Malla de triangulos con los puntos, y los indices de cada triangulo, sobre un origen
  *   @param x centro x
  *   @param y centro y
  *   @param z centro z
  */
  FXGLTriangleM3D(const FXfloat& x,const FXfloat& y,const FXfloat& z,IterVectorPoints points,IterVertices vertices,const unsigned int& sizeTriangles,FXfloat *n=0);
	
	/** Constructor a partir de una malla
	*/
	FXGLTriangleM3D(CglDocument3d::TMeshMdd* mesh);
  
  /** Constructor de copia
  */
  FXGLTriangleM3D(const FXGLTriangleM3D& mesh);  

  /** Destructor
  */
  virtual ~FXGLTriangleM3D();

  /** Copia este objeto
  */
  virtual FXGLObject* copy();

  /** Cambia los puntos de la malla
  */
  void setVertex(IterVectorPoints points);

  /** Fija los vertices
  */
  void setVertices(IterVertices vertices,const unsigned int& sizeTriangles);

  /**
  */
  FXint getVertexNumber() const;    
  
  /** Almacena en un stream la malla
  */
  virtual void save(FXStream& store) const;

  /** Carga la malla de un stream
  */
  virtual void load(FXStream& store);

	void setSelected(const bool& sel){
		selected=sel;
	}

	void setTypeMesh(const unsigned int& typeMesh){
		m_typeMesh=typeMesh;
	}
	
	unsigned int getTypeMesh(){
		return m_typeMesh;
	}
  
protected:
  /** Constructor por omisión
  */
  FXGLTriangleM3D();
  /** Dibuja el shape
  */
  virtual void drawshape(FXGLViewer* viewer);
  /** Recalcula las normas
  */
  virtual void recomputerange();
  /** Genera las normales
  */
  virtual void generatenormals();  

protected:
	bool selected;

private:
  IterVectorPoints m_vertexBuffer;
  IterVertices m_vertices;
  unsigned long m_sizeTriangles;
  FXfloat *m_normalBuffer;  
	unsigned int m_typeMesh;
};

// Implementación del objeto
FXIMPLEMENT(FXGLTriangleM3D,FXGLShape,NULL,0)

FXGLTriangleM3D::FXGLTriangleM3D()
:m_vertexBuffer(0),m_vertices(0),m_sizeTriangles(0),m_normalBuffer(0){
  // Crea un bounding box de los puntos
  range[0][0]=0.0; range[0][1]=0.0;
  range[1][0]=0.0; range[1][1]=0.0;
  range[2][0]=0.0; range[2][1]=0.0;
}

FXGLTriangleM3D::FXGLTriangleM3D(CglDocument3d::TMeshMdd* mesh)
:m_vertexBuffer(mesh->getVectorPoints()),m_vertices(mesh->getVertices()),m_sizeTriangles(mesh->getSizeMesh()),m_normalBuffer(0){
  // Crea un bounding box de los puntos
  range[0][0]=0.0; range[0][1]=0.0;
  range[1][0]=0.0; range[1][1]=0.0;
  range[2][0]=0.0; range[2][1]=0.0;
}


FXGLTriangleM3D::FXGLTriangleM3D(const FXfloat& x,const FXfloat& y,const FXfloat& z,IterVectorPoints points,IterVertices vertices,const unsigned int& sizeTriangles,FXfloat *n)
:FXGLShape(x,y,z,SHADING_SMOOTH|STYLE_SURFACE|FACECULLING_OFF),m_vertexBuffer(points),m_vertices(vertices),m_sizeTriangles(sizeTriangles),m_normalBuffer(n){
  if(!m_normalBuffer){ 
    generatenormals(); 
  }
  recomputerange();
	selected=false;
}

FXGLTriangleM3D::FXGLTriangleM3D(const FXGLTriangleM3D& orig):FXGLShape(orig){
  FXMEMDUP(&m_normalBuffer,FXfloat,orig.m_normalBuffer,3*orig.m_sizeTriangles);
  FXMEMDUP(&m_vertices,TVertex,orig.m_vertices,3*orig.m_sizeTriangles);
  m_vertexBuffer=orig.m_vertexBuffer;
  m_sizeTriangles=orig.m_sizeTriangles;
}

FXGLTriangleM3D::~FXGLTriangleM3D(){      
  FXFREE(&m_normalBuffer);  
}

void FXGLTriangleM3D::setVertex(IterVectorPoints points){
  m_vertexBuffer=points;  
  recomputerange();
}

void FXGLTriangleM3D::setVertices(IterVertices vertices,const unsigned int& sizeTriangles){
  m_vertices=vertices;
  m_sizeTriangles=sizeTriangles;  
}


void FXGLTriangleM3D::recomputerange(){
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

void FXGLTriangleM3D::drawshape(FXGLViewer*){
  // Dibuja el objeto
  if(!m_vertexBuffer || m_vertexBuffer->size()==0) 
     return;  
#ifdef HAVE_OPENGL
  register FXint i;  
  IterPoint v1,v2,v3;	
	if(m_sizeTriangles>0){
		glBegin(GL_TRIANGLES);
		glColor3f(0.0,0.0,0.0);  
		for(i=0; i<m_sizeTriangles; i++){
			v1=(*m_vertexBuffer)[m_vertices[i*3]-1];
			v2=(*m_vertexBuffer)[m_vertices[i*3+1]-1];
			v3=(*m_vertexBuffer)[m_vertices[i*3+2]-1];

			if(m_normalBuffer){
				glNormal3fv(&m_normalBuffer[3*i]);
			}

			glVertex3f(v1->getData(0),v1->getData(1),v1->getData(2));
			if(m_normalBuffer){
				glNormal3fv(&m_normalBuffer[3*i]);      
			}
			glVertex3f(v2->getData(0),v2->getData(1),v2->getData(2));
			if(m_normalBuffer){
				glNormal3fv(&m_normalBuffer[3*i]);      
			}
			glVertex3f(v3->getData(0),v3->getData(1),v3->getData(2));
		}
		glEnd();
	}else{
		// Dibuja el objeto
		if(!m_vertexBuffer || m_vertexBuffer->size()==0) 
			 return;  
	  
		glBegin(GL_POINTS);
		glColor3f(0.0,0.0,1.0);		
		for(i=0; i<m_vertexBuffer->size(); i++){
			glPointSize(50);
			glVertex3f((*m_vertexBuffer)[i]->getData(0),
								(*m_vertexBuffer)[i]->getData(1),
								(*m_vertexBuffer)[i]->getData(2));
	    
		}
		glEnd();
	}	
	if((options&STYLE_BOUNDBOX) || selected){
	  glDisable(GL_LIGHTING);
	  glShadeModel(GL_FLAT);
	  if(selected){
	    glColor3f(0.0,1.0,0.0);
	    drawbox();
	    glPointSize(4.0);
	    drawhandles();
    }
	  else{
	    glColor3f(0.7f,0.7f,0.7f);
	    drawbox();
    }
  }	
#endif
}

void FXGLTriangleM3D::generatenormals(){
  // Calcula las normales a cada triangulo
  register FXint i;
  FXASSERT(m_vertexBuffer);
  IterPoint v1,v2,v3;
  if(!m_normalBuffer) 
    FXMALLOC(&m_normalBuffer,FXfloat,m_sizeTriangles*3);

  for(i=0; i<m_sizeTriangles; i++){
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

    m_normalBuffer[i*3]=normal[0];
    m_normalBuffer[i*3+1]=normal[1];
    m_normalBuffer[i*3+2]=normal[2];    
  }
}

// Copy this object
FXGLObject* FXGLTriangleM3D::copy(){
  return new FXGLTriangleM3D(*this);
}

// Save object to stream
void FXGLTriangleM3D::save(FXStream& store) const {
}


// Load object from stream
void FXGLTriangleM3D::load(FXStream& store){
}

#endif