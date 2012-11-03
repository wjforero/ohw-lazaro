#include "algorithms/delaunay.h"

long factorial(int v){
  long r=1;
  for(int i=1;i<=v;i++)
    r=r*i;
  return r;
}

void siguiente_combinacion(int n,int m,int* c){
  int j=m,i;
  while(j>0){
    if(c[j-1]<n-m+j){      
      c[j-1]=c[j-1]+1;
      
      for(i=j+1; i<=m; i++){        
        c[i-1]=c[i-2]+1;
      } 
      return;
    }else{
      j--;
    }
  }
}

void combinacionSecuencia( int n, int m, int*** comb,long& sizecomb){
  long i=0;
  int* c=new int[m];
  for(i=0;i<m;i++){
    c[i]=i+1;
  }
  long ncom=factorial(n)/((factorial(n-m))*factorial(m));

  (*comb)=new int*[ncom];
  for(i=0;i<ncom;i++){
    (*comb)[i]=new int[3];
  }  
  sizecomb=ncom;
  for(i=0;i<ncom;i++){    
    for(int it=0;it<m;it++){
      (*comb)[i][it]=c[it]-1;
    }     
    siguiente_combinacion(n,m,c);
  }  
  delete c;
}


float verticesedge[12][3]={{0,0.5,1},
                        {1,0.5,1},
                        {1,0.5,0},
                        {0,0.5,0},
                
                        {0.5,0,1},
                        {0.5,0,0},
                        {1,0,0.5},
                        {0,0,0.5},

                        {0,1,0.5},
                        {0.5,1,0},
                        {1,1,0.5},
                        {0.5,1,1}

               };

int verticesindexedge[12][2]={{0,2},
                        {1,3},
                        {5,7},
                        {4,6},
                
                        {2,3},
                        {6,7},
                        {3,7},
                        {6,2},

                        {0,4},
                        {4,5},
                        {1,5},
                        {0,1}

               };


float vertices[12][3]= {{0,1,1},
                        {1,1,1},
                        {0,0,1},
                        {1,0,1},
                
                        {0,1,0},
                        {1,1,0},
                        {0,0,0},
                        {1,0,0},                        
               };

int neig[8][3]={{1,2,4},
                {0,3,5},
                {0,3,6},
                {1,2,7},
                {0,5,6},
                {4,7,1},
                {4,2,7},
                {5,1,3}
};

bool getBit(unsigned char v,int index){
  unsigned char temp=1<<index;
  return (v&temp)?true:false;
}


class triangleCube{
public:
  int p1,p2,p3;

  bool isValidTriangle(){
    //cout<<"p1-"<<p1<<" "<<" p2-"<<p2<<" p3-"<<p3<<endl;
    return p1!=p2 && p2!=p3 && p1!=p3;
  }

  bool isValidTriangleMark(unsigned char v){
    // Si los vertices son validos estan entre uno marcado y uno no marcado
    return (getBit(v,verticesindexedge[p1][0]) ^ getBit(v,verticesindexedge[p1][1]))
         &&(getBit(v,verticesindexedge[p2][0]) ^ getBit(v,verticesindexedge[p2][1]))
         &&(getBit(v,verticesindexedge[p3][0]) ^ getBit(v,verticesindexedge[p3][1]));  
  }


  bool containtPoints(unsigned char v){
    unsigned char j;
    for(j=0;j<8;j++){
      //if(getBit(v,j))
        if(getSide(j)==0){
          return true;
        }
    }
    return false;
  }

  int getSide(int in){    
    Matrix<float> m;
    m.newsize(4,4);
    m[0][0]=verticesedge[p1][0]; m[0][1]=verticesedge[p1][1]; m[0][2]=verticesedge[p1][2]; m[0][3]=1;
    m[1][0]=verticesedge[p2][0]; m[1][1]=verticesedge[p2][1]; m[1][2]=verticesedge[p2][2]; m[1][3]=1;
    m[2][0]=verticesedge[p3][0]; m[2][1]=verticesedge[p3][1]; m[2][2]=verticesedge[p3][2]; m[2][3]=1;
    
    m[3][0]=vertices[in][0];     m[3][1]=vertices[in][1];     m[3][2]=vertices[in][2];      m[3][3]=1;

//    cout<<m;
    NumericalVector<float> b(4),c(4),x(4);
    float D;
    b.setData(0);
    b[3]=1;
    Matrix<float> spanning_vectors(4);

    bool issolve=m.solveLinearSystem(b,c,x,D,spanning_vectors);    
    //cout<<m;    
    if(D==0){
      return 0;
    }else if(D<0){
      return -1;
    }else;
      return 1;
  }

  unsigned char getN(){
    unsigned char n=0;
    int j;

    for(j=0;j<8;j++){
      unsigned char temp=1;
      if(getSide(j)==1){//lado derecho
        temp=1;
        temp=1<<j;
      }else{// lado izquierdo
        temp=0;
      }
      n=n|temp;
    }
    return n;
  }

