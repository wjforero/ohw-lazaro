/**
 * $Revision: 1.1 $
 * $Date: 2003/03/11 20:55:29 $
 */

#include <iostream>
#include <fstream>
using namespace std;

#include "cgl.h"
#include "kernel2d/affinespace2d.h"
#include "kernel3d/affinespace3d.h"
#include "kernel3d/predicates3d.h"
#include "reconstruction/hoppe/hoppe.h"
#include "structures/cell.h"
#include "structures/octree.h"


/*
CGL_BEGIN 
    
 
 template <class T>
  class Locator : public Visitor<T> {
    typedef AffineSpace3d<float> TMySpace;
    typedef typename TMySpace::TAffinePoint TPoint;
    typedef typename TMySpace::TAffineVector TVector;
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
      for( unsigned int i=0; i<TMySpace::dim; i++) {
        //cout<<"compara la coordenada "<<i<<endl;
        if( point[i] < pos[i] || point[i] > pos[i]+size[i] )
          return false;
      }
      return true;
    }
  };

  CGL_END
*/
    using namespace ohw::cgl;

/*void mainoct() {

  typedef float TScalar;
	typedef AffineSpace3d<TScalar> TSpace;
  typedef AffineSpace3d<TScalar>::TAffinePoint TPoint;
  typedef Octree<TSpace,TPoint*> TOctree;

  TPoint& punto1 = *new TPoint(0.4f,0.1f,0.2f);

  Locator<TOctree*> miLocator;
  
  //Cell<TSpace> cell;
  
  TOctree &octree = *new TOctree(TSpace::TAffinePoint(0,0,0),TSpace::TAffineVector(1,1,1),5);
  
  miLocator.m_point = &punto1;
  octree.locateData(&punto1, miLocator);
  TOctree *search =  miLocator.getData();
  cout<<(*search)<<endl;
  search = search->eqFaceNeighbor(1);
  cout<<search<<endl;
  if(search != 0)
    cout<<(*search)<<endl;
  
  delete &octree;
  
  delete &punto1;
}*/

void main() {
	typedef float TScalar;
	typedef AffineSpace3d<TScalar> TSpace;
	typedef VolumetricSpace3d<TSpace> TVolumetricSpace;
	
	Hoppe<TVolumetricSpace> hoppe(0.11f);
  int npoints=60;
	int nfaces=0;
	int i;
	
  ifstream myInput;
  //myInput.open("..\\nubes\\mechpart.4102.m3d");
  //myInput.open("..\\nubes\\cabeza.m3d");
  myInput.open("..\\nubes\\cubo.m3d"); 
  //myInput.open("..\\nubes\\cactus.m3d"); 
  //myInput.open("..\\nubes\\cat10.10000.m3d");
  //myInput.open("..\\nubes\\apple.m3d");
  myInput>>npoints>>nfaces;
	for(i=0; i<npoints; i++) {
		float x,y,z;
    myInput>>x>>y>>z;
		TSpace::TAffinePoint *p=new TSpace::TAffinePoint(x,y,z);		
		hoppe.addInputData(*p);
    //cout<<(*p)<<endl;
	}

  cout<<"se cargaron "<<npoints<<endl;

  myInput.close();
  hoppe.calculateTangentPlanes();
  //hoppe.orientTangentPlanes();
	//hoppe.doContourTracing();

  ofstream myOutput;
  myOutput.open("salida.m3d");

  myOutput<<hoppe.getOutputDataSize()<<" "<<hoppe.getOutputDataSize()/3<<endl;
  for(i=0; i<hoppe.getOutputDataSize(); i++) {
    TSpace::TAffinePoint &point = hoppe.getOutputPoint(i);
		myOutput<<point[0]<<" "<<point[1]<<" "<<point[2]<<" "<<endl;
	}
  for(long j=1; j<=hoppe.getOutputDataSize(); j++) {
    myOutput<<j<<" ";
    if( (j%3) == 0)
      myOutput<<endl;
  }
  myOutput.close();
	


  
}

/*void mainCUBES() {

  typedef float TScalar;
	typedef AffineSpace3d<TScalar> TSpace;
	typedef VolumetricSpace3d<TSpace> TVolumetricSpace;
	
  TVolumetricSpace::TMarchingCells::myOutput.open("mccases.m3d");

  TSpace::TAffineVector size(0.5,0.5,0.5);
  TSpace::TAffineVector incremento(0.8,0,0);
  TSpace::TAffinePoint pos(0,0,0);
  unsigned char code = 0x00;
  for (unsigned int i=1; i<256; i++) {
	  //cout<<"calculando para el vertice "<<i<<endl;
	  
	  TSpace::TAffinePoint val;       
    for(int j=0; j<TVolumetricSpace::dimc; j++) {
      val=pos;
      for(int k=0; k<TSpace::dim; k++) { 
        if( (j&(0x04>>k)) != 0x00 )
			    val[k]=pos[k]+size[k];
        else
          val[k]=pos[k];
      }
      //cout<<"El vertice es "<<val<<endl;
		  

    }
    code ++;
    //TVolumetricSpace::TMarchingCells::processCell(pos,size,code);

    pos = pos + incremento;
  }
  
  for(long i=1; i<=TVolumetricSpace::TMarchingCells::npoints; i++) {
    TVolumetricSpace::TMarchingCells::myOutput<<i<<" ";
    if( (i%3) == 0)
      TVolumetricSpace::TMarchingCells::myOutput<<endl;
  }

  TVolumetricSpace::TMarchingCells::myOutput<<TVolumetricSpace::TMarchingCells::npoints<<" "<<TVolumetricSpace::TMarchingCells::npoints/3<<endl;
	TVolumetricSpace::TMarchingCells::myOutput.close();

}
*/