#ifndef MATRIX_H
#define MATRIX_H

#include "base.h"
#include "numericalvector.h"

NUM_BEGIN

template <class T>
class Matrix;

/**
*	Provee una función virtual para asignar usada para implementar
*	por las asignaciones.
*/
template <class TDataType>
class MatrixAssignable {
public:
	/**
	*	Función para asignar vectores, esta es la base de la expansión
	*	para la evaluación.
	*	@param Valor de asignación.
	*/
  virtual void assign(Matrix<TDataType>& M) const = 0;
};

/**
*	Clase que modela una matriz sobre el estandar TNT
*	Se puede accesar con indice 0 con[][] y con indice 1 si () es usado
*/
template <class T>
class Matrix {
public:
	// Tipos de datos para la matriz
  typedef T& reference;
	typedef const T& const_reference;
  typedef T* iterator;

public:
	
	// Construcción y destrucción
	/**
	*	Constructor por omision
	*/
	Matrix();


	/**
	*	Constructor por omision
	*/
	Matrix(const unsigned int& w);


	/**
	*	Construye una matriz de dimensiones w, h.
	*	@param w ancho de la matriz
	*	@param h alto de la matriz
	*/
	Matrix(const unsigned int& w,const unsigned int& h);

	/**
	*	Constructor de copia.
	*	@param m matriz a copiar.
	*/
	Matrix(const Matrix<T>& m);

	/**
	*	Construye una matriz de dimensiones w, h y la rellena con v.
	*	@param w ancho de la matriz
	*	@parma h alto de la matriz
	*	@param v valor escalar a rellenar
	*/	
	Matrix(const unsigned int& w,const unsigned int& h,const T& v);

	/**
	*	Construye una matriz de dimensiones w, h y la rellena con los valores del vector n.
	*	@param n vector con los valores a llenar
	*	@param w ancho de la matriz
	*	@param h alto de la matriz
	*/
	Matrix(const NumericalVector<T>& n,const unsigned int& w,const unsigned int& h);

	/**
	* 	Elimina la matriz
	*/	
	~Matrix();

public:
	// Operadores y funcionalidades
	/**
	* 	Retorna el número de entradas en la matriz
	*	@param retorna el numero de entradas en la matriz
	*/
	unsigned int size()const;

	/**
	* 	Retorna la dimension especificada de la matriz w si m=1, h si m=2.
	*	@return valor de la dimensión especificada
	*/
	unsigned int dim(const unsigned int& dim)const;

	/**
	* 	Retorna la dimension especificada de la matriz w si m=1, h si m=2.
	*	@return ancho de la matriz
	*/
	unsigned int getWidth()const;

	/**
	* 	Retorna la dimension especificada de la matriz w si m=1, h si m=2.
	*	@return alto de la matriz.
	*/
	unsigned int getHeight()const;

	/**
	* 	Redimensiona la matriz.
	*	@param w nuevo ancho de la matriz.
	*	@param h nuevo alto de la matriz.
	*/	
	void newsize(const unsigned int& w,const unsigned int& h);

	/**
	* 	Asigna una matriz m a otra.
	*	@param m matriz a asignar.
	*	@return Matriz asignada.
	*/	
	Matrix<T>& operator=(const Matrix<T>& m);	

	/**
	* 	Asigna un escalar m a la matriz.
	*	@param m Valor escalar asignado a la matriz
	*	@return Matriz con todas las entradas iguales a m
	*/
	Matrix<T>& operator=(const T& m);	


	/**
	* 	Operador de acceso para indice 0.
	*	@param i indice de la fila.
	*	@return Referencia a la i-esima fila.
	*/
	T* operator[](const unsigned int& i);

	/**
	* 	Operador de acceso para indice 0.
	*	@param i indice de la fila
	*	@return referencia constante a la i-esima fila
	*/
	const T* operator[](const unsigned int& i) const;

	/**
	* 	Operador de acceso para indice 1
	*	@return Referencia a la i-esima fila
	*/
	reference operator()(const unsigned int& i);

	/**
	* 	Operador de acceso para indice 1
	*	@param i indice de la fila.
	*	@return referencia constante a los datos de la fila.
	*/
	const_reference operator()(const unsigned int& i) const;
	
	/**
	* 	Operador de acceso para inidice 1.
	*	@param i indice de la fila.
	*	@param j indice de la columna.
	*	@return referencia al valor (i,j) de la matriz.
	*/
	reference operator()(const unsigned int& i,const unsigned int& j);

