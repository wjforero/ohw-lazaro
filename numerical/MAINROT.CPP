#include "algorithms/delaunay.h"

 


float tablaVertices[8][3]={{-1,-1,-1},
                    {-1,-1,1},
                    {-1,1,-1},
                    {-1,1,1},
                    {1,-1,-1},
                    {1,-1,1},
                    {1,1,-1},
                    {1,1,1}
                      };

enum EVertices{
ldb,ldf,lub,luf,rdb,rdf,rub,ruf
};

enum ELados{
ld,lu,lb,lf,rd,ru,rb,rf,db,df,uv,uf
};

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

float t1[8][9]={{1, 0, 0, 0, 1, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, -1, 0, 1, 0},
                {1, 0, 0, 0, 0, 1, 0, -1, 0},              
              {1, 0, 0, 0, -1, 0, 0, 0, -1},
              {0, 0, 1, 0, 1, 0, -1, 0, 0},
              {-1, 0, 0, 0, 1, 0, 0, 0, -1},
              {0, 0, 1, -1, 0, 0, 0, -1, 0},              
              {0, 0, -1, 0, -1, 0, -1, 0, 0}};




void rellena(float*f, Matrix<float>& m, int  nh){
  int ik=0;  
  m.newsize(3,nh);
  for(int i=0;i<m.getHeight();i++)
    for(int j=0;j<m.getWidth();j++){
      m[i][j]=f[ik];
      ik++;
    }
}

void rellenavertices(int* index,Matrix<float>& m,int size){
  m.newsize(3,size);  
  for(int i=0;i<size;i++){
    m[i][0]=tablaVertices[index[i]][0];
    m[i][1]=tablaVertices[index[i]][1];
    m[i][2]=tablaVertices[index[i]][2];
  }
}

void rellenatriangulos(int* index,Matrix<float>& m,int rows){
  m.newsize(3,rows*3);
  int h=0;
  int vh=0;
  for(int i=0;i<rows;i++){
    for(int j=0;j<3;j++){            
      m[h][0]=tablaLados[index[h]][0];
      m[h][1]=tablaLados[index[h]][1];
      m[h][2]=tablaLados[index[h]][2];      
      h++;
    } 
    
  }
}

int getIndexVertice(float iv,float jv,float kv){
  for(int i=0;i<8;i++){
    if((iv==tablaVertices[i][0]) && (jv==tablaVertices[i][1]) && (kv==tablaVertices[i][2])){
      return i;
    }
  }
  return -1;
}

int getIndexLados(float iv,float jv,float kv){
  for(int i=0;i<12;i++){
    if((iv==tablaLados[i][0]) && (jv==tablaLados[i][1]) && (kv==tablaLados[i][2])){
      return i;
    }
  }
  return -1;
}


void marchingtable(int* c1v,int* c1t,int sizepuntos,int sizetri){
  Matrix<float> T(3,3);  
  Matrix<float> Ppuntos(3,4);  
  Matrix<float> Ptriangulos(3,4);  
  Matrix<float> R(3,4),RT;    
  int ij;  
  rellenavertices(c1v,Ppuntos,sizepuntos);      
  rellenatriangulos(c1t,Ptriangulos,sizetri);
  /*cout<<"R es "<<endl<<Ppuntos<<endl;
  cout<<"RT es "<<endl<<Ptriangulos<<endl;*/
  for(int k=0;k<=0;k++){
    for(int i=0;i<8;i++){   
      unsigned char temp=0;
      rellena(t1[i],T,3);      
      Matrix<float>::multiply(Ppuntos,T,R);      
      Matrix<float>::multiply(Ptriangulos,T,RT);
      
      for(ij=0;ij<sizepuntos;ij++){
        temp=temp|(1<<getIndexVertice(R[ij][0],R[ij][1],R[ij][2]));        
      }
      cout<<"Esta es la fila "<<(unsigned int)temp<<endl;

      for(ij=0;ij<sizetri*3;ij++){
        cout<<"Este el número de la columna"<<getIndexLados(RT[ij][0],RT[ij][1],RT[ij][2])<<endl;
      }
    }
  }  
};


void main(){
  int c1v[2]={ldf,rdf};
  int c1t[6]={lf,rd,rf,
             lf,ld,rd};  

  marchingtable(c1v,c1t,2,2);

  /*int c1v[2]={ldf,rdf};
  int c1t[6]={lf,rd,rf,
             lf,ld,rd};  
  marchingtable(c1v,c1t,2,2);*/
}