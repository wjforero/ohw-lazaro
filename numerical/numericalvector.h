#ifndef NUMERICALVECTOR_H
#define NUMERICALVECTOR_H

#include "base.h"

#define EXPRVEC_USE_TEMPORARY_EXPRESSION

NUM_BEGIN

template <class TDataType>
class NumericalVector;

/** Provee una interface para modelar objetos asignables (lados derechos) de cualquier tipo
 */
template <class TDataType>
class VectorAssignable {
	public:
		/** Funci�n para la asignaci�n, solo cuando es llamada son ejecutadas las operaci�nes necesarias
		 *	@param v Vector al cual se asignaran los valores resultado de los calculos
		 */
		virtual void assign(NumericalVector<TDataType>& v) const = 0;
};

/**
 *	Representa un vector n�merico
 */
template <class TDataType>
class NumericalVector{
public:		
	/** Tipo iterador.
	*/
  typedef TDataType* iterT;
	/** Tipo de dato almacenado.
	*/
  typedef TDataType  elementT;

public:
	/** Construye un vector de tama�o n.
	*	@param n Tama�o del vector
	*/
	NumericalVector(const unsigned int& n):m_length(0){
		// Asigna memoria al vector
		initialize(n);
	}
	/** Destructor del vector.
	*/
  ~NumericalVector(){
		// Elimina el vector
		destroy();
  }
	/** Redimensiona un vector
	*	@param n nuevo tama�o
	*/
	void newSize(const unsigned int& n){
		initialize(n);
	}
	/** Operador de acceso a los datos de un arreglo.
	*	@param i Coordenada del valor accesado
	*	@return i-esimo dato
	*/
  TDataType& operator[](const unsigned int& i){
		// Retorna el i-esimo dato del vector
		return m_apdata[i];
    }
	/**
	*/
	TDataType& getData(const unsigned int& i) const {
		// Retorna el i-esimo dato del vector
		return m_apdata[i];
	}
	/** Retorna un "iterador" al primer valor del arreglo.
	*	@return Iterador inicial de los datos
	*/
	iterT begin() const{
		// Retorna un puntero al dato inicial
		return m_apdata;
	}
	/** Retorna un "iterador" que apunta al final de los datos.
	*		@return iterT Iterador al final del vector
	*/
	iterT end() const{
		// Retorna un puntero al dato final
		return m_apdata + m_length;
	}
	/** Retorna la longitud del vector.
	*	@return Longitud del vector
	*/
	unsigned int length() const{
		// Retorna el tama�o del vector
		return m_length;
	}
	/** Operador de asignaci�n cuando es llamado ejecuta la asignaci�n de x.
	*	@param x Objeto Asignable, lado derecho (Posiblimente expandido por template expresi�n)
	*	@return Referencia a la asignaci�n
	*/
  NumericalVector<TDataType>& operator=(const VectorAssignable<TDataType>& x){
		x.assign(*this);
		return *this;
	}

	/** otro
	 */
	NumericalVector<TDataType>& operator=(const NumericalVector<TDataType>& x)	{		
		(*this).copy(x);
		return *this;		
	}

  /** Llena el vector con un valor especifico.
  *   @param val Valor con el que se llena el vector.
  */
  void setData(const TDataType &val){
    set(val);  
  }

protected:
	/** Inicializa el vector con un tama�o n
	*	@param n tama�o del vector
	*/
	void initialize(const unsigned int n){
		if(m_length==n){
			return;
		}    
		m_apdata = new TDataType[n];
		CHECKMEMORY(m_apdata);
		m_length=n;	
	}
	/** Fija todas las entradas del vector con el valor val
	*	@param val valor a fijar
	*/
	void  set (const TDataType &val){
		iterT i=begin();				
		do{
			*i=val;
		}while(++i!=end());
	}
	/** Copia los datos de un vector en otro
	*	@v vector a copiar
	*/
	void copy(const NumericalVector<TDataType>& v){
		iterT i=begin();
		iterT j=v.begin();
		do{
			*i=*j;
			++j;			
		}while(++i!=end());
	}
	/** Elimina los datos del vector
	*/
	void destroy(){
		// elimina los valores
    if(m_length!=0)
		  delete []m_apdata;	
		m_apdata=0;				
	}

private:
	/** Apuntador a los datos
	 */
	TDataType* m_apdata;
	/** Longitud del vector
	 */
	unsigned int m_length;
};

