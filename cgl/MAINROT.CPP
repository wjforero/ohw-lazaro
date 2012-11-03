#include "algorithms/delaunay.h"
#include <fstream>

/** matrices de transformacion
 */
float transformaciones[24][9]={
/*00*/  { 1, 0, 0 ,0, 1, 0 ,0, 0, 1 },
/*01*/  { 1, 0, 0 ,0, 0,-1 ,0, 1, 0 },
/*02*/  { 0, 0,-1 ,0, 1, 0 ,1, 0, 0 },
/*03*/  { 0,-1, 0 ,1, 0, 0 ,0, 0, 1 },
/*04*/  { 1, 0, 0, 0,-1, 0, 0, 0,-1 },
/*05*/  { 0, 0,-1,-1, 0, 0, 0, 1, 0 },
/*06*/  { 0,-1, 0, 0, 0,-1, 1, 0, 0 },
/*07*/  { 0,-1, 0, 0, 0,-1, 1, 0, 0 },
/*08*/  {-1, 0, 0, 0, 1, 0, 0, 0,-1 },
/*09*/  { 0, 0,-1, 1, 0, 0, 0,-1, 0 },
/*10*/  { 0, 0, 1, 1, 0, 0, 0, 1, 0 },
/*11*/  { 0,-1, 0, 0, 0,-1, 1, 0, 0 },
/*12*/  {-1, 0, 0, 0,-1, 0, 0, 0, 1 },
/*13*/  { 1, 0, 0, 0, 0, 1, 0,-1, 0 },
/*14*/  { 0, 0,-1, 0,-1, 0,-1, 0, 0 },
/*15*/  { 0,-1, 0,-1, 0, 0, 0, 0,-1 },
/*16*/  { 0,-1, 0,-1, 0, 0, 0, 0,-1 },
/*17*/  {-1, 0, 0, 0, 0, 1, 0, 1, 0 },
/*18*/  { 0, 0,-1, 0, 1, 0, 1, 0, 0 },
/*19*/  { 0, 1, 0,-1, 0, 0, 0, 0, 1 },
/*20*/  { 0, 0, 1, 0,-1, 0, 1, 0, 0 },
/*21*/  {-1, 0, 0, 0, 0, 1, 0, 1, 0 },
/*22*/  {-1, 0, 0, 0, 0,-1, 0,-1, 0 },
/*23*/  { 0,-1, 0,-1, 0, 0, 0, 0,-1 }
};
/** vertices del cubo unitario
 */
float tablaVertices[8][3]={
                    {-1,-1,-1},
                    {-1,-1,1},
                    {-1,1,-1},
                    {-1,1,1},
                    {1,-1,-1},
                    {1,-1,1},
                    {1,1,-1},
                    {1,1,1}
                      };
enum EVertices {ldb=0,ldf,lub,luf,rdb,rdf,rub,ruf};

/** aristas del cubo unitario
 */
float tablaLados[12][3]={{-1,-1,0},
                         {-1,1,0},
                         {-1,0,-1},
                         {-1,0,1},
                         {1,-1,0},
                         {1,1,0},
                         {1,0,-1},
                         {1,0,1},
                         {0,-1,-1},
                         {0,-1,1},
                         {0,1,-1},
                         {0,1,1},
                          };
enum ELados {ld=0,lu,lb,lf,rd,ru,rb,rf,db,df,ub,uf};

/** vertices de los casos basicos
 */
int casosvert[16][5]= {
/*00*/ {-1,-1,-1,-1},
/*01*/ {ldf,-1,-1,-1},
/*02*/ {ldf,rdf,-1,-1},
/*03*/ {ldf,ruf,-1,-1},
/*04*/ {ldf,rub,-1,-1},
/*05*/ {rdf,rdb,ldb,-1},
/*06*/ {ldf,rdf,rub,-1},
/*07*/ {luf,rub,rdf,-1},
/*08*/ {ldb,ldf,rdb,rdf,-1},
/*09*/ {lub,ldb,ldf,rdb,-1},
/*10*/ {luf,ldf,rub,rdb,-1},
/*11*/ {ldf,ldb,rdb,rub,-1},
/*12*/ {luf,ldb,rdb,rdf,-1},
/*13*/ {lub,ldf,ruf,rdb,-1},
/*14*/ {lub,ldb,rdb,rdf,-1}
};                     
/** triangulos de los casos basicos
 */
