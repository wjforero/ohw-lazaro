#include "iodata/inputslices.h"
#include "iodata/inputmesh.h"
#include "iodata/inputcloudpoints3d.h"
#include "reconstruction/alphashapes/alphashape.h"
#include "algorithms/delaunay.h"
#include "algorithms/convexhull.h"
#include "reconstruction/kohonen/kohonenreconstruction.h"
#include "kernel2d/affinespace2d.h"
#include "kernel3d/affinespace3d.h"
#include "kernel3d/predicates3d.h"
#include "reconstruction/hoppe/hoppe.h"
#include <fstream>



using namespace ohw::cgl;

typedef CGAffineSpace<float,3> TSpace;
typedef SimplexAlpha<TSpace>* IterSimplexAlpha;


void main(){	
	vector<TSpace::TAffinePoint*>* points=new vector<TSpace::TAffinePoint*>();
	InputMesh* mesh=new InputMesh(InputMesh::FORMAT_3DS);			
	mesh->setVector(points);
	mesh->load("elipse.3ds");		
	cout<<points->size();
}