	/**
	* 	Operador de acceso para indice 1.
	*	@param i indice de la fila.
	*	@param j indice de la columna.
	*	@return referencia constante al valor (i,j) de la matriz.
	*/
	const_reference operator()(const unsigned int& i,const unsigned int& j) const;

	/**
	*	Operador de acceso para expresion templates.
	*	@param x Matiz a asignar
	*	@return Resultado de la copia de la matriz
	*/
	Matrix<T>& operator=(const MatrixAssignable<T>& x);

	/**
	*	Retorna un iterador al inicio de la matriz.
	*	@return Iterador al inicio de la matriz
	*/
	iterator begin()const;

	/**
	*	Retorna un iterador al final
	*	@return Iterador al final de la matriz.
	*/
	iterator end() const;

protected:
	//Miembros protegidos
	/**
	*	Inicializa una matriz.
	*	@param w Ancho de la matriz.
	*	@param h Alto de la matriz
	*/
	void initialize(const unsigned int& w,const unsigned int& h);

	/**
	*	Copia los valores de un vector en una matriz.
	*	@param Vector con los valores a copiar
	*/
	void copy(const NumericalVector<T>& v);

	/**
	*	Destruye una matriz.
	*/
	void destroy();

	/**
	*	Fija un valor escalar en una matriz.
	*	@param v valor a llenar
	*/
	void set(const T& v);	
	
public:
	/** Intercambia los valores de dos filas de la matriz.
	*	@param i fila uno del intercambio
	*	@param j fila dos del intercambio
	*/
	void interchangeRow(const unsigned int& i, const unsigned int&j);

	/** Intercambia los valores dos columnas de la matriz
	*	@param i columa uno del intercambio
	*	@param j columna dos del intercambio
	*/
	void interchangeCol(const unsigned int& i, const unsigned int&j);

public:
	// Operaciones
	/** 
	*	Retorna la transpuesta de una matriz.
	*	@return transpuesta de una matriz
	*/
	Matrix<T> traspose();

	/** Carga la matriz identidad
	*/
	void setIdentity();

	/** Resuelve el sistema lineal Ax=b.
	*	@param b vector lado derecho
	*	@param c vector de coeficientes
	*	@param x Vector de soluciones, este vector es D*x', donde x' es la solución real
	*	@param D valor para almacenar el valor de la determinante
	*	@param spanning_vectors vectores generados
	*	@return true si el sistema tiene solución, else en caso contrario
	*/
	bool solveLinearSystem(NumericalVector<T>& b,NumericalVector<T>& c,NumericalVector<T>& x,T& D,Matrix<T>& spanning_vectors);


	/** Calcula la inversa de la matriz y la almacena en la matriz resultado
	*	@param Matriz destino
	*	@return true si la matriz es invertible, false en caso contrario
	*/
	bool inverse(Matrix<T>& target);

	/** Multiplica dos matrices A*B=C
	*	@param m1 matriz A
	*	@param m2 matriz B
	*	@param resul matriz para almacenar el resultado
	*/
	static void multiply(const Matrix<T>& m1,const Matrix<T>& m2,Matrix<T>& resul);

	/** Multiplica dos matrices A*v1=v2
	*	@param m matriz A
	*	@param v1 vector v1
	*	@param resul vector para almacenar el resultado
	*/
	static void multiply(const Matrix<T>& m,const NumericalVector<T>& v1,NumericalVector<T>& v2);
	
	

private:	
	//Miembros privados

	// Vector con los datos númericos
	NumericalVector<T>* m_values;		
	// Apuntador a las filas
	T** m_rows;
	// Desplazamientos para indice 1
	NumericalVector<T>* m_values1;	
	// Desplazamientos para indice 1
	T** m_rows1;                    
	// Ancho
	unsigned int m_w;
	// Alto
	unsigned int m_h;
	// Tamaño del vector que representa la matriz
	unsigned int m_wh;
};


// Miembros para asignación de memoria
template <class T>
inline void Matrix<T>::initialize(const unsigned int& w,const unsigned int& h)
{	
	// Asigna las dimensiones
	m_w=w;
	m_h=h;
	m_wh=w*h;
	// Asigna memoria al vector
	m_values1=m_values=new NumericalVector<T>(w*h);
	CHECKMEMORY(m_values);

	// Asigna memoria para las filas
	m_rows=new T*[h];
	CHECKMEMORY(m_rows);
	m_rows1=new T*[h];	
	CHECKMEMORY(m_rows1);
	T* p=m_values->begin();
	for(unsigned int i=0;i<h;i++)
	{
		// Asigna el apuntador al valor necesario
		m_rows[i]=p;
		m_rows1[i]=p-1;
		p=p+m_w;
	}
	m_rows1--;
}