int casostrian[16][13] = {
/*00*/ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
/*01*/ {df,lf,ld,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
/*02*/ {ld,rd,rf,ld,rf,lf,-1,-1,-1,-1,-1,-1,-1},
/*03*/ {df,lf,ld,rf,ru,uf,-1,-1,-1,-1,-1,-1,-1},
/*04*/ {df,lf,ld,rb,ub,ru,-1,-1,-1,-1,-1,-1,-1},
/*05*/ {lb,rb,rf,ld,lb,rf,ld,rf,df,-1,-1,-1,-1},
/*06*/ {ld,rd,rf,ld,rf,lf,rb,ub,ru,-1,-1,-1,-1},
/*07*/ {rb,ub,ru,df,rd,rf,lf,uf,lu,-1,-1,-1,-1},
/*08*/ {lf,lb,rb,lf,rb,rf,-1,-1,-1,-1,-1,-1,-1},
/*09*/ {lu,ub,lf,lf,ub,df,df,ub,rb,df,rb,rd,-1},
/*10*/ {ld,df,uf,uf,lu,ld,db,ub,ru,ru,rd,db,-1},
/*11*/ {df,ru,rd,lf,lb,df,lb,ru,df,lb,ub,ru,-1},
/*12*/ {lu,lf,uf,lb,rb,rf,ld,lb,rf,ld,rf,df,-1},
/*13*/ {lf,ld,df,db,rb,rd,rf,ru,uf,lu,ub,lb,-1},
/*14*/ {lu,ub,rb,lu,rb,df,df,rb,rf,ld,lu,df,-1}
};                                         

/** vertices de los casos de reflexion
 */
int casosvert1[16][5]= {
/*00*/ {-1,-1,-1,-1},
/*01*/ {ldf,-1,-1,-1},
/*02*/ {ldb,rdb,-1,-1},
/*03*/ {ldb,rub,-1,-1},
/*04*/ {ldf,rub,-1,-1},
/*05*/ {ruf,rub,lub,-1},
/*06*/ {luf,lub,rdf,-1},
/*07*/ {lub,rdb,rdf,-1},
/*08*/ {ldb,ldf,rdb,rdf,-1},
/*09*/ {lub,ldb,ldf,rdb,-1},
/*10*/ {luf,ldf,rub,rdb,-1},
/*11*/ {ldf,ldb,rdb,rub,-1},
/*12*/ {luf,ldb,rdb,rdf,-1},
/*13*/ {lub,ldf,ruf,rdb,-1},
/*14*/ {lub,ldb,rdb,rdf,-1}
};                     
/** triangulos de los casos de reflexion
 */
int casostrian1[16][13] = {
/*00*/ {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
/*01*/ {df,lf,ld,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
/*02*/ {ld,lb,rb,ld,rb,rd,-1,-1,-1,-1,-1,-1,-1},
/*03*/ {ld,lb,db,ub,ru,rb,-1,-1,-1,-1,-1,-1,-1},
/*04*/ {df,lf,ld,rb,ub,ru,-1,-1,-1,-1,-1,-1,-1},
/*05*/ {lb,rf,rb,lu,rf,lb,lu,uf,rf,-1,-1,-1,-1},
/*06*/ {df,rd,rf,lf,uf,ub,lf,ub,lb,-1,-1,-1,-1},
/*07*/ {lu,ub,lb,df,db,rb,df,rb,rf,-1,-1,-1,-1},
/*08*/ {lf,lb,rb,lf,rb,rf,-1,-1,-1,-1,-1,-1,-1},
/*09*/ {lu,ub,lf,lf,ub,df,df,ub,rb,df,rb,rd,-1},
/*10*/ {ld,df,uf,uf,lu,ld,db,ub,ru,ru,rd,db,-1},
/*11*/ {df,ru,rd,lf,lb,df,lb,ru,df,lb,ub,ru,-1},
/*12*/ {lu,lf,uf,lb,rb,rf,ld,lb,rf,ld,rf,df,-1},
/*13*/ {lf,ld,df,db,rb,rd,rf,ru,uf,lu,ub,lb,-1},
/*14*/ {lu,ub,rb,lu,rb,df,df,rb,rf,ld,lu,df,-1}
};                                         
                                           
/** carga la transformacion i en la matriz m
 */
void cargarTransformacion(int &i, Matrix<float>& m){
  m.newsize(3,3);
  int j=0,k=0,l=0;
  for(j=0; j<3; j++){
    for(k=0; k<3; k++) {
      m[j][k] = transformaciones[i][l];
      l++;
    }
  }
}
/** carga el vertice i en la columna j de la matrix vert
 */
void cargarVertice(int&i, Matrix<float>& vert, int& j) {
  vert[0][j] = tablaVertices[i][0];
  vert[1][j] = tablaVertices[i][1];
  vert[2][j] = tablaVertices[i][2];
}
/** carga el vertice i en la columna j de la matrix vert
 */
void cargarLado(int&i, Matrix<float>& lados, int& j) {
  lados[0][j] = tablaLados[i][0];
  lados[1][j] = tablaLados[i][1];
  lados[2][j] = tablaLados[i][2];
}
/** devuelve el indice del vertice de la columna i de la matrix vert
 */
int buscarVertice(int& i, Matrix<float>& vert) {
  for(int j=0; j<8; j++) {
    if(tablaVertices[j][0] == vert[0][i] &&
      tablaVertices[j][1] == vert[1][i] &&
      tablaVertices[j][2] == vert[2][i])
      return j;
  }
  return -1;
}
/**
 */
int buscarLado(int& i, Matrix<float>& lados) {
  for(int j=0; j<12; j++) {
    if(tablaLados[j][0] == lados[0][i] &&
      tablaLados[j][1] == lados[1][i] &&
      tablaLados[j][2] == lados[2][i])
      return j;
  }
  return -1;
}
/**
 */
void codificarCaso(int& caso, Matrix<int>& casos) {
  int i,j,nvertices,nlados;
  unsigned char index;
  Matrix<float> transform;
  Matrix<float> vert;
  Matrix<float> lados;
  Matrix<float> newvert;
  Matrix<float> newlados;
  
  // se identifica el indice del caso y el numero de vertices
  //cout<<"se identifica el indice del caso y el numero de vertices"<<endl;
  nvertices=0; j=0;
  while(casosvert[caso][nvertices]!=-1) {
    nvertices++;
  }
  vert.newsize(nvertices,3); 
  while(casosvert[caso][j]!=-1) {
    cargarVertice(casosvert[caso][j],vert,j);
    j++;
  }
  // se identifica el indice del caso y el numero de vertices
  //cout<<"se identifica el indice del caso y el numero de vertices"<<endl;
  nlados=0; j=0;
  while(casostrian[caso][nlados]!=-1) {
    nlados++;
  }
  lados.newsize(nlados,3); 
  while(casostrian[caso][j]!=-1) {
    cargarLado(casostrian[caso][j],lados,j);
    j++;
  }
  
  //se crean los casos de simetria rotacional
  //cout<<"se crean los casos de simetria rotacional"<<endl;
  for(i=0; i<24; i++) {
    // se carga la transformacion
    //cout<<"se carga la transformacion"<<endl;
    cargarTransformacion(i,transform);
    
    // se transforman los vertices
    //cout<<"se transforman los vertices"<<endl;
    Matrix<float>::multiply( transform, vert, newvert);
    Matrix<float>::multiply( transform, lados, newlados);
    
    index = 0x00;
    for(j=0; j<nvertices; j++){
      int v = buscarVertice(j,newvert);
      if(v>=0)
        index = index | 0x01<<v;
      else {
        cout<<"error"<<endl;
        //index=-1; break;
      }
      //cout<<" - "<<buscarVertice(j,newvert);
    } 
    //cout<<" - indice: "<<(unsigned short)index<<endl;
    //cout<<" - indice: "<<(unsigned short)(unsigned char)(~index)<<endl;
    if(index!=-1)
      for(j=0; j<nlados; j++){
        //cout<<" - "<<buscarLado(j,newlados);
        casos[index][j]=buscarLado(j,newlados);
      }// cout<<endl;
      for(j=nlados-1; j>=0; j--){
        //cout<<" - "<<buscarLado(j,newlados);
        casos[(unsigned char)(~index)][j]=buscarLado(j,newlados);
      }// cout<<endl;
    
  }
}
/**
 */
void codificarCaso1(int& caso, Matrix<int>& casos) {
  int i,j,nvertices,nlados;
  unsigned char index;
  Matrix<float> transform;
  Matrix<float> vert;
  Matrix<float> lados;
  Matrix<float> newvert;
  Matrix<float> newlados;
  
  // se identifica el indice del caso y el numero de vertices
  //cout<<"se identifica el indice del caso y el numero de vertices"<<endl;
  nvertices=0; j=0;
  while(casosvert1[caso][nvertices]!=-1) {
    nvertices++;
  }
  vert.newsize(nvertices,3); 
  while(casosvert1[caso][j]!=-1) {
    cargarVertice(casosvert1[caso][j],vert,j);
    j++;
  }
  // se identifica el indice del caso y el numero de vertices
  //cout<<"se identifica el indice del caso y el numero de vertices"<<endl;
  nlados=0; j=0;
  while(casostrian1[caso][nlados]!=-1) {
    nlados++;
  }
  lados.newsize(nlados,3); 
  while(casostrian1[caso][j]!=-1) {
    cargarLado(casostrian1[caso][j],lados,j);
    j++;
  }
  
  //se crean los casos de simetria rotacional
  //cout<<"se crean los casos de simetria rotacional"<<endl;
  for(i=0; i<24; i++) {
    // se carga la transformacion
    //cout<<"se carga la transformacion"<<endl;
    cargarTransformacion(i,transform);
    
    // se transforman los vertices
    //cout<<"se transforman los vertices"<<endl;
    Matrix<float>::multiply( transform, vert, newvert);
    Matrix<float>::multiply( transform, lados, newlados);
    
    index = 0x00;
    for(j=0; j<nvertices; j++){
      int v = buscarVertice(j,newvert);
      if(v>=0)
        index = index | 0x01<<v;
      else {
        cout<<"error"<<endl;
        //index=-1; break;
      }
      //cout<<" - "<<buscarVertice(j,newvert);
    } 
    //cout<<" - indice: "<<(unsigned short)index<<endl;
    //cout<<" - indice: "<<(unsigned short)(unsigned char)(~index)<<endl;
    if(index!=-1)
      for(j=0; j<nlados; j++){
        //cout<<" - "<<buscarLado(j,newlados);
        casos[index][j]=buscarLado(j,newlados);
      }// cout<<endl;
      for(j=nlados-1; j>=0; j--){
        //cout<<" - "<<buscarLado(j,newlados);
        casos[(unsigned char)(~index)][j]=buscarLado(j,newlados);
      }// cout<<endl;
    
  }
}

void main() {
  int i,j;
  Matrix<int> casos;
  casos.newsize(16,256);
  for(i=0; i<256; i++) {
    for(j=0; j<16; j++) {
      casos[i][j] = -1;
    }
  }
  for(i=0; i<15; i++) {
    cout<<"caso "<<i<<endl;
    codificarCaso(i,casos);
    codificarCaso1(i,casos);
  }
  ofstream myOutput;
  myOutput.open("cases.h");
  for(i=0; i<256; i++) {
    myOutput<<"{";
    for(j=0; j<16; j++) {
      if(casos[i][j]<10 && casos[i][j]>-1)
        myOutput<<" ";
      myOutput<<casos[i][j];
      if(j<15)
        myOutput<<",";
    }
    myOutput<<"}, /*caso "<<i<<"*/"<<endl;
  }
  myOutput.close();
}