/**
*	Esta clase modela la suma como una operaci�n binaria
*	TDataType Tipo
*/
template <class TDataType>
class DApAdd {
public:
	/**  Constructor por omisi�n.
	*/
	DApAdd(){
		// El constructor solo se proporciona por compatibilidad
	}
	/** Funci�n estatica que suma los valores a y b esta funci�n es finalmente.
	*		@param a Valor a a sumar
	*		@param b Valor b a sumar
	*		@param resultado de a + b
	*/
  static inline TDataType apply(const TDataType& a, const TDataType& b){
		// Retorna la suma de los dos TDataType
		return a+b;
	}
};

/** Esta clase modela la divisi�n como una operaci�n binaria
*		TDataType Tipo
*/
template <class TDataType>
class DApDivide {
public:
	/**
	*		Constructor por omisi�n
	*/
  DApDivide(){
		// Proporcionada por compatibilidad
	}
	/**	Funci�n estatica que divide los valores a y b
	*		@param a Divisor
	*		@param b Dividendo
	*		@param resultado de a/b
	*/
  	static inline TDataType apply(const TDataType& a, const TDataType& b){
		// Retorna la divisi�n de los dos TDataType
		return a/b;
	}
};

/**	Esta clase modela la resta como una operaci�n binaria
*		TDataType Tipo
*/
template <class TDataType>
class DApSubtract{
public:
	/**	Constructor por omisi�n
	*/
	  DApSubtract(){
		// No hace nada
	}
	/**	Funci�n estatica que resta los valores a y b
	*		@param a Minuendo
	*		@param b Sustraendo
	*		@param resultado de a-b
	*/
  	static inline TDataType apply(const TDataType& a, const TDataType& b){
		// Retorna la resta de los dos TDataType
		return a-b;
	}
};

/** Esta clase modela la multiplicaci�n como una operaci�n binaria
*		TDataType Tipo
*/
template <class TDataType>
class DApMultiply {

public:
	/**
	*		Constructor por omisi�n
	*/
	DApMultiply(){
		// No hace nada
	}
	/**
	*		Funci�n estatica que multiplica los valores a y b
	*		@param a
	*		@param b
	*		@param resultado de a*b
	*/
	static inline TDataType apply(const TDataType& a, const TDataType& b){
		// Retorna la multiplicaci�n de a*b
		return a*b;
	}
};

/** Esta clase modela el vector inverso como una operaci�n unaria
*		TDataType Tipo
*/
template <class TDataType>
class DApInverse {
public:
	/**
	*		Constructor por omisi�n
	*/
	DApInverse(){
		// No hace nada
	}
	/** Funci�n para encontrar el vector inverso
	*		@param a	
	*		@param resultado de -a
	*/
	static inline TDataType apply(const TDataType& a){
		// Retorna la multiplicaci�n de a*b
		return -a;
	}
};


/**	Esta la operacion identidad
*		TDataType Tipo
*/
template <class TDataType>
class DApIdentity {
public:
	/**	Constructor por omisi�n
	*/
	DApIdentity(){
		// No hace nada
	}
	/**	Funci�n para encontrar el vector inverso
	*		@param a	
	*		@param resultado de -a
	*/
	static inline TDataType apply(const TDataType& a){
		// Retorna la multiplicaci�n de a*b
		return a;
	}
};