template <class T>
inline void Matrix<T>::copy(const NumericalVector<T>& v)
{
	// Copia el vector
	T* i=m_values->begin();
	T* j=v.begin();
	do{
		*i=*j;
		++j;
	}while(++i!=m_values->end());
}

template <class T>
inline void Matrix<T>::destroy()
{
	// elimina los valores
	delete m_values;	
	m_values=0;
	// elimina las filas
	delete[] m_rows;
	if(m_rows1!=0)
		m_rows1++;
	delete[] m_rows1;
	m_rows=0;
	m_rows1=0;
}

template <class T>
void Matrix<T>::set(const T& v)
{
	T* i=m_values->begin();	
	do{			
		*i=v;
	}while(++i!=m_values->end());
}

// Constructores
template <class T>
inline Matrix<T>::Matrix() : m_values(0),m_values1(0), m_rows(0),m_rows1(0), m_w(0), m_h(0), m_wh(0)
{

}

template <class T>
Matrix<T>::Matrix(const Matrix<T>& m)
{
	initialize(m.m_w,m.m_h);
 	copy(*m.m_values);
}

template <class T>
inline Matrix<T>::Matrix(const unsigned int& s)
{
	initialize(s,s);
}

template <class T>
inline Matrix<T>::Matrix(const unsigned int& w,const unsigned int& h)
{
	initialize(w,h);
}


template <class T>
inline Matrix<T>::Matrix(const unsigned int& w,const unsigned int& h,const T& v)
{
	initialize(w,h);
	set(v);
}


template <class T>
inline Matrix<T>::Matrix(const NumericalVector<T>& n,const unsigned int& w,const unsigned int& h)
{
	initialize(w,h);
	copy(n);
}

template <class T>
inline Matrix<T>::~Matrix()
{
	destroy();
}

template <class T>
inline void Matrix<T>::newsize(const unsigned int& w,const unsigned int& h)
{
	if(w==m_w && h==m_h)
  		return;
	destroy();
	initialize(w,h);
	return;
}

template <class T>	
inline Matrix<T>& Matrix<T>::operator=(const Matrix<T>& M)
{	
	if(m_values==M.m_values)
		return *this;

	// No necesita relocalizar la memoria	
	if(m_w==M.m_w && m_h==M.m_h){
		copy(*M.m_values);
	}else{
		destroy();
		initialize(M.m_w,M.m_h);
		copy(*M.m_values);
	}	
	return *this;
}

template <class T>	
inline Matrix<T>& Matrix<T>::operator=(const T& m)
{
	set(m);
	return *this;
}

template <class T>	
unsigned int Matrix<T>::size()const
{
	return m_wh;
}

template <class T>	
inline unsigned int Matrix<T>::dim(const unsigned int& dim) const
{
	return (dim==1)?m_w:(dim==2)?m_h:0;
}


template <class T>	
inline unsigned int Matrix<T>::getWidth() const
{
	return m_w;
}

template <class T>	
inline unsigned int Matrix<T>::getHeight() const
{
	return m_h;
}


template <class T>	
inline Matrix<T>& Matrix<T>::operator=(const MatrixAssignable<T>& x){					
	x.assign(*this);
	return *this;
}

template <class T>	
inline Matrix<T>::iterator Matrix<T>::begin() const{
 	return m_values->begin();
}

template <class T>	
inline Matrix<T>::iterator Matrix<T>::end() const{
 	return m_values->begin() + m_wh;
}


template <class T>	
inline T* Matrix<T>::operator[](const unsigned int& i)
{
	return m_rows[i];
}

template <class T>	
inline const T* Matrix<T>::operator[](const unsigned int& i) const
{
	return m_rows[i];
}

template <class T>	
inline Matrix<T>::reference Matrix<T>::operator()(const unsigned int& i)
{
	return (*m_values1)[i];	
}


template <class T>	
inline Matrix<T>::const_reference Matrix<T>::operator()(const unsigned int& i) const
{
	return (*m_values1)[i];	
}

template <class T>	
inline Matrix<T>::reference Matrix<T>::operator()(const unsigned int& i,const unsigned int& j)
{
	return m_rows1[i][j];
}

template <class T>	
inline Matrix<T>::const_reference Matrix<T>::operator()(const unsigned int& i,const unsigned int& j) const
{
	return m_rows1[i][j];
}


