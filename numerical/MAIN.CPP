#include "numericalvector.h"
#include "matrix.h"
#include "algebra.h"
using namespace ohw::numerical;

void main() {
	cout<<"Hola mundo!!!"<<endl;
	Algebra<float>::TMatrix a(2,2);
	a(1,1)=14; a(1,2)=3;
	a(2,1)=3;  a(2,2)=9;
	Algebra<float>::TMatrix v(2,2);
	Algebra<float>::TVector d(3);
	d[0]=0;
	int nrot;
	
	Algebra<float>::jacobi(a,d,v,nrot,2);
	cout<<"a: "<<endl<<a<<endl;
	cout<<"v: "<<endl;
	cout<<v<<endl;
	cout<<"d: "<<d<<endl;
	cout<<"nrot: "<<nrot<<endl;

	
}