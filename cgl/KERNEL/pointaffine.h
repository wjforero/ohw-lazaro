#ifndef POINTAFFINE_H
#define POINTAFFINE_H

// inclusiones especificas
#include "base.h"
#include "vectorvectorial.h"

CGL_BEGIN

/** Esta clase modela un punto afín
 */
template <class TAffineSpace>
class PointAffine : public TAffineSpace::TVectorialSpaceSelf::TVector {
	public:			
		/** Define el tipo de dato del espacio vectorial del cual hereda.
		 */
		typedef typename TAffineSpace::TVectorialSpaceSelf TVectorialSpace;
		/** Define el tipo de dato del espacio vectorial es decir de sus vectores.
		 */
		typedef typename TAffineSpace::TVectorialSpaceSelf::TData TData;		
		/**
		 */
		typedef typename TAffineSpace::TVectorialSpaceSelf::TVector TVectorBase;

public:
		/** Constructor por omisión
		 */
		PointAffine() : TVectorBase() {
			(*this)[TAffineSpace::dim]=1;
		}
		/** Constructor por omisión
		 */
		PointAffine(const PointAffine& p) : TVectorBase()	{				      
			(*this).copy(p);      
      (*this)[TAffineSpace::dim]=1;      
		}

	public:
		/** Operador de asignación
		 */
		PointAffine<TAffineSpace>& operator=(const VectorAssignable<TData>& x) {
			x.assign(*this);
			return *this;
		}

    void setData(const TData &val){
      TVectorBase::set(val);
      (*this)[TAffineSpace::dim]=1; 
    }

	public:
		/** Retorna la dimensión del punto
		 */
		unsigned int getDim() {
    		return VectorVectorial<TVectorialSpace>::getDim()-1;
		}

	private:
		/** Oculta el operador suma que no coinciden con el algebra del punto
		 */
		VectorExpr<BinaryOperation< TData* , TData*,DApAdd<TData>,TData>,TData>
		operator+(const PointAffine<TAffineSpace>& a) {
			return *this;
		}
		/** Oculta el operador multiplicación por binario
		 */
		VectorExpr<BinaryOperation<ExprLiteral<TData>,TData*,DApMultiply<TData>,TData>,TData>		
		operator*(const TData& a) {
  			return *this;
		}
		
};

CGL_END

#endif