// Operaciones

template <class T>	
inline Matrix<T> Matrix<T>::traspose()
{
	unsigned int i=0,j=0;
	Matrix<T> r(m_h,m_w);
	for(i=0;i<m_w;i++)
	{		
  		for(j=0;j<m_h;j++)
		{
			r[j][i] = (*this)[i][j];
		}
	}
	return r;
}


template <class T>	
inline void Matrix<T>::setIdentity()
{
	unsigned int i=0,j=0;
	for(i=0;i<m_h;i++)
		for(j=0;j<m_w;j++){
			if(i!=j)
				(*m_values)[i*m_w+j]=0;
			else
				(*m_values)[i*m_w+j]=1;
		}
}


template <class T>
void Matrix<T>::interchangeRow(const unsigned int& i, const unsigned int&j)
{
	unsigned int k;
	T tempdata;
	if(i==j)
		return;
	for(k=0;k<m_w;k++)
	{
		tempdata=(*m_values)[i*m_w+k];
		(*m_values)[i*m_w+k]=(*m_values)[j*m_w+k];
		(*m_values)[j*m_w+k]=tempdata;
	}
}

template <class T>	
void Matrix<T>::interchangeCol(const unsigned int& i, const unsigned int&j){
	unsigned int k;
	T tempdata;
	if(i==j)
		return;
	for(k=0;k<m_h;k++)
	{
		tempdata=(*m_values)[k*m_w+i];
		(*m_values)[k*m_w+i]=(*m_values)[k*m_w+j];
		(*m_values)[k*m_w+j]=tempdata;
	}
}

template <class T>
bool Matrix<T>::solveLinearSystem(NumericalVector<T>& b, NumericalVector<T>& c, NumericalVector<T>& x, T& D, Matrix<T>& spanning_vectors){
	// Gauss Elimination J Edmonds (1968) Systems and varius representatives
	// and algebra linear LEDA (imatrix)
	// La eliminación gaussiana opera en dos fases
	int i=0,j=0,k=0;
	bool solvable=true;	
	// El Sistema lineal es Ax=b
	
	Matrix<T> C(getWidth()+1,getHeight());
	Matrix<T> L(getHeight(),getHeight());


	// C Inicialmente se carga con A
	for(i=0 ; i<getHeight() ; i++){
		for(j=0 ; j<getWidth() ; j++)
    		C[i][j]=(*this)[i][j];
		C[i][getWidth()]=b[i];
	}		
	L.setIdentity();	

	//var es un vector de los indices de las columnas de C
	std::vector <int> var(getWidth());
	for(i=0 ; i<getWidth() ; i++)
		var[i] = i;

	T denom;
  denom=1;  
	int sign = 1;
	int rank = 0;
  	
	int rows = getHeight();
	int cols = getWidth();	
  for(k=0; k<rows; k++){
		//Esta parte es una reducción gaussiana convencional
		bool non_zero_found = false;
		for(i =k; i<rows; i++){
			// Desde k-esima fila hasta rows-1
			for (j = k ; j < cols && C[i][j] == 0; j++) ;
		   // Desde la k-esima columna hasta cols-1
			if (j < cols){
				non_zero_found = true;
				break;
			}
		}

		//No se encontro pivote en la exploración inicial
		if (non_zero_found){
			rank++; //Incremente el rango
			if (i != k){
				sign = - sign;
				//Intercambia la fila k y i
				L.interchangeRow(k,i);
				C.interchangeRow(k,i);
			}

			if (j != k){
				sign = - sign;
				// Intercambia las columnas k y j
				C.interchangeCol(k,j);

				// Se alamacena el intercambio de las variables en var
				int temp=var[k];
				var[k]=var[j];
				var[j]=temp;
			}

			for(i = k + 1; i < rows; i++)
				for (j = 0; j <  rows; j++)  //todas las columnas de L										
					L[i][j] = (L[i][j] * C[k][k] - C[i][k] * L[k][j] )/denom;

			for(i = k + 1; i < rows; i++){
				T temp = C[i][k];
				for (j = k; j <= cols; j++)
					C[i][j]= (C[i][j]*C[k][k] - temp*C[k][j])/denom;
			}

			denom = C[k][k];
		}else
			break;
	}	

  for(i =rank ; i <rows && C[i][cols] == 0; i++);
  	if(i < rows){
		solvable = false;
		c.newSize(rows);
		for (j = 0; j <rows; j++)
    		c[j] = L[i][j];
	}

  if(solvable){        
		x.newSize(cols);
		D = denom;

		for(i=rank - 1 ; i>= 0; i--){
			T h = C[i][cols] * D;
			for (j = i + 1; j<rank; j++){
				h -= C[i][j]*x[var[j]];
			}
			x[var[i]] = h / C[i][i];
		}    
  }else{    
		int dimension=cols - rank;		
		spanning_vectors.newsize(dimension,cols);
		if(dimension>0){
    		for(int l=0;l < dimension;l++){
     				spanning_vectors[var[rank+l]][l]=D;
					for(i = rank - 1;i>=0 ; i--){
          				T h = -C[i][rank+l]*D;
						for(j=i+1;j<rank;j++)
							h-=C[i][j]*spanning_vectors[var[j]][l];
						spanning_vectors[var[i]][l]=h/C[i][i];
					}  	
			}
		}
	}
  return solvable;
}