/**	Funci�n template para la asignaci�n de un resultado
*/
template<class Iter,class DataType>
void assignResult(NumericalVector<DataType>& a, const Iter& result)
{
#ifdef EXPRVEC_USE_TEMPORARY_EXPRESSION
	// Realiza una copita temporal de un iterador. Esto es rap�do en arquitecturas
	// segmentadas, si todos los iteradores estan en el mismo segmento
	Iter result2 = result;
#else
	// Realiza un cast sobre la referencia constante
	Iter& result2 = (Iter&)result;
#endif

#ifdef EXPRVEC_USE_INDIRECTION
	// Copia los valores de los vectores
	// Obtiene una referencia al inicio del array
	Iter* iter = a.begin();
	// Declara una variable que copia los resultados
	for (register int i = a.length(); i--;)
		iter[i] = *result2;   // Expande la expresi�n
#else
	// Obtiene los iteradores inicial y final del vector
	DataType* iter = a.begin();
	DataType* end  = a.end();

	// Copia los valores
	do {		
		*iter = *result2;       // Ejecuta la expresion expandida
				++result2;
	} while (++iter != end);
#endif
}

/** Funci�n template para la asignaci�n de un resultado
*/
template<class Iter,class DataType>
void assignResultLiteral(NumericalVector<DataType>& a, const Iter& result){
#ifdef EXPRVEC_USE_TEMPORARY_EXPRESSION
	// Realiza una copita temporal de un iterador. Esto es rap�do en arquitecturas
	// segmentadas, si todos los iteradores estan en el mismo segmento
	Iter result2 = result;
#else
	// Realiza un cast sobre la referencia constante
	Iter& result2 = (Iter&)result;
#endif

#ifdef EXPRVEC_USE_INDIRECTION
	// Copia los valores de los vectores
	// Obtiene una referencia al inicio del array
	Iter* iter = a.begin();
	// Declara una variable que copia los resultados
	for (register int i = a.length(); i--;)
		iter[i] = result2[i];   // Expande la expresi�n
#else
	// Obtiene los iteradores inicial y final del vector
	DataType* iter = a.begin();
	DataType* end  = a.end();

	// Copia los valores
	do {		
		*iter = *result2;     // Expande la expresi�n		        
	} while (++iter != end);
#endif
}

/**	Esta clase modela una expresi�n de tipo Asignable
*/
template<class A,class TDataType>
class VectorExpr : public VectorAssignable<TDataType>{
public:
	VectorExpr(const A& a) : m_iter(a){
		// No hace m�s nada
	}
	TDataType operator*() const {
		// Retorna el iterador
		return *m_iter;
	}
	TDataType operator[](const unsigned int& i) const{
		// Retorna el iterador en su iesima posici�n
		return m_iter[i];
	}
	void operator++(){
		// Avanza el itierador
		++m_iter;
	}
	/** Sobreescribe la funci�n virtual de asignaci�n
	*/
	virtual void assign(NumericalVector<TDataType>& x) const{
		// Llama a la funci�n de para que evalue la expresi�n y la asigne a x
		assignResult(x,*this);
	}

private:
	A m_iter;
};

/**	Esta clase modela una expresi�n de tipo Asignable
*/
template<class TDataType>
class ExprLiteral : public VectorAssignable<TDataType>{
public:
	ExprLiteral(const TDataType& a) : m_iter(a){
		// No hace m�s nada
	}
	void operator++(){
		// Se provee por compatibilidad con los operadores binarios
	}
	TDataType operator*() const {		
		return m_iter;
	}    	    
	/**
	*	Sobreescribe la funci�n virtual de asignaci�n
	*/
	virtual void assign(NumericalVector<TDataType>& x) const{
		// Llama a la funci�n de para que evalue la expresi�n y la asigne a x
		assignResultLiteral(x,*this);
	}

private:
	TDataType m_iter;
};

/** DVBinExprOp Representa una operaci�n en dos iteradores desrreferenciados (A,B)
*/
template<class A, class B, class Op,class TDataType>
class BinaryOperation {
public:		
	BinaryOperation(const A& a, const B& b) : m_iter1(a), m_iter2(b){
		// No hace m�s nada
	}
	void operator++() {
		// Aumenta los dos iteradores
		++m_iter1; ++m_iter2;
	}
	TDataType operator*() const {
		// Aplica la operaci�n correspondiente	
		return Op::apply(*m_iter1,*m_iter2);
	}
	TDataType operator[](const unsigned int& i) const{			
			return Op::apply(m_iter1[i], m_iter2[i]);
		}
private:
	/** Iterador del operador A
	 */
	A m_iter1;
	/** Iterador del operador B
	 */
	B m_iter2;
};

