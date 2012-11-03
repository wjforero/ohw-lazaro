#ifndef TRIANGLEMESH_H
#define TRIANGLEMESH_H

// Inclusiones especificas
#include "fox.h"

/** Modela una malla m3d
*/
class TriangleMesh: public FXGLTriangleMesh{
  public:
    /** Construye una malla cuyo origen esta en x,y,z
    */
    TriangleMesh(const FXfloat& x,const FXfloat& y,const FXfloat& z);
		/** Destructor
		*/
    ~TriangleMesh();

		/** Fija el número de vertices
		*/
    void setVertexNumber(FXint nvertices);    

  private:


};

TriangleMesh::TriangleMesh(const FXfloat& x,const FXfloat& y,const FXfloat& z):FXGLTriangleMesh(x,y,z,0){
  
}

#endif