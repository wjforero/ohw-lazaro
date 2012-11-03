/** Ejemplifica el uso de la libreria para reconstruir con kohonen
*/
#include <iostream>
using namespace std;
#include "reconstruction/kohonen/kohonenreconstruction.h"
#include "kernel/affinespace.h"
#include <fstream>

using namespace ohw::cgl;

/** Carga cualquier tipo de datos
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

/** Pasos para la reconstrucción con kohonen
*		- Crear el espacio afin sobre el cual se trabaja
*		- Crear la reconstrucción
*		- Extraer los datos de memoria y almacenarlos en estructuras geometricas
*/
void loadDataReconstruction(char* argv){
	// Declara el tipo de dato a usar
  typedef float TDatatype;
	int dim=0;
  int size=0;
  float** data;
  int a,j;  

	// Construye el espacio afin sobre el cual se va a trabajar	
	typedef CGAffineSpace<TDatatype,3> AffineSpace;
	// Instancia el espacio afin
  AffineSpace as;
  
	// Lee los datos
  readFileNativ(argv,size,dim,&data);
	
  vector<AffineSpace::TAffinePoint*> vp;
  
	// Inserta los puntos a un vector
	for(a=0;a<size;a++){    
    AffineSpace::TAffinePoint* pt=new AffineSpace::TAffinePoint();    
    for(j=0;j<pt->getDim();j++){
       (*pt)[j]=data[a][j];
    }
    vp.push_back(pt);
  }
  cout<<"Crea la reconstruccion "<<endl;
	// Construye la reconstrucción
  KohonenReconstruction<AffineSpace> kr;  
	// Inserta la reconstrucción directamente
  kr.reconstructDefaultParameters(vp,1000);
  cout<<"Termina la recosntruccion "<<endl;
  vector<AffineSpace::TAffinePoint*> vpr;
  unsigned int* sali;
  unsigned int tama;
	// Obtiene la salida y la almacena en un vector
  kr.getResult(vpr,&sali,tama);
	// Estos puntos pueden posiblemente serializarse
}

int main(int argc, char *argv[]){  
  if(argc!=2){
    cout<<" points.m3d "<<endl;
    return 0;
  }
  loadDataReconstruction(argv[1]);
  return 1;
}