/** DVBinExprOp Representa una operaci�n en dos iteradores desrreferenciados (A,B)
*/
template<class A, class Op,class TDataType>
class UnaryOperation{
public:
	UnaryOperation(const A& a) : m_iter(a){
		// No hace m�s nada
	}
	void operator++() {
		// Aumenta los dos iteradores
		++m_iter;
	}
	TDataType operator*() const {
		// Aplica la operaci�n correspondiente
		return Op::apply(*m_iter);
	}
	TDataType operator[](const unsigned int& i) const{
		return Op::apply(m_iter[i]);
	}
private:
	// Iterador del operador A
	A m_iter;
};

/** Operador de adici�n entre vectores
 */
template <class TDataType>
VectorExpr< BinaryOperation<TDataType* ,TDataType* ,DApAdd<TDataType> ,TDataType >,TDataType>
operator+(const NumericalVector<TDataType>& a, const NumericalVector<TDataType>& b){
	typedef BinaryOperation<TDataType*,TDataType*,DApAdd<TDataType>,TDataType> ExprT;
	return VectorExpr<ExprT,TDataType>(ExprT(a.begin(),b.begin()));
}

/**
 */
template<class A,class TDataType>
VectorExpr< BinaryOperation< VectorExpr<A,TDataType>,	TDataType*, DApAdd<TDataType>, TDataType>, TDataType>
operator+(const VectorExpr<A,TDataType>& a, const NumericalVector<TDataType>& b){
	typedef BinaryOperation<VectorExpr<A,TDataType>,TDataType*,DApAdd<TDataType>,TDataType> ExprT;
	return VectorExpr<ExprT,TDataType>(ExprT(a,b.begin()));
}

/**
 */
template<class A,class TDataType>
VectorExpr< BinaryOperation<TDataType*,VectorExpr<A,TDataType>,DApAdd<TDataType>,TDataType>, TDataType>
operator+(const NumericalVector<TDataType>& a, const VectorExpr<A,TDataType>& b){
	typedef BinaryOperation<TDataType*,VectorExpr<A,TDataType>,DApAdd<TDataType>,TDataType> ExprT;
	return VectorExpr<ExprT,TDataType>(ExprT(a.begin(),b));
}

/**
 */
template<class A, class B,class TDataType>
VectorExpr<BinaryOperation<VectorExpr<A,TDataType>,VectorExpr<B,TDataType>,DApAdd<TDataType>,TDataType> ,TDataType >
operator+(const VectorExpr<A,TDataType>& a, const VectorExpr<B,TDataType>& b){
	typedef BinaryOperation<VectorExpr<A,TDataType>,VectorExpr<B,TDataType>,DApAdd<TDataType>,TDataType> ExprT;
	return VectorExpr<ExprT,TDataType>(ExprT(a,b));
}

/**
*	Operadores de substracci�n entre vectores
*/
template <class TDataType>
VectorExpr< BinaryOperation<TDataType* ,TDataType* ,DApSubtract<TDataType> ,TDataType >,TDataType>
operator-(const NumericalVector<TDataType>& a, const NumericalVector<TDataType>& b){
	typedef BinaryOperation<TDataType*,TDataType*,DApSubtract<TDataType>,TDataType> ExprT;
	return VectorExpr<ExprT,TDataType>(ExprT(a.begin(),b.begin()));
}

/**
 */
template<class A,class TDataType>
VectorExpr< BinaryOperation< VectorExpr<A,TDataType>,	TDataType*, DApSubtract<TDataType>, TDataType>, TDataType>
operator-(const VectorExpr<A,TDataType>& a, const NumericalVector<TDataType>& b){
	typedef BinaryOperation<VectorExpr<A,TDataType>,TDataType*,DApSubtract<TDataType>,TDataType> ExprT;
	return VectorExpr<ExprT,TDataType>(ExprT(a,b.begin()));
}

