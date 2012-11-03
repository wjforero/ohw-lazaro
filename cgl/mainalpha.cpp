/** Ejemplo de uso de la libreria
*/
// Incluye el alpha shape
#include "reconstruction/alphashapes/alphashape.h"
#include "algorithms/utils.h"
#include <fstream>

// Permite trabajar en el espacio de nombres de cgl
using namespace ohw::cgl;

typedef CGAffineSpace<float,3> TSpace;
typedef SimplexAlpha<TSpace>* IterSimplexAlpha;

/** Lee los datos nativos generalmente esta función
*/
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

/** Pasos para construir un alpha shape
*		- Constuir un espacio afin
*		- Crear un alpha-shape con este espacio afin
*		- Insertar cada uno de los puntos
*		- Calcular los intervalos alpha
*		- Serializar la salida
*/
int mainAlpha3D(int argc, char *argv[]){
	// Define el tipo de dato sobre el que se va a trabajar
  typedef float TDatatype;
	int dim=0;
  int size=0;
  TDatatype** data;
  int a,j;


  if(argc!=3){
    cout<<" cgl nombre.m3d dimension "<<endl;
    return 0;
  }

	// Construye el espacio sobre el cual se realizaran las operaciones 
	typedef CGAffineSpace<TDatatype,3> AffineSpace;
	// Instancia un espacio de trabajo
  AffineSpace as;
	// Construye un alpha shape que trabaja sobre este espacio
  AlphaShape<AffineSpace> alphaShape;
	// Lee los datos posiblemente de cualquier fuente
  readFileNativ(argv,size,dim,&data);
	
	// Crea un vector de puntos afines
  vector<AffineSpace::TAffinePoint*> vp;
  
	// Carga los datos al punto afin
	for(a=0;a<size;a++){
    AffineSpace::TAffinePoint* pt=new AffineSpace::TAffinePoint();
    
    for(j=0;j<pt->getDim();j++)
    {
       (*pt)[j]=data[a][j];
    }    
    vp.push_back(pt);
  }

	// Baraja los puntos para garantizar que no se tenga una estructura
  shuffle(vp,1);

	// Inserta los puntos en forma incremental
  for(a=0;a<vp.size();a++){
		if(a%10==0)
			cout<<endl<<"Proceso hasta el momento "<<a<<"..."<<endl;    
    alphaShape.insertPoint(vp[a]);
	}

	// Realiza el calculo del alpha
  alphaShape.alpha();

	// Escribe en la salida los valores
  int cont;
  do{
		float alpha=0;
		// Realiza el calculo del alpha shape para distintos valores
		cout<<"Escriba el valor del alpha ";
		cin>>alpha;		
    alphaShape.setValAlpha(alpha);
    fstream fs("sali.m3d",ios::out);
		// Serializa directamente a un stream
    fs<<alphaShape;
		cout<<"Desea continuar [no-0] ";
		cin>>cont;	
	}while(cont!=0);
  
	// Elimina los datos
  for(a=0;a<size;a++){
		delete data[a];	
	}
  delete data;
  return 1;
}

int mainAlpha3D(int argc, char *argv[]){
  return mainAlpha(argc,argv);  
}

