/** Ejemplifica el uso de la libreria para reconstruir con slices
*/
#include "reconstruction/slices/slicereconstruction.h"
#include "kernel/affinespace.h"
#include <fstream>

using namespace ohw::cgl;

// Construye el tipo de espacio vectorial
typedef CGAffineSpace<float,3> TSpace;

/** Lee datos desde cualquier fuente
*/
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
	for(int aFil=0;aFil<size;aFil++){
		for(int bCol=0;bCol<dim;bCol++){
			fscanf(in,"%f",&value);
      (*mdata)[aFil][bCol]=value;			
		}		
	}	
	fclose(in);	
}

/** Carga los slices para la reconstrucción
*/
void loadSliceSliceReconstruction(char* argv,Slice<TSpace>& s){
  typedef float TDatatype;
	int dim=0;
  int size=0;
  float** data;
  int a,j;  

	typedef CGAffineSpace<TDatatype,3> AffineSpace;
  AffineSpace as;
  readFileNativ(argv,size,dim,&data);
	
  vector<AffineSpace::TAffinePoint*> vp;
  
  s.newSize(size);
	for(a=0;a<size;a++){
    AffineSpace::TAffinePoint* pt=new AffineSpace::TAffinePoint();
    
    for(j=0;j<pt->getDim();j++)
    {
       (*pt)[j]=data[a][j];
    }    
    vp.push_back(pt);
    s.setPoint(pt,a);
  }
}

/*
// Carga slices para reconstruye
int main(int argc, char *argv[]){
  
  if(argc!=5){
    cout<<" slice1.m3d slice2.m3d slice3.m3d salida.m3d"<<endl;
    return;
  }

  Slice<TSpace> s1;
  Slice<TSpace> s2;
  Slice<TSpace> s3;
  loadSliceSliceReconstruction(argv[1],s1);
  loadSliceSliceReconstruction(argv[2],s2);  
  loadSliceSliceReconstruction(argv[3],s3);  


  SliceReconstruction<TSpace> sre(2);
  sre.addSliceInLevel(&s1,0);
  sre.addSliceInLevel(&s2,0);
  sre.addSliceInLevel(&s3,1);
  
  sre.reconstruct();

  fstream fs(argv[4],ios::out);
  fs<<sre;  
}*/

int main(int argc, char *argv[]){  
  if(argc!=4){
    cout<<" slice1.m3d slice2.m3d  salida.m3d"<<endl;
    return 0;
  }

  Slice<TSpace> s1;
  Slice<TSpace> s2;
  loadSliceSliceReconstruction(argv[1],s1);
  loadSliceSliceReconstruction(argv[2],s2);  


  SliceReconstruction<TSpace> sre(2);
  sre.addSliceInLevel(&s1,0);
  sre.addSliceInLevel(&s2,1);
  
  sre.reconstruct();

  fstream fs(argv[3],ios::out);
  fs<<sre;  
}