/**
 */
template<class A,class TDataType>
VectorExpr< BinaryOperation<TDataType*,VectorExpr<A,TDataType>,DApSubtract<TDataType>,TDataType>, TDataType>
operator-(const NumericalVector<TDataType>& a, const VectorExpr<A,TDataType>& b){
	typedef BinaryOperation<TDataType*,VectorExpr<A,TDataType>,DApSubtract<TDataType>,TDataType> ExprT;
	return VectorExpr<ExprT,TDataType>(ExprT(a.begin(),b));
}

/**
 */
template<class A, class B,class TDataType>
VectorExpr<BinaryOperation<VectorExpr<A,TDataType>,VectorExpr<B,TDataType>,DApSubtract<TDataType>,TDataType> ,TDataType >
operator-(const VectorExpr<A,TDataType>& a, const VectorExpr<B,TDataType>& b){
	typedef BinaryOperation<VectorExpr<A,TDataType>,VectorExpr<B,TDataType>,DApSubtract<TDataType>,TDataType> ExprT;
	return VectorExpr<ExprT,TDataType>(ExprT(a,b));
}

/** Operadores de division entre vectores
*/

/**
 */
template <class TDataType>
VectorExpr< BinaryOperation<TDataType* ,TDataType* ,DApDivide<TDataType> ,TDataType >,TDataType>
operator/(const NumericalVector<TDataType>& a, const NumericalVector<TDataType>& b){
	typedef BinaryOperation<TDataType*,TDataType*,DApDivide<TDataType>,TDataType> ExprT;
	return VectorExpr<ExprT,TDataType>(ExprT(a.begin(),b.begin()));
}

/**
 */
template<class A,class TDataType>
VectorExpr< BinaryOperation< VectorExpr<A,TDataType>,	TDataType*, DApDivide<TDataType>, TDataType>, TDataType>
operator/(const VectorExpr<A,TDataType>& a, const NumericalVector<TDataType>& b){
	typedef BinaryOperation<VectorExpr<A,TDataType>,TDataType*,DApDivide<TDataType>,TDataType> ExprT;
	return VectorExpr<ExprT,TDataType>(ExprT(a,b.begin()));
}

/**
 */
template<class A,class TDataType>
VectorExpr< BinaryOperation<TDataType*,VectorExpr<A,TDataType>,DApDivide<TDataType>,TDataType>, TDataType>
operator/(const NumericalVector<TDataType>& a, const VectorExpr<A,TDataType>& b){
	typedef BinaryOperation<TDataType*,VectorExpr<A,TDataType>,DApDivide<TDataType>,TDataType> ExprT;
	return VectorExpr<ExprT,TDataType>(ExprT(a.begin(),b));
}

/**
 */
template<class A, class B,class TDataType>
VectorExpr<BinaryOperation<VectorExpr<A,TDataType>,VectorExpr<B,TDataType>,DApDivide<TDataType>,TDataType> ,TDataType >
operator/(const VectorExpr<A,TDataType>& a, const VectorExpr<B,TDataType>& b){
	typedef BinaryOperation<VectorExpr<A,TDataType>,VectorExpr<B,TDataType>,DApDivide<TDataType>,TDataType> ExprT;
	return VectorExpr<ExprT,TDataType>(ExprT(a,b));
}

/** Operadores de multiplicacion entre vectores
*/

/**
 */
template <class TDataType>
VectorExpr< BinaryOperation<TDataType* ,TDataType* ,DApMultiply<TDataType> ,TDataType >,TDataType>
operator*(const NumericalVector<TDataType>& a, const NumericalVector<TDataType>& b){
	typedef BinaryOperation<TDataType*,TDataType*,DApMultiply<TDataType>,TDataType> ExprT;
	return VectorExpr<ExprT,TDataType>(ExprT(a.begin(),b.begin()));
}

