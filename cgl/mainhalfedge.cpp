#include "kernel/affinespace.h"
#include "kernel3d/affinespace3d.h"
#include "structures/hehalfedge.h"

using namespace ohw::cgl;
int main(int argc, char *argv[]){
  typedef float TScalar;
  typedef AffineSpace3d<TScalar> TSpace;
  typedef Halfedge<HalfedgeVertex,HalfedgeEdge,HalfedgeFace,TSpace> THalfedge;


  THalfedge& he = *new THalfedge();
  TSpace::TAffinePoint& p1 = *new TSpace::TAffinePoint(1,0,0);
  TSpace::TAffinePoint& p2 = *new TSpace::TAffinePoint(2,0,0);
  TSpace::TAffinePoint& p3 = *new TSpace::TAffinePoint(3,0,0);
  cout<<he.addVertex(p1)<<endl;
  cout<<he.addVertex(p2)<<endl;
  cout<<he.addVertex(p3)<<endl;
  cout<<he.addFace(0,1,2)<<endl;;
  delete &he;
  delete &p1;
  delete &p2;
  delete &p3;
  return 0;
}
