#include "kernel/sphere.h"
#include "kernel/affinespace.h"
#include "algorithms/alphashape.h"
#include "algorithms/simplexalpha.h"
#include <fstream>



using namespace ohw::cgl;

typedef CGAffineSpace<float,2> TSpace;
typedef SimplexAlpha<TSpace>* IterSimplexAlpha;


void readFileNativ(char *argv[], int& _size,int &_dim,float*** mdata){
  FILE *in;
	int size,dim;
	cout<<"El archivo es "<<argv[1]<<" de dimension "<<argv[2]<<"\n";
	if((in=fopen (argv[1],"r"))==NULL){
		cout<<"No se pudo abrir el archivo de entrada\n";
		exit(1);
	}
	fscanf(in,"%d %d\n",&size,&dim);
  dim=atoi(argv[2]);

  (*mdata)=new float*[size];
  for(int ik=0;ik<size;ik++){
    (*mdata)[ik]=new float[dim];
  }
	cout<<"Size "<<size<<" "<<dim<<"\n";

  float value;
	int kcont=1;
	_dim=dim;
  _size=size;
	for(int aFil=0;aFil<size;aFil++){
		for(int bCol=0;bCol<dim;bCol++){
			fscanf(in,"%f",&value);
      (*mdata)[aFil][bCol]=value;			
		}		
	}	
	fclose(in);	
}

int mainAlpha(int argc, char *argv[]){
  typedef float TDatatype;
	int dim=0;
  int size=0;
  float** data;
  int a,j;

  if(argc!=3){
    cout<<" cgl nombre.m3d dimension "<<endl;
    return 0;
  }

	typedef CGAffineSpace<TDatatype,2> AffineSpace;
  AffineSpace as;
  AlphaShape<AffineSpace> alphaShape;  
  readFileNativ(argv,size,dim,&data);
	
  
	for(a=0;a<size;a++){
    AffineSpace::TAffinePoint* pt=new AffineSpace::TAffinePoint();
    for(j=0;j<pt->getDim();j++)
    {
       (*pt)[j]=data[a][j];
    }
		//if(a%10==0)
			cout<<endl<<"Proceso hasta el momento "<<a<<"..."<<endl;
    cout<<"Inserta el punto "<<*pt<<endl;
    alphaShape.insertPoint(pt);
	}

  alphaShape.alpha();

  int cont;
  do{
		float alpha=0;
		cout<<"Escriba el valor del alpha ";
		cin>>alpha;		
    alphaShape.setValAlpha(alpha);
    fstream fs("sali.m3d",ios::out);
    fs<<alphaShape;
		cout<<"Desea continuar [no-0] ";
		cin>>cont;	
	}while(cont!=0);
  
  for(a=0;a<size;a++){
		delete data[a];	
	}
  delete data;
  return 1;
}


int main(int argc, char *argv[]){
  //SimplexAlpha<TSpace> sa;
  return mainAlpha(argc,argv);
  
}

