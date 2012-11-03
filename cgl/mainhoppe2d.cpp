/**
 * $Revision: 1.1 $
 * $Date: 2003/03/11 20:55:29 $
 */

#include <iostream>
#include <fstream>
using namespace std;

#include "cgl.h"
#include "kernel2d/affinespace2d.h"
#include "kernel3d/predicates3d.h"
#include "reconstruction/hoppe.h"

using namespace ohw::cgl;

void main() {
	typedef float TScalar;
	typedef AffineSpace2d<TScalar> TSpace;
	typedef VolumetricSpace2d<TSpace> TVolumetricSpace;
	
	Hoppe<TVolumetricSpace> hoppe;
  int npoints=60;
	int nfaces=0;
	int i;
	ofstream myOutput;
	myOutput.open("salida.m3d");
	myOutput<<npoints<<" "<<nfaces<<endl;
	float t = 2*3.1416/(float)npoints;
	for(i=0; i<npoints; i++) {
		float x=t*i;
		float y=x;
		x= cos(x);y=sin(y);
		TSpace::TAffinePoint *p=new TSpace::TAffinePoint((x+1)/2.4,(y+1)/2.4);		
		hoppe.addPoint(p);
		myOutput<<*p<<endl;
	}
	cout<<"se calculan los planos tangentes"<<endl;
	hoppe.calculateTangentPlanes();
	cout<<"se calculan contour tracing"<<endl;
	hoppe.doContourTracing();
	cout<<"fin"<<endl;
	myOutput.close();

	unsigned char c=0x10;
	cout<<c<<endl;
	c=0x00;
	cout<<c<<endl;

}