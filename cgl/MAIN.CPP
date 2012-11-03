/**
 * $Revision: 1.2 $
 * $Date: 2003/02/18 12:29:22 $
 */

#include <iostream>
using namespace std;

#include "cgl.h"
#include "kernel2d/predicates2d.h"
#include "kernel3d/predicates3d.h"
#include "reconstruction/hoppe.h"

using namespace ohw::cgl;

void main() {
	typedef float TScalar;
	typedef AffineSpace2d<TScalar> TSpace;
	typedef VolumetricSpace2d<TSpace> TVolumetricSpace;
	TSpace space;

	//TSpace::TAffinePoint v1(2,2);
	//TSpace::TAffinePoint v2(4,2);
	//TSpace::TAffinePoint v3(2,0);//=space.getVector(0,1,0);
	//TSpace::TAffinePoint c3 = Predicates2d<TSpace>::circumcenter(v1,v2,v3);
	//cout<<TSpace::TAffineVector::crossProduct(v1,v1)<<endl;
	//cout<<endl<<c3<<endl;
  
	Hoppe<TVolumetricSpace> hoppe;
	/*TSpace::TAffinePoint p1(0.20,0.10); hoppe.addPoint(&p1);
	TSpace::TAffinePoint p2(0.30,0.15); hoppe.addPoint(&p2);
	TSpace::TAffinePoint p3(0.70,0.12); hoppe.addPoint(&p3);
	TSpace::TAffinePoint p4(0.75,0.14); hoppe.addPoint(&p4);
	TSpace::TAffinePoint p5(0.82,0.08); hoppe.addPoint(&p5);
	TSpace::TAffinePoint p6(0.87,0.22); hoppe.addPoint(&p6);
	TSpace::TAffinePoint p7(0.21,0.32); hoppe.addPoint(&p7);
	TSpace::TAffinePoint p8(0.14,0.45); hoppe.addPoint(&p8);*/

	/*TSpace::TAffinePoint *p1=new TSpace::TAffinePoint(0.30,0.20); hoppe.addPoint(p1);
	TSpace::TAffinePoint *p2=new TSpace::TAffinePoint(0.60,0.15); hoppe.addPoint(p2);
	TSpace::TAffinePoint *p3=new TSpace::TAffinePoint(0.80,0.20); hoppe.addPoint(p3);
	TSpace::TAffinePoint *p4=new TSpace::TAffinePoint(0.40,0.30); hoppe.addPoint(p4);
	TSpace::TAffinePoint *p5=new TSpace::TAffinePoint(0.70,0.40); hoppe.addPoint(p5);*/

	float t = 2*3.1416/1000.0;
	for(int i=0; i<1000; i++) {
		float x=t*i;
		float y=x;
		x= cos(x);y=sin(y);
		TSpace::TAffinePoint *p=new TSpace::TAffinePoint((x+1)/2.0,(y+1)/2.0);		
		hoppe.addPoint(p);
	}

	hoppe.calculateTangentPlanes();
	hoppe.doContourTracing();

}