  void print(int op){
    if(op==1){
      cout<<"("<<verticesedge[p1][0]<<","<<verticesedge[p1][1]<<","<<verticesedge[p1][2]<<")";
      cout<<"("<<verticesedge[p2][0]<<","<<verticesedge[p2][1]<<","<<verticesedge[p2][2]<<")";
      cout<<"("<<verticesedge[p3][0]<<","<<verticesedge[p3][1]<<","<<verticesedge[p3][2]<<")"<<endl;
    }else{      
      cout<<"("<<verticesedge[p2][0]<<","<<verticesedge[p2][1]<<","<<verticesedge[p2][2]<<")";
      cout<<"("<<verticesedge[p1][0]<<","<<verticesedge[p1][1]<<","<<verticesedge[p1][2]<<")";
      cout<<"("<<verticesedge[p3][0]<<","<<verticesedge[p3][1]<<","<<verticesedge[p3][2]<<")"<<endl;
    }
  }
};



void printbit(unsigned char v){
  for(unsigned char j=0;j<8;j++){
    cout<<getBit(v,j);
  }
}

unsigned char getCeroBit(int index){
  unsigned char r=255,temp=1<<index;
  return ~(r&temp);
}


void numbersVX(unsigned char v,unsigned char& _v1){
  char v1=0;
  for(unsigned char j=0;j<8;j++){
    
    if(getBit(v,j)==true){
      //cout<<"El bit es j "<<(unsigned int)j<<endl;
      for(int it=0;it<3;it++){
        unsigned char temp=1<<neig[j][it];
        //cout<<"Los vecinos son "<<neig[j][it]<<endl;
        if(getBit(v,neig[j][it])==true){
          /*cout<<"Fija uno de los bits ";
          cout<<(unsigned int)getCeroBit(j)<<endl;*/
          v1=v1&getCeroBit(j);// El bit j queda en cero        
        }else{          
          //cout<<"Fija otro de los bits\n";
          v1=v1|temp;// El bit j queda en uno
        }
      }
    }
  }
  _v1=v1;
}

void main(){

 

  int ** comb;
  long sizecomb,i;
  combinacionSecuencia(12,3,&comb,sizecomb);
  // Lista con todos los posibles triangulos
  vector<triangleCube*> vt;  
  for(i=0;i<sizecomb;i++){
    triangleCube* t=new triangleCube();
    t->p1=comb[i][0];
    t->p2=comb[i][1];
    t->p3=comb[i][2];
    vt.push_back(t);
  }

  // Para cada triangulo obtengo su n
    
  

  for(unsigned char it=200;it<=200;it++){
    unsigned char vti=it,vu;
    numbersVX(vti,vu);
    /*cout<<"f--";
    printbit(it);
    cout<<endl;
    cout<<"m--";
    printbit(vu);
    cout<<endl;*/
    for(i=0;i<sizecomb;i++){
      if(vt[i]->isValidTriangle() && vt[i]->isValidTriangleMark(it)){
        unsigned char nt=vt[i]->getN();
        unsigned char ntn=~nt;
        /*cout<<"nt&"<<" (";
        printbit(nt);
        cout<<")"<<endl;
        vt[i]->print();
        /*cout<<") Vu&"<<" (";
        printbit((vu&nt));
        cout<<")"<<endl;*/
        
        if((vti&~ntn) && ((vu&ntn)==vu) && ntn!=0 && ntn!=255){
          cout<<"El triangulo "<<i<<" se inserta en "<<(unsigned int)it<<endl;
          vt[i]->print(2);
          printbit(ntn);
          cout<<endl;
          cout<<" (vu&nt) ";
          printbit((vu&ntn));     
          cout<<endl;
        }

        if((vti&~nt) && ((vu&nt)==vu) && nt!=0 && nt!=255){
            /*cout<<"t--";
            printbit(nt);
            cout<<endl;
            cout<<"Se inserta\n";*/
            cout<<"El triangulo "<<i<<" se inserta en "<<(unsigned int)it<<endl;
            vt[i]->print(1);
            printbit(nt);
            cout<<endl;
            /*cout<<"nt&"<<" (";
            printbit(nt);
            cout<<")"<<endl;
            vt[i]->print();*/
/*            cout<<" (vu&nt) ";
            printbit((vu&nt));     
            cout<<endl;
            /*printbit(vu);
            cout<<" nt";
            printbit(nt);
            cout<<" ";
            printbit(vu&nt);
            cout<<endl;*/
        }
      }
    }
  }

  vt[7]->print(1);
  cout<<"vvv"<<vt[7]->containtPoints(255)<<endl;

  for(i=0;i<sizecomb;i++){
    delete comb[i];
  }
  delete comb;
}