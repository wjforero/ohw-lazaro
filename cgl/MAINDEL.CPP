/** Ejemplifica el uso de la libreria para realizar triangulaciones de Delaunay
*/
#include "algorithms/delaunay.h"
#include "kernel/affinespace.h"

using namespace ohw::cgl;

/** Carga los datos desde diversas fuentes
*/
void readFileNativ(char* archiv, int& _size,int &_dim,float*** mdata){
  FILE *in;
	int size,dim;
	cout<<"El archivo es "<<archiv<<"\n";
	if((in=fopen (archiv,"r"))==NULL){
		cout<<"No se pudo abrir el archivo de entrada\n";
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

/** Pasos para el calculo de la triangulación de Delaunay
*		- Construya el espacio afin sobre el cual se va a trabajar
*		- Cree una triangulación de Delaunay sobre este espacio
*		- Inserte cada uno de los puntos
*		- Serialize la salida
*/
int mainDelaunay(int argc, char *argv[]){
	// Construye el tipo de dato
  typedef float TDatatype;
	int dim=0;
  int size=0;
  float** data;
  int a,j;

  if(argc==1){
    cout<<"no hay parametros "<<endl;
    return 0;
  }

	// Construye el espacio afin sobre el cual se va a trabajar
	typedef CGAffineSpace<TDatatype,3> AffineSpace;
	// instancia el espacio afin
  AffineSpace as;
	// construye la triangulación de Delaunay
  Delaunay<AffineSpace> del;  
	// Carga los datos desde cualquier fuente
  readFileNativ(argv[1],size,dim,&data);
  
	// Carga los datos a un punto y los inserta directamente a la triangulación
	for(a=0;a<size;a++){    
    AffineSpace::TAffinePoint* pt=new AffineSpace::TAffinePoint();      
    for(j=0;j<pt->getDim();j++)
    {      
       (*pt)[j]=data[a][j];
    }		
		if(a%10==0)
			cout<<"\nProceso hasta el momento "<<a<<"..."<<endl;   
    del.insertPoint(pt);		    
	} 

	// Serializa la triangulación a un stream
  cout<<del;
	// Elimina los datos
  for(a=0;a<size;a++){
		delete data[a];	
	}
  delete data;
  return 1;
}


int main(int argc, char *argv[]){  
  mainDelaunay(argc,argv);  
  return 0;
}