template <class T>
bool Matrix<T>::inverse(Matrix<T>& target)
{
	return true;
}


// Funciones para expression templates


/**
*		Función template para la asignación de un resultados, binarios
*/
template<class Iter,class DataType>
void assignResult(Matrix<DataType>& a, const Iter& result)
{
#ifdef EXPRVEC_USE_TEMPORARY_EXPRESSION
		// Realiza una copita temporal de un iterador. Esto es rapìdo en arquitecturas
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
        iter[i] = result2[i];   // Expande la expresión
#else
		// Obtiene los iteradores inicial y final del vector
    DataType* iter = a.begin();
    DataType* end  = a.end();

		// Copia los valores
    do {
        *iter = *result2;       // Expande la expresión
        ++result2;
    } while (++iter != end);
#endif
}

/**
*		Función template para la asignación de un resultado, unario
*/
template<class Iter,class DataType>
void assignResultLiteral(Matrix<DataType>& a, const Iter& result)
{
#ifdef EXPRVEC_USE_TEMPORARY_EXPRESSION
		// Realiza una copita temporal de un iterador. Esto es rapìdo en arquitecturas
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
        iter[i] = result2[i];   // Expande la expresión
#else
		// Obtiene los iteradores inicial y final del vector
    DataType* iter = a.begin();
    DataType* end  = a.end();

		// Copia los valores
    do {
        *iter = *result2;       // Expande la expresión
    } while (++iter != end);
#endif
}

/**
*		Función template para la asignación de un resultados, binarios
*/
template<class Iter,class DataType>
void assignResultTraspose(Matrix<DataType>& a, const Iter& result)
{
#ifdef EXPRVEC_USE_TEMPORARY_EXPRESSION
	// Realiza una copia temporal de un iterador. Esto es rapìdo en arquitecturas
	// segmentadas, si todos los iteradores estan en el mismo segmento
    Iter result2 = result;
#else
		// Realiza un cast sobre la referencia constante
    Iter& result2 = (Iter&)result;
#endif

#ifdef EXPRVEC_USE_INDIRECTION
		// Copia los valores de los vectores
		// Obtiene una referencia al inicio del array
    DataType* iter = a.begin();
	DataType tempdata;
    for (register unsigned int i = 0; i<a.getWidth();i++)
		for (register unsigned int j =0; j<a.getHeight();j++){
			tempdata=iter[i*a.getWidth()+j];
			iter[i*a.getWidth()+j]=result2[j*a.getWidth()+i];
			result2[j*a.getWidth()+i]=tempdata;			
			
		}
#else
    DataType* iter = a.begin();
	DataType tempdata;				
	if(*iter!=*result2){
		for (register unsigned int i = 0; i<a.getWidth();i++)
			for (register unsigned int j =0; j<a.getHeight();j++){												
				iter[i*a.getWidth()+j]=result2[j*a.getWidth()+i];
			}		
	}else{
		for (register unsigned int i = 0; i<a.getWidth();i++)
			for (register unsigned int j =0; j<=i;j++){												
				if(i==j){
					iter[i*a.getWidth()+j]=result2[j*a.getWidth()+i];
				}else{					
					tempdata=iter[i*a.getWidth()+j];
					iter[i*a.getWidth()+j]=result2[j*a.getWidth()+i];
					iter[j*a.getWidth()+i]=tempdata;
				}
			}		
	}
#endif
}


/**
*		Esta clase modela el tipo de expresión
*/
template<class A,class TDataType>
class MatrixExpr : public MatrixAssignable<TDataType>{
public:
    MatrixExpr(const A& a) : m_iter(a){
			// No hace más nada
		}

