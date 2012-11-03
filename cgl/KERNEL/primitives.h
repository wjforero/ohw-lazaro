#ifndef PRIMITIVES_H
#define PRIMITIVES_H

// Inclusiones especificas
#include "base.h"
#include "matrix.h"
#include "../structures/simplex.h"
#include "../kernel/pointaffine.h"

CGL_BEGIN
	/**
	*	Un simplex en un espacio euclidiano d-dim es un politopo donde cada
	*	subconjunto de puntos extremos define un face
	*	Ej. los triangulos definen simplices en E2
	*			Los tetrahedros son simplices en E3
	*	En este caso el simplex se representa con una lista del puntos
	*	el primer punto es un origen, el resto de puntos forman un
	*	hiperplano, y conforman la base del simplex
	* La representación es como la de un nodo de un grafo
	*
	*	@author Francisco Gómez
	*/
	template <class TAfinSpace>
	class Primitives
	{
		public:
      typedef typename TAfinSpace::TData TDataType;
      typedef PointAffine<TAfinSpace>* IterPoint;
      typedef Simplex<TAfinSpace>* IterSimplex;

      static int getSideOfBase(IterPoint center,IterPoint p, IterSimplex s,const unsigned char& op)
      {        
        if(op==2){          
          setHiperplane(center,s);          
          if(s->getHiperplane().length()!=0){            
            TDataType res;
            res=0;
            TDataType resc;
            resc=0;
            for(int ic=0;ic<=p->getDim();ic++){              
              res+=s->getHiperplane().getData(ic)*(*p).getData(ic);              
            }

            if(res==0)
              return 0;
            if(res<0)
              return 1;
            else 
              return -1;
          }
        }

        // Construye la matriz
        unsigned int i,j,h=s->getInsertPoints();
        Matrix<TDataType> m(TAfinSpace::dim+1,h);


        // Matriz               
        for(i=0;i<s->getInsertPoints()-1;i++){
          for(j=0;j<p->getDim()+1;j++){
            m[i][j]=s->getVert(i+1)->getData(j);           
          }                  
        }

        
        for(j=0;j<p->getDim()+1;j++)
          m[h-1][j]=p->getData(j);                

        NumericalVector<TDataType> b(TAfinSpace::dim+1);
        NumericalVector<TDataType> c(TAfinSpace::dim+1),x(TAfinSpace::dim+1);
        TDataType D;
        TDataType zero;
        zero=0;
        c.setData(zero);
        Matrix<TDataType> spanning_vectors(TAfinSpace::dim+1,TAfinSpace::dim+1);
        b.setData(zero);
        b[TAfinSpace::dim]=1;        
        
        bool issolve=m.solveLinearSystem(b,c,x,D,spanning_vectors);
        if(D==0)
          return 0;
        else if(D<0)
          return 1;
        else 
          return -1;
      }

      static bool containedInBase(IterPoint p, IterSimplex s)
      {
        return false;
      }

      static void setHiperplane(IterPoint p, IterSimplex s)
      {
        // Construye la matriz
        unsigned int i,j,h=s->getInsertPoints();
        Matrix<TDataType> m(TAfinSpace::dim+1,h);


        // Matriz               
        for(i=0;i<s->getInsertPoints()-1;i++){
          for(j=0;j<p->getDim()+1;j++){
            m[i][j]=s->getVert(i+1)->getData(j);           
          }                  
        }

        
        for(j=0;j<p->getDim()+1;j++)
          m[h-1][j]=p->getData(j);                

        NumericalVector<TDataType> b(TAfinSpace::dim+1);
        NumericalVector<TDataType> c(TAfinSpace::dim+1),x(TAfinSpace::dim+1);
        TDataType D;
        TDataType zero;
        zero=0;
        c.setData(zero);
        Matrix<TDataType> spanning_vectors(TAfinSpace::dim+1,TAfinSpace::dim+1);
        b.setData(zero);
        b[TAfinSpace::dim]=1;
        x.setData(zero);
        bool issolve=m.solveLinearSystem(b,c,x,D,spanning_vectors);
        
        NumericalVector<TDataType> r(TAfinSpace::dim+1);
        r=x*(*p);        
        TDataType resul;
        resul=0;
        for(i=0;i<r.length();i++){
          resul+=r.getData(i);
        }

        if(resul<0)
          x=-x;        

        s->setHiperplane(x);        
      }

      /** Retorna true si p esta en el hull afin del plano soporte de s
      *   @param p iterador del punto
      *   @param s simplex
      */
      static bool isAffineHull(IterPoint p, IterSimplex s)
      {
        // Construye la matriz        
        unsigned int i,j,h=s->getInsertPoints();
        Matrix<TDataType> m(TAfinSpace::dim+1,h+1);

        // Matriz        
        for(i=0;i<s->getInsertPoints();i++)
          for(j=0;j<p->getDim()+1;j++){
            m[i][j]=s->getVert(i)->getData(j);
          }        

        for(j=0;j<p->getDim()+1;j++)
          m[h][j]=p->getData(j);                        

        

        NumericalVector<TDataType> b(TAfinSpace::dim+1);
        NumericalVector<TDataType> c(TAfinSpace::dim+1),x(TAfinSpace::dim+1);
        TDataType D;
        TDataType zero;
        zero=0;
        c.setData(zero);
        Matrix<TDataType> spanning_vectors(TAfinSpace::dim+1,TAfinSpace::dim+1);
        b.setData(zero);
        b[TAfinSpace::dim]=1;

        bool issolve=m.solveLinearSystem(b,c,x,D,spanning_vectors);
        return !issolve;
      }
	};	

CGL_END

#endif