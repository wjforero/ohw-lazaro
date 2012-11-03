#ifndef FORMATMXD_H
#define FORMATMXD_H

// inclusiones especificas
#include "base.h"
#include "kernel/affinespace.h"

#include <istream>

CGL_BEGIN

/** Esta clase carga una scena 3ds (3d studio max)
*/
template <class TAffineSpace>
class FormatMXD{
  public:
    enum{
       dim=TAffineSpace::dim
    };
    typedef TAffineSpace::TAffinePoint TPoint;
    typedef TAffineSpace::TAffinePoint* IterPoint;
    typedef TAffineSpace::TData TData;
	public:
    static bool loadModelPoints(const string& path,vector<IterPoint>& points);
    static bool loadModel(const string& path,vector<IterPoint>& points,unsigned int** vertices,unsigned int& size);
  public:
};

template <class TAffineSpace>
bool FormatMXD<TAffineSpace>::loadModelPoints(const string& path,vector<IterPoint>& points){
  unsigned int i=0,j=0;
  unsigned int npoints, nsimplexes;
  TData t;
  ifstream file(path.data());
  if(!file.is_open()){
    EXCEPTION("El archivo no existe");
    return false;
  }

  file>>npoints>>nsimplexes;
  IterPoint p=0;
  points.clear();
  for(i=0;i<npoints;i++){
    p=new TPoint();
    for(j=0;j<dim;j++){
      file>>t;
      (*p)[j]=t;      
    }
    points.push_back(p);
  }

  return true;
}

template <class TAffineSpace>
bool FormatMXD<TAffineSpace>::loadModel(const string& path,vector<IterPoint>& points,unsigned int** vertices,unsigned int& size){
  unsigned int i=0,j=0;
  unsigned int npoints,indexv;
  TData t;
  ifstream file(path.data());
  if(!file.is_open()){
    EXCEPTION("El archivo no existe");
    return false;
  }

  file>>npoints>>size;
  IterPoint p=0;
  points.clear();
  for(i=0;i<npoints;i++){
    p=new TPoint();
    for(j=0;j<dim;j++){
      file>>t;
      (*p)[j]=t;      
    }
    points.push_back(p);
  }
  
  (*vertices)=new unsigned int[size*dim];
  for(i=0;i<size;i++){
    for(j=0;j<dim;j++){
      file>>indexv;
      (*vertices)[i*3+j]=indexv;
    }                  
	}
  return true;
}

CGL_END

#endif