/**
 */
template<class A,class TDataType>
VectorExpr< BinaryOperation< VectorExpr<A,TDataType>,	TDataType*, DApMultiply<TDataType>, TDataType>, TDataType>
operator*(const VectorExpr<A,TDataType>& a, const NumericalVector<TDataType>& b){
	typedef BinaryOperation<VectorExpr<A,TDataType>,TDataType*,DApMultiply<TDataType>,TDataType> ExprT;
	return VectorExpr<ExprT,TDataType>(ExprT(a,b.begin()));
}

/**
 */
template<class A,class TDataType>
VectorExpr< BinaryOperation<TDataType*,VectorExpr<A,TDataType>,DApMultiply<TDataType>,TDataType>, TDataType>
operator*(const NumericalVector<TDataType>& a, const VectorExpr<A,TDataType>& b){
	typedef BinaryOperation<TDataType*,VectorExpr<A,TDataType>,DApMultiply<TDataType>,TDataType> ExprT;
	return VectorExpr<ExprT,TDataType>(ExprT(a.begin(),b));
}

/**
 */
template<class A, class B,class TDataType>
VectorExpr<BinaryOperation<VectorExpr<A,TDataType>,VectorExpr<B,TDataType>,DApMultiply<TDataType>,TDataType> ,TDataType >
operator*(const VectorExpr<A,TDataType>& a, const VectorExpr<B,TDataType>& b){
	typedef BinaryOperation<VectorExpr<A,TDataType>,VectorExpr<B,TDataType>,DApMultiply<TDataType>,TDataType> ExprT;
	return VectorExpr<ExprT,TDataType>(ExprT(a,b));
}

/**	Operadores unarios para vectores
*/

/**
 */
template <class TDataType>
VectorExpr< UnaryOperation<TDataType* ,DApInverse<TDataType> ,TDataType >,TDataType>
operator-(const NumericalVector<TDataType>& a){
	typedef UnaryOperation<TDataType* ,DApInverse<TDataType> ,TDataType > ExprT;
	return VectorExpr<ExprT,TDataType>(ExprT(a.begin()));
}

/**
 */
template<class A, class B,class TDataType>
VectorExpr< UnaryOperation<VectorExpr<A,TDataType>,DApInverse<TDataType>,TDataType> ,TDataType >
operator-(const VectorExpr<A,TDataType>& a){
	typedef UnaryOperation<VectorExpr<A,TDataType>,DApInverse<TDataType>,TDataType> ExprT;
	return VectorExpr<ExprT,TDataType>(ExprT(a));
}

/**	Operadores con expresiones literares
*/

/**
*	Adicion con literal
*/

/**
 */
template <class TDataType>
VectorExpr< BinaryOperation<ExprLiteral<TDataType>,TDataType*,DApAdd<TDataType>,TDataType>,TDataType>
operator+(const TDataType& a, const NumericalVector<TDataType>& b){
	typedef BinaryOperation<ExprLiteral<TDataType>,TDataType*,DApAdd<TDataType>,TDataType> ExprT;	
	return VectorExpr<ExprT,TDataType>(ExprT(ExprLiteral<TDataType>(a),b.begin()));
}

/**
 */
template <class TDataType>
VectorExpr< BinaryOperation<ExprLiteral<TDataType>,TDataType*,DApAdd<TDataType>,TDataType>,TDataType>
operator+(const NumericalVector<TDataType>& b,const TDataType& a){
	typedef BinaryOperation<ExprLiteral<TDataType>,TDataType*,DApAdd<TDataType>,TDataType> ExprT;	
	return VectorExpr<ExprT,TDataType>(ExprT(ExprLiteral<TDataType>(a),b.begin()));
}

/**
 */
