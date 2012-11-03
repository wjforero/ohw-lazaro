#include <iostream>
using namespace std;
#include "reconstruction/kohonen/kohonenreconstruction.h"
#include "kernel/affinespace.h"
#include <fstream>

using namespace ohw::cgl;

typedef CGAffineSpace<float,2> TSpace;

void readFileNativ(char* argv, int& _size,int &_dim,float*** mdata){
  FILE *in;
	int size,dim;
	cout<<"El archivo es "<<argv<<"\n";
	if((in=fopen (argv,"r"))==NULL){
		cout<<"No se pudo abrir el archivo de entrada "<<argv<<endl;
		exit(1);
	}
	fscanf(in,"%d %d\n",&size,&dim);  
  dim=3;

  (*mdata)=new float*[size];
  for(int ik=0;ik<size;ik++){
    (*mdata)[ik]=new float[dim];
  }
	cout<<"Size "<<size<<" "<<dim<<"\n";

  float value;
	int kcont=1;
	_dim=dim;
  _size=size;
	dim=2;
	int dimt=3;
	for(int aFil=0;aFil<size;aFil++){
		for(int bCol=0;bCol<dimt;bCol++){
			fscanf(in,"%f",&value);
			if(bCol<dim)
				(*mdata)[aFil][bCol]=value;			
		}		
	}	
	fclose(in);	
}

void loadDataReconstruction(char* argv){
  typedef float TDatatype;
	int dim=0;
  int size=0;
  float** data;
  int a,j;  

	typedef CGAffineSpace<TDatatype,2> AffineSpace;
  AffineSpace as;
  
  readFileNativ(argv,size,dim,&data);
	
  vector<AffineSpace::TAffinePoint*> vp;
    
	for(a=0;a<size;a++){    
    AffineSpace::TAffinePoint* pt=new AffineSpace::TAffinePoint();
    
    for(j=0;j<pt->getDim();j++)
    {
       (*pt)[j]=data[a][j];
    }    
    vp.push_back(pt);    
  }
  cout<<"Crea la reconstruccion "<<endl;
  KohonenReconstruction<TSpace> kr;  
  kr.reconstructDefaultParameters(vp,8000);
  cout<<"Termina la recosntruccion "<<endl;
  vector<AffineSpace::TAffinePoint*> vect;
	ofstream fs("sali.m3d");
  unsigned int* simplicial;
  unsigned int sizes;
  kr.getResult(vect,&simplicial,sizes);	
	fs<<vect.size()<<" "<<sizes<<endl;
	//cout<<"valores"<<vect.size()<<" "<<sizes<<endl;
	for(j=0;j<vect.size();j++){			
			fs.precision(20);
			//cout<<vect[j]->getData(0)<<" "<<vect[j]->getData(1)<<" 0"<<endl;
			fs<<vect[j]->getData(0)<<" "<<vect[j]->getData(1)<<" 0"<<endl;			
		}
		for(j=0;j<sizes;j++){
			fs<<simplicial[j*3]<<" "<<simplicial[j*3+1]<<" "<<simplicial[j*3+2]<<endl;			
		}
}

int main(int argc, char *argv[]){
  
  if(argc!=2){
    cout<<" points.m3d "<<endl;
    return 0;
  }
  loadDataReconstruction(argv[1]);
  return 1;
}