    TDataType operator*() const {
			// Retorna el iterador
			return *m_iter;
		}

    TDataType operator[](const unsigned int& i) const{
			// Retorna el iterador en su iesima posición
			return m_iter[i];
		}

    void operator++(){
			// Avanza el itierador
			++m_iter;
		}

		/**
		*		Función virtual para la asignación
		*/
    virtual void assign(Matrix<TDataType>& x) const{
			assignResult(x,*this);
		}

private:
    A m_iter;
};


/**
*	Esta clase modela una expresión de tipo Asignable
*/
template<class TDataType>
class ExprLiteralMatrix : public MatrixAssignable<TDataType>{
public:
  ExprLiteralMatrix(const TDataType& a) : m_iter(a){
		// No hace más nada
	}

	void operator++(){
		// Se provee por compatibilidad con los operadores binarios
	}

  TDataType operator*() const {		
		return m_iter;
	}    	

	/**
	*	Sobreescribe la función virtual de asignación
	*/
  virtual void assign(Matrix<TDataType>& x) const{
		// Llama a la función de para que evalue la expresión y la asigne a x
		assignResultLiteral(x,*this);
	}
private:
    TDataType m_iter;
};


/**
*	Esta clase modela una expresión de tipo Asignable
*/
template<class A,class TDataType>
class ExprMatrixTraspose : public MatrixAssignable<TDataType>{
public:
  ExprMatrixTraspose(const A& a) : m_iter(a){
		// No hace más nada
	}

	void operator++(){
		// Se provee por compatibilidad con los operadores binarios
		++ m_iter;
	}

  TDataType operator*() const {		
		return *m_iter;
	}    	

  TDataType operator[](const unsigned int& i) const{
			// Retorna el iterador en su iesima posición
			return m_iter[i];
	}


	/**
	*	Sobreescribe la función virtual de asignación
	*/
	virtual void assign(Matrix<TDataType>& x) const{		
		// Llama a la función de para que evalue la expresión y la asigne a x
		assignResultTraspose(x,*this);
	}
private:
	A m_iter;
};


// Operadores de adicion para la matriz
template <class TDataType>
MatrixExpr< BinaryOperation<TDataType* ,TDataType* ,DApAdd<TDataType> ,TDataType >,TDataType>
operator+(const Matrix<TDataType>& a, const Matrix<TDataType>& b){
    typedef BinaryOperation<TDataType*,TDataType*,DApAdd<TDataType>,TDataType> ExprT;
    return MatrixExpr<ExprT,TDataType>(ExprT(a.begin(),b.begin()));
}


template<class A,class TDataType>
MatrixExpr< BinaryOperation< MatrixExpr<A,TDataType>,	TDataType*, DApAdd<TDataType>, TDataType>, TDataType>
operator+(const MatrixExpr<A,TDataType>& a, const Matrix<TDataType>& b){
    typedef BinaryOperation<MatrixExpr<A,TDataType>,TDataType*,DApAdd<TDataType>,TDataType> ExprT;
    return MatrixExpr<ExprT,TDataType>(ExprT(a,b.begin()));
}

template<class A,class TDataType>
MatrixExpr< BinaryOperation< MatrixExpr<A,TDataType>,	TDataType*, DApAdd<TDataType>, TDataType>, TDataType>
operator+(const Matrix<TDataType>& b,const MatrixExpr<A,TDataType>& a){
    typedef BinaryOperation<MatrixExpr<A,TDataType>,TDataType*,DApAdd<TDataType>,TDataType> ExprT;
    return MatrixExpr<ExprT,TDataType>(ExprT(a,b.begin()));
}

template<class A, class B,class TDataType>
MatrixExpr<BinaryOperation<MatrixExpr<A,TDataType>,MatrixExpr<B,TDataType>,DApAdd<TDataType>,TDataType> ,TDataType >
operator+(const MatrixExpr<A,TDataType>& a, const MatrixExpr<B,TDataType>& b){
    typedef BinaryOperation<MatrixExpr<A,TDataType>,MatrixExpr<B,TDataType>,DApAdd<TDataType>,TDataType> ExprT;
    return MatrixExpr<ExprT,TDataType>(ExprT(a,b));
}

//	Operadores de substracción