template<class A,class TDataType>
VectorExpr< BinaryOperation<ExprLiteral<TDataType>,VectorExpr<A,TDataType>,DApAdd<TDataType>,TDataType>,TDataType>
operator+(const TDataType& a, const VectorExpr<A,TDataType>& b){
	typedef BinaryOperation<ExprLiteral<TDataType>,VectorExpr<A,TDataType>,DApAdd<TDataType>,TDataType> ExprT;	
	return VectorExpr<ExprT,TDataType>(ExprT(ExprLiteral<TDataType>(a),b));
}

/**
 */
template<class A,class TDataType>
VectorExpr< BinaryOperation<ExprLiteral<TDataType>,VectorExpr<A,TDataType>,DApAdd<TDataType>,TDataType>,TDataType>
operator+(const VectorExpr<A,TDataType>& b,const TDataType& a){
	typedef BinaryOperation<ExprLiteral<TDataType>,VectorExpr<A,TDataType>,DApAdd<TDataType>,TDataType> ExprT;	
	return VectorExpr<ExprT,TDataType>(ExprT(ExprLiteral<TDataType>(a),b));
}

/** Substraccion con literal
*/

/**
 */
template <class TDataType>
VectorExpr< BinaryOperation<ExprLiteral<TDataType>,TDataType*,DApSubtract<TDataType>,TDataType>,TDataType>
operator-(const TDataType& a, const NumericalVector<TDataType>& b){
	typedef BinaryOperation<ExprLiteral<TDataType>,TDataType*,DApSubtract<TDataType>,TDataType> ExprT;	
	return VectorExpr<ExprT,TDataType>(ExprT(ExprLiteral<TDataType>(a),b.begin()));
}

/**
 */
template <class TDataType>
VectorExpr< BinaryOperation<TDataType*,ExprLiteral<TDataType>,DApSubtract<TDataType>,TDataType>,TDataType>
operator-(const NumericalVector<TDataType>& b,const TDataType& a){
	typedef BinaryOperation<TDataType*,ExprLiteral<TDataType>,DApSubtract<TDataType>,TDataType> ExprT;	
	return VectorExpr<ExprT,TDataType>(ExprT(b.begin(),ExprLiteral<TDataType>(a)));
}

/**
 */
template<class A,class TDataType>
VectorExpr< BinaryOperation<ExprLiteral<TDataType>,VectorExpr<A,TDataType>,DApSubtract<TDataType>,TDataType>,TDataType>
operator-(const TDataType& a, const VectorExpr<A,TDataType>& b){
	typedef BinaryOperation<ExprLiteral<TDataType>,VectorExpr<A,TDataType>,DApSubtract<TDataType>,TDataType> ExprT;	
	return VectorExpr<ExprT,TDataType>(ExprT(ExprLiteral<TDataType>(a),b));
}

/**
 */
template<class A,class TDataType>
VectorExpr< BinaryOperation<VectorExpr<A,TDataType>,ExprLiteral<TDataType>,DApSubtract<TDataType>,TDataType>,TDataType>
operator-(const VectorExpr<A,TDataType>& b,const TDataType& a){
	typedef BinaryOperation<VectorExpr<A,TDataType>,ExprLiteral<TDataType>,DApSubtract<TDataType>,TDataType> ExprT;	
	return VectorExpr<ExprT,TDataType>(ExprT(b,ExprLiteral<TDataType>(a)));
}

/**	Mutiplicacion con literal
*/

/**
 */
template <class TDataType>
VectorExpr< BinaryOperation<ExprLiteral<TDataType>,TDataType*,DApMultiply<TDataType>,TDataType>,TDataType>
operator*(const TDataType& a, const NumericalVector<TDataType>& b){
	typedef BinaryOperation<ExprLiteral<TDataType>,TDataType*,DApMultiply<TDataType>,TDataType> ExprT;	
	return VectorExpr<ExprT,TDataType>(ExprT(ExprLiteral<TDataType>(a),b.begin()));
}

/**
 */
