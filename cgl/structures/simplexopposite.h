#ifndef SIMPLEXOPPOSITE_H
#define SIMPLEXOPPOSITE_H

// Inclusiones especificas
#include "base.h"

CGL_BEGIN	
	template <class TAffineSpace>
	class Simplex;

	template <class TAffineSpace>
	class SimplexOpposite
	{
		public:
			typedef Simplex<TAffineSpace>* IterSimplex;

		public:
			/** Construye un simplex para la dimensión del espacio afín
			*/
			SimplexOpposite()
			{
				m_simplexOpposite=0;
				m_indexOpposite=0;
			}

			/** Destructor
			*/
			~SimplexOpposite()
			{

			}

			/** Fija el opuesto
			*/
			void setOpposite(IterSimplex s,const int& ind)
			{
				m_indexOpposite=(unsigned char)ind;
        m_simplexOpposite = s;
			}
			
			/** Obtiene el opuesto
			*/
			IterSimplex getSimplexOpposite()
			{
				return m_simplexOpposite;
			}

			/** Obtiene el indice del opuesto
			*/
			const unsigned int getindexPointOpposite()const
			{
				return m_indexOpposite;
			}

		private:
			// Apuntador al simplex opuesto
			IterSimplex m_simplexOpposite;
			// Indice opuesto
			unsigned char m_indexOpposite;			
	};	
CGL_END

#endif