template <class TDataType>
MatrixExpr< BinaryOperation<TDataType* ,TDataType* ,DApSubtract<TDataType> ,TDataType >,TDataType>
operator-(const Matrix<TDataType>& a, const Matrix<TDataType>& b){
    typedef BinaryOperation<TDataType*,TDataType*,DApSubtract<TDataType>,TDataType> ExprT;
    return MatrixExpr<ExprT,TDataType>(ExprT(a.begin(),b.begin()));
}

template<class A,class TDataType>
MatrixExpr< BinaryOperation< MatrixExpr<A,TDataType>,	TDataType*, DApSubtract<TDataType>, TDataType>, TDataType>
operator-(const MatrixExpr<A,TDataType>& a, const Matrix<TDataType>& b){
    typedef BinaryOperation<MatrixExpr<A,TDataType>,TDataType*,DApSubtract<TDataType>,TDataType> ExprT;
    return MatrixExpr<ExprT,TDataType>(ExprT(a,b.begin()));
}

template<class A,class TDataType>
MatrixExpr< BinaryOperation<TDataType*,MatrixExpr<A,TDataType>,DApSubtract<TDataType>,TDataType>, TDataType>
operator-(const Matrix<TDataType>& a, const MatrixExpr<A,TDataType>& b){
    typedef BinaryOperation<TDataType*,MatrixExpr<A,TDataType>,DApSubtract<TDataType>,TDataType> ExprT;
    return MatrixExpr<ExprT,TDataType>(ExprT(a.begin(),b));
}

template<class A, class B,class TDataType>
MatrixExpr<BinaryOperation<MatrixExpr<A,TDataType>,MatrixExpr<B,TDataType>,DApSubtract<TDataType>,TDataType> ,TDataType >
operator-(const MatrixExpr<A,TDataType>& a, const MatrixExpr<B,TDataType>& b){
    typedef BinaryOperation<MatrixExpr<A,TDataType>,MatrixExpr<B,TDataType>,DApSubtract<TDataType>,TDataType> ExprT;
    return MatrixExpr<ExprT,TDataType>(ExprT(a,b));
}

//	Mutiplicacion de los valores uno a uno

template <class TDataType>
MatrixExpr< BinaryOperation<TDataType* ,TDataType* ,DApMultiply<TDataType> ,TDataType >,TDataType>
operator*(const Matrix<TDataType>& a, const Matrix<TDataType>& b){
    typedef BinaryOperation<TDataType*,TDataType*,DApMultiply<TDataType>,TDataType> ExprT;
    return MatrixExpr<ExprT,TDataType>(ExprT(a.begin(),b.begin()));
}

template<class A,class TDataType>
MatrixExpr< BinaryOperation< MatrixExpr<A,TDataType>,	TDataType*, DApMultiply<TDataType>, TDataType>, TDataType>
operator*(const MatrixExpr<A,TDataType>& a, const Matrix<TDataType>& b){
    typedef BinaryOperation<MatrixExpr<A,TDataType>,TDataType*,DApMultiply<TDataType>,TDataType> ExprT;
    return MatrixExpr<ExprT,TDataType>(ExprT(a,b.begin()));
}

template<class A,class TDataType>
MatrixExpr< BinaryOperation<TDataType*,MatrixExpr<A,TDataType>,DApMultiply<TDataType>,TDataType>, TDataType>
operator*(const Matrix<TDataType>& a, const MatrixExpr<A,TDataType>& b){
    typedef BinaryOperation<TDataType*,MatrixExpr<A,TDataType>,DApMultiply<TDataType>,TDataType> ExprT;
    return MatrixExpr<ExprT,TDataType>(ExprT(a.begin(),b));
}

template<class A, class B,class TDataType>
MatrixExpr<BinaryOperation<MatrixExpr<A,TDataType>,MatrixExpr<B,TDataType>,DApMultiply<TDataType>,TDataType> ,TDataType >
operator*(const MatrixExpr<A,TDataType>& a, const MatrixExpr<B,TDataType>& b){
    typedef BinaryOperation<MatrixExpr<A,TDataType>,MatrixExpr<B,TDataType>,DApMultiply<TDataType>,TDataType> ExprT;
    return MatrixExpr<ExprT,TDataType>(ExprT(a,b));
}


//	Mutiplicacion con literal

template <class TDataType>
MatrixExpr< BinaryOperation<ExprLiteralMatrix<TDataType>,TDataType*,DApMultiply<TDataType>,TDataType>,TDataType>
operator*(const TDataType& a, const Matrix<TDataType>& b){
   typedef BinaryOperation<ExprLiteralMatrix<TDataType>,TDataType*,DApMultiply<TDataType>,TDataType> ExprT;
	return MatrixExpr<ExprT,TDataType>(ExprT(ExprLiteralMatrix<TDataType>(a),b.begin()));
}