template <class TDataType>
VectorExpr< BinaryOperation<ExprLiteral<TDataType>,TDataType*,DApMultiply<TDataType>,TDataType>,TDataType>
operator*(const NumericalVector<TDataType>& b,const TDataType& a){
	typedef BinaryOperation<ExprLiteral<TDataType>,TDataType*,DApMultiply<TDataType>,TDataType> ExprT;	
	return VectorExpr<ExprT,TDataType>(ExprT(ExprLiteral<TDataType>(a),b.begin()));
}

/**
 */
template<class A,class TDataType>
VectorExpr< BinaryOperation<ExprLiteral<TDataType>,VectorExpr<A,TDataType>,DApMultiply<TDataType>,TDataType>,TDataType>
operator*(const TDataType& a, const VectorExpr<A,TDataType>& b){
	typedef BinaryOperation<ExprLiteral<TDataType>,VectorExpr<A,TDataType>,DApMultiply<TDataType>,TDataType> ExprT;	
	return VectorExpr<ExprT,TDataType>(ExprT(ExprLiteral<TDataType>(a),b));
}

/**
 */
template<class A,class TDataType>
VectorExpr< BinaryOperation<VectorExpr<A,TDataType>,ExprLiteral<TDataType>,DApMultiply<TDataType>,TDataType>,TDataType>
operator*(const VectorExpr<A,TDataType>& b,const TDataType& a){
	typedef BinaryOperation<VectorExpr<A,TDataType>,ExprLiteral<TDataType>,DApMultiply<TDataType>,TDataType> ExprT;	
	return VectorExpr<ExprT,TDataType>(ExprT(b,ExprLiteral<TDataType>(a)));
}

/**
*	Division con literal
*/

/**
 */
template <class TDataType>
VectorExpr< BinaryOperation<ExprLiteral<TDataType>,TDataType*,DApDivide<TDataType>,TDataType>,TDataType>
operator/(const TDataType& a, const NumericalVector<TDataType>& b){
	typedef BinaryOperation<ExprLiteral<TDataType>,TDataType*,DApDivide<TDataType>,TDataType> ExprT;	
	return VectorExpr<ExprT,TDataType>(ExprT(ExprLiteral<TDataType>(a),b.begin()));
}

/**
 */
template <class TDataType>
VectorExpr< BinaryOperation<TDataType*,ExprLiteral<TDataType>,DApDivide<TDataType>,TDataType>,TDataType>
operator/(const NumericalVector<TDataType>& b,const TDataType& a){
	typedef BinaryOperation<TDataType*,ExprLiteral<TDataType>,DApDivide<TDataType>,TDataType> ExprT;	
	return VectorExpr<ExprT,TDataType>(ExprT(b.begin(),ExprLiteral<TDataType>(a)));
}

/**
 */
template<class A,class TDataType>
VectorExpr< BinaryOperation<ExprLiteral<TDataType>,VectorExpr<A,TDataType>,DApDivide<TDataType>,TDataType>,TDataType>
operator/(const TDataType& a, const VectorExpr<A,TDataType>& b){
	typedef BinaryOperation<ExprLiteral<TDataType>,VectorExpr<A,TDataType>,DApDivide<TDataType>,TDataType> ExprT;	
	return VectorExpr<ExprT,TDataType>(ExprT(ExprLiteral<TDataType>(a),b));
}

/**
 */
template<class A,class TDataType>
VectorExpr< BinaryOperation<VectorExpr<A,TDataType>,ExprLiteral<TDataType>,DApDivide<TDataType>,TDataType>,TDataType>
operator/(const VectorExpr<A,TDataType>& b,const TDataType& a){
	typedef BinaryOperation<VectorExpr<A,TDataType>,ExprLiteral<TDataType>,DApDivide<TDataType>,TDataType> ExprT;	
	return VectorExpr<ExprT,TDataType>(ExprT(b,ExprLiteral<TDataType>(a)));
}

/** 
*	Operadores de extracci�n de flujo
*/	
template <class TDataType>
ostream& operator<<(ostream& os, const NumericalVector<TDataType>& nv){    
  int i=0;
  for(i=0;i<nv.length();i++){
    os<<nv.getData(i)<<" ";
  }		
	return os;
}

NUM_END

#endif

