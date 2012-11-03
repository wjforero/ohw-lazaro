#ifndef SIMPLEX_H
#define SIMPLEX_H

// Inclusiones especificas
#include "base.h"
#include "simplexopposite.h"
#include "numericalvector.h"
using namespace ohw::numerical;

using std::vector;

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
	*
	*	@author Francisco Gómez
	*/	

  template <class TAffineSpace>
  class SimplexAlpha;

	template <class TAffineSpace>
	class Simplex
	{
		public:
      typedef TAffineSpace::TData TDataType;
			typedef Simplex<TAffineSpace> TSimplexSelf;
			typedef Simplex<TAffineSpace>* IterSimplex;
      typedef SimplexOpposite<TAffineSpace> TSimplexOpposite;
			typedef SimplexOpposite<TAffineSpace>* IterSimplexOpposite;
			typedef TAffineSpace::TAffinePoint TPoint;
			typedef TAffineSpace::TAffinePoint* IterTPoint;

		public:
			/** Construye un simplex para la dimensión del espacio afín
			*/
			Simplex():m_points(0),m_simplexOpposite(0),m_hiperplane(0)
			{        
				create();
			}

			/** Destructor
			*/
			~Simplex()
			{        
				destroy();        
			}

			/** Retorna el i-esimo vertice.
			*	@i indice del valor
			*/
			IterTPoint getVert(const unsigned int& i)
			{
				return m_points[i];
			}
			
			/**	Fija vertice p en la posición ind
			*	@param p punto a fijar
			*	@param ind el indice en el cual se insertara el punto
			*/
			void setVert(IterTPoint p,const unsigned int& i)
			{        
				m_points[i]=p;
			}
			
			/**	Fija el Peak del simplex
			*	@param p punto antiorigen
			*/
			void setPeak(IterTPoint p)
			{
				m_points[0]=p;
			}
			
			/**	Retorna el vecino opuesto al vertice ind
			*	@param i Retorna el simplex opuesto al vertice en la posición ind
			*	@return Apuntador al vertice opuesto al vertice en la posición ind
			*/
			IterSimplex getOpposite(const unsigned int& i) const
			{				
				if(m_simplexOpposite[i]!=0)
					return m_simplexOpposite[i]->getSimplexOpposite();					
				return 0;
			}


			/**	Retorna el indice del punto opuesto al vertice ind en el simplex opuesto
			*	@param ind Indice en este simplex
			*	@return vertice opuesto al indice en el simplex opuesto		
			*/
			const int indexOpposite(const unsigned int& i) const
			{        
				if(m_simplexOpposite[i]!=0)
					return m_simplexOpposite[i]->getindexPointOpposite();
				return 0;
			}
			
			
			/**	Fija las relaciones de vecindad de los simplices
			*	@param neighbour Vecino opuesto al vertice ind
			*	@param ind Indice del vertice en este simplex
			*	@param indOp Indice del vertice opuesto al vertice ind en el simplex opuesto
			*/
			void setOpposite(IterSimplex n,const unsigned int& i,const unsigned int& io)
			{        
        if(m_simplexOpposite!=NULL){          
  					m_simplexOpposite[i]->setOpposite(n,io);
        }else{          
					EXCEPTION("Falla al fijar el opuesto");
        }
			}

      /** Fija la marca.
			*	@param fija el valor de una marca
			*/
			void setMark(const bool& mark)
			{
				/*if(mark)
					m_mark=(mark|(unsigned char)1);
				else
					m_mark=(mark&((unsigned char)254));*/
        m_mark=mark;
			}

			/** Retorna el valor de la marca.
			*	@param retorna el valor de la mascara
			*/
			const bool getMark() const
			{
				//return (m_mark&1)?true:false;
        return m_mark;
			}

			/** Fija el valor de la mascara de visibilidad
			*	@param fija el valor de la mascara
			*/
			void setMarkVisible(const bool& mark)
			{
        /*if(mark)
					m_mark=(mark|(unsigned char)2);
				else
					m_mark=(mark&((unsigned char)253));*/
        markvis=mark;
			}  

			/** Retorna el valor de la mascara de visibilidad.
			*	@return true si la mascara de visibilidad esta activa, false en caso contrario.
			*/
			const bool getMarkVisible() const
			{
				//return (m_mark&2)?true:false;
        return markvis;
			}

			/** Retorna un iterador al simplex opuesto
			*	@param i indice del simplex opuesto
			*/
			IterSimplexOpposite getApOpposite(const unsigned int& i)
			{
				return m_simplexOpposite[i];
			}

			/** Retorna true si el simplex es vecino de este.
			*	@return true si el simplex es vecino de este, falso en caso contrario.
			*/
			bool isNeigth(IterSimplex s) const
			{
				bool isopposite=false;
				int i=0;
				
				for(vector<IterSimplexOpposite>::iterator iOpos=m_OppositeSimplex->begin(); iOpos!=m_OppositeSimplex->end() && iCont<m_nElements ;iOpos++)
				for(i=0;i<TAffineSpace::dim+1;i++)
				{
					if(m_simplexOpposite[i]!=0)
					{
						if(m_simplexOpposite[i]->getSimplexOpposite()==s)
							isopposite=true;
					}					
				}
				return isopossite;
			}

      /** 
      */
      unsigned int getInsertPoints()const{
        unsigned int cp=0,i=0;        
        while(i<TAffineSpace::dim+1){
          if(m_points[i]!=0){
            cp++;
          }
          i++;
        }
        return cp;
      }

      const NumericalVector<TDataType>& getHiperplane() const{        
        return m_hiperplane;
      }

      void setHiperplane(const NumericalVector<TDataType>& v){
        m_hiperplane.newSize(v.length());        
        for(int ic=0;ic<v.length();ic++){
          m_hiperplane[ic]=v.getData(ic);
        }        
      }

      void setHiperplanePoint(TPoint* p){
        Primitives<TAffineSpace>::setHiperplane(p,this);        
      }

      enum{
        mdim=TAffineSpace::dim
      };
      typedef CGAffineSpace<typename TAffineSpace::TData,mdim-1> TAfinSpaceDown;
      SimplexAlpha<TAfinSpaceDown>* sa;

		protected:
			void create()
			{
				unsigned int i=0;
				m_points=new IterTPoint[TAffineSpace::dim+1];
				m_simplexOpposite=new IterSimplexOpposite[TAffineSpace::dim+1];
				m_mark=0;        
        for(i=0;i<TAffineSpace::dim+1;i++){
					m_points[i]=0;
          m_simplexOpposite[i]=new TSimplexOpposite();
        }        
        sa=0;
        m_mark=false;
        markvis=false;
			}

      void destroy()
			{        
        unsigned int i=0;       
				delete []m_points;        
        for(i=0;i<TAffineSpace::dim+1;i++){
          delete m_simplexOpposite[i];
        }
				delete []m_simplexOpposite;
				m_points=0;
				m_simplexOpposite=0;
			}      

		private:
			// Vector con apuntadores a los puntos que conforman el simplex
			IterTPoint* m_points;
      // Apuntador al vecino
			IterSimplexOpposite* m_simplexOpposite;
			// Marca para recorrido
			bool m_mark;
      // Marca para visibilidad
      bool markvis;
      // Hiperplano soporte
      NumericalVector<TDataType> m_hiperplane;
	};

  template <class TAffineSpace>
  ostream& operator<<(ostream& os,Simplex<TAffineSpace>& s)
  {
    int i=0;
    for(i=0;i<s.getInsertPoints();i++)
    {      
      os<<*(s.getVert(i))<<endl;
    }
    os<<"Hiperplane: ";
    os<<s.getHiperplane()<<endl;
    return os;
  }

CGL_END

#endif