template <class TDataType>
MatrixExpr< BinaryOperation<ExprLiteralMatrix<TDataType>,TDataType*,DApMultiply<TDataType>,TDataType>,TDataType>
operator*(const Matrix<TDataType>& b,const TDataType& a){
    typedef BinaryOperation<ExprLiteralMatrix<TDataType>,TDataType*,DApMultiply<TDataType>,TDataType> ExprT;	
	return MatrixExpr<ExprT,TDataType>(ExprT(ExprLiteralMatrix<TDataType>(a),b.begin()));
}

template<class A,class TDataType>
MatrixExpr< BinaryOperation<ExprLiteralMatrix<TDataType>,MatrixExpr<A,TDataType>,DApMultiply<TDataType>,TDataType>,TDataType>
operator*(const TDataType& a, const MatrixExpr<A,TDataType>& b){
    typedef BinaryOperation<ExprLiteralMatrix<TDataType>,MatrixExpr<A,TDataType>,DApMultiply<TDataType>,TDataType> ExprT;	
	return MatrixExpr<ExprT,TDataType>(ExprT(ExprLiteralMatrix<TDataType>(a),b));
}

template<class A,class TDataType>
MatrixExpr< BinaryOperation<MatrixExpr<A,TDataType>,ExprLiteralMatrix<TDataType>,DApMultiply<TDataType>,TDataType>,TDataType>
operator*(const MatrixExpr<A,TDataType>& b,const TDataType& a){
    typedef BinaryOperation<MatrixExpr<A,TDataType>,ExprLiteralMatrix<TDataType>,DApMultiply<TDataType>,TDataType> ExprT;	
	return MatrixExpr<ExprT,TDataType>(ExprT(b,ExprLiteralMatrix<TDataType>(a)));
}

//	Trasposicion y operaciones con trasposicion
template <class TDataType>
ExprMatrixTraspose<UnaryOperation<TDataType* ,DApIdentity<TDataType> ,TDataType >,TDataType>
traspose(const Matrix<TDataType>& a){
    typedef UnaryOperation<TDataType* ,DApIdentity<TDataType> ,TDataType > ExprT;
    return ExprMatrixTraspose<ExprT,TDataType>(ExprT(a.begin()));
}

template<class A, class B,class TDataType>
MatrixExpr<BinaryOperation<ExprMatrixTraspose<A,TDataType>,ExprMatrixTraspose<B,TDataType>,DApAdd<TDataType>,TDataType> ,TDataType >
operator+(const ExprMatrixTraspose<A,TDataType>& a, const ExprMatrixTraspose<B,TDataType>& b){
    typedef BinaryOperation<ExprMatrixTraspose<A,TDataType>,ExprMatrixTraspose<B,TDataType>,DApAdd<TDataType>,TDataType> ExprT;
    return MatrixExpr<ExprT,TDataType>(ExprT(a,b));
}

// Faltan más operaciones y acoplarlas con los otros tipos de expresion


// Operador de extracción de flujo
template <class T>
ostream& operator<<(ostream& os,const Matrix<T>& t){
	unsigned int w=t.getWidth(), h=t.getHeight();
	for(unsigned int i=0;i<h;i++){
		for(unsigned int j=0;j<w;j++){
			os<<t[i][j]<<" ";
		}
		os<<"\n";
	}
	return os;
}

template <class T>
void Matrix<T>::multiply(const Matrix<T>& m1,const Matrix<T>& m2,Matrix<T>& resul)
{	
	int i,j,k;
	resul.newsize(m2.getWidth(),m1.getHeight());		
	for(i=0;i<m1.getHeight();i++){		
		for(j=0;j<m2.getWidth();j++){
			//temp=0;
			resul[i][j]=0;
      for(k=0;k<m2.getHeight();k++){        
				resul[i][j]+=m1[i][k]*m2[k][j];			
      }
		}		
	}
}


template <class T>
void Matrix<T>::multiply(const Matrix<T>& m1,const NumericalVector<T>& v,NumericalVector<T>& vr)
{
	int i,j,k;
	T temp;
	vr.newSize(m1.getHeight());
	for(i=0;i<m1.getHeight();i++){		
		temp=0;
    for(j=0;j<v.length();j++){  
				temp+=m1[i][j]*v.getData(j);
    }
		vr[i]=temp;
	}
}

NUM_END

#endif
