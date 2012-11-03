#ifndef CLOUDSLICE_H
#define CLOUDSLICE_H

// inclusiones especificas
#include "base.h"
#include "kernel/affinespace.h"
#include "../../../reconstruction/slices/slicereconstruction.h"

#include <istream>

CGL_BEGIN

/** Esta clase carga una scena con polygonal slices
*/
template <class TAffineSpace>
class CloudSlice{
  public:
    enum{
       dim=TAffineSpace::dim
    };
    typedef TAffineSpace::TAffinePoint TPoint;
    typedef TAffineSpace::TAffinePoint* IterPoint;
    typedef TAffineSpace::TData TData;

    typedef Slice<TAffineSpace> TSlice;
    typedef Slice<TAffineSpace>* IterSlice;
    typedef vector<IterSlice> TVectorSlices;
    typedef vector<IterSlice>* IterVectorSlices;
	public:    
    static bool loadModelSlices(const string& path,const unsigned int& dimSlices,TVectorSlices& vectorSlices);
  public:
};

template <class TAffineSpace>
bool CloudSlice<TAffineSpace>::loadModelSlices(const string& path,const unsigned int& dimSlices,TVectorSlices& vectorSlices){
  unsigned int i=0,j=0,k=0;
  unsigned int npoints, nsimplexes;
  bool isNewSlice=true;
  vector<IterPoint> pointsSlice;
  Slice<TAffineSpace>* sliceLoad;
  TData t,ts;
  ifstream file(path.data());
  if(!file.is_open()){
    EXCEPTION("El archivo no existe");
    return false;
  }

  file>>npoints>>nsimplexes;
  IterPoint p=0;  
  isNewSlice=false;
  pointsSlice.clear();
  for(i=0;i<npoints;i++){
    p=new TPoint();
    for(j=0;j<dim;j++){
      file>>t;
      (*p)[j]=t;
      if(j==dimSlices){
        if(i==0){          
          ts=t;          
        }
        if(t!=ts){
          ts=t;
          isNewSlice=true;
        }
      }      
    }
    if(isNewSlice){
      sliceLoad=new TSlice(pointsSlice.size());
      // Es punto es de un nuevo slice
      for(k=0;k<pointsSlice.size();k++){        
        sliceLoad->setPoint(pointsSlice[k],k);
      }      
      pointsSlice.clear();      
      isNewSlice=false;
      vectorSlices.push_back(sliceLoad);
    }    
    pointsSlice.push_back(p);
  }
  sliceLoad=new TSlice(pointsSlice.size());
  for(k=0;k<pointsSlice.size();k++)
    sliceLoad->setPoint(pointsSlice[k],k);    
  vectorSlices.push_back(sliceLoad);
  return true;
}

CGL_END

#endif