#ifndef TRANSFORMATIONAFFINE_H
#define TRANSFORMATIONAFFINE_H

CGL_BEGIN

/** Esta clase modela una transformación afín que lleva un punto de un espacio afín a otro
 */
template <class TCGAfinneSpaceSource,class TCGAfinneSpaceTarget>
class TransformationAffine {
public:
	/** 
	 */
	typedef Matrix<TCGAfinneSpaceSource::TDataType> TMatrix;	
	/**
	 */
	typedef typename TCGAfinneSpaceSource::AffineFrame TAffineFrameSource;
	/**
	 */
	typedef typename TCGAfinneSpaceTarget::AffineFrame TAffineFrameTarget;
	/**
	 */
	typedef typename TCGAfinneSpaceSource::AffinePoint TAffinePointSource;
	/**
	 */
	typedef typename TCGAfinneSpaceTarget::AffinePoint TAffinePointTarget;
	/**
	 */
	typedef typename TCGAfinneSpaceSource::AffineVector TAffineVectorSource;
	/**
	 */
	typedef typename TCGAfinneSpaceTarget::AffineVector TAffineVectorTarget;	

public:
	/** Constructor por omisión
	 */
	TransformationAffine() : m_isInvertible(false) {
		m_transformation.newsize(TCGAfinneSpaceSource::dim+1,TCGAfinneSpaceSource::dim+1);
		m_inversetransformation.newsize(TCGAfinneSpaceSource::dim+1,TCGAfinneSpaceSource::dim+1);
		m_inversetransformation.setIdentity();
		m_transformation.setIdentity();
	}
	/** Lleva un frame en un punto y un conjunto de vectores en otro espacio
	 *	Por ahora el espacio origen y el destino son los mismos.
	 *	@param f frame afín en el espacio origen
	 *	@param o punto en el espacio destino
	 *	@param v conjunto de vectores en el espacio origen
	 */
	TransformationAffine(const TAffineFrameSource& f1,const TAffinePointTarget& o,TAffineVectorTarget* v[]) : m_isInvertible(false) {
		TAffineFrameTarget f2(o,v);
		m_transformation.newsize(TCGAfinneSpaceSource::dim+1,TCGAfinneSpaceSource::dim+1);
		m_inversetransformation.newsize(TCGAfinneSpaceSource::dim+1,TCGAfinneSpaceSource::dim+1);
		cout<<m_transformation;
		TMatrix::multiply(f1.getStdToFrame(), f2.getStdToFrame(),m_transformation);			
		m_isInvertible=m_transformation.inverse(m_inversetransformation);
	}
	/** Construye una transformación a partir de dos frames
	 *	@param f1 frame 1
	 *	@param f2 frame 2
	 */
	TransformationAffine(const TAffineFrameSource& f1,const TAffineFrameTarget& f2) : m_isInvertible(false) {	
		m_transformation.newsize(TCGAfinneSpaceSource::dim+1,TCGAfinneSpaceSource::dim+1);
		m_inversetransformation.newsize(TCGAfinneSpaceSource::dim+1,TCGAfinneSpaceSource::dim+1);			
		TMatrix::multiply(f1.getStdToFrame(), f2.getStdToFrame(),m_transformation);
		m_isInvertible=m_transformation.inverse(m_inversetransformation);			
	}
	/** Construye una transformación a partir de otra
	 *	@param t transformación
	 */
	TransformationAffine(const TransformationAffine<TCGAfinneSpaceSource,TCGAfinneSpaceTarget>& t) : m_isInvertible(false) {
		m_inversetransformation=t.m_inversetransformation;
		m_transformation=t.m_transformation;
		m_isInvertible=true;
	}
	/** Compone dos frames para crear una transformación
	 *	@param f1 frame1 a componer
	 *	@param f2 frame2 a componer
	 */
	void compose(const TAffineFrameSource& f1,const TAffineFrameTarget& f2) {
		if(f1.getIsInvertible() && f2.getIsInvertible()) {
			TMatrix::multiply(f1.getStdToFrame(), f2.getStdToFrame(),m_transformation);
			TMatrix::multiply(f2.getStdToFrame(), f1.getStdToFrame(),m_inversetransformation);
			m_isInvertible=m_transformation.inverse(m_inversetransformation);
		}
	}
	/** Genera la tranformación inversa de un mapa
	 *	@param ta Mapa para encontrar la transformación inversa
	 */
	void inverse(const TransformationAffine<TCGAfinneSpaceSource,TCGAfinneSpaceSource>& ta) {
		if(ta.getIsInvertible()) {
			m_inversetransformation=ta.m_transformation;
			m_transformation=ta.m_inversetransformation;
			m_isInvertible=true;
		}	
	}
	/** Retorna la matriz de transformación inversa.
	 *	@return matriz de transformación inversa
	 */
	const TMatrix& getInverseTransformation() const {
		return m_inversetransformation;
	}
	/** Retorna la matriz de transformación directa
	 *	@return matriz de transformación directa
	 */
	const TMatrix& getTransformation() const {
		return m_inversetransformation;
	}
	/** Transforma el punto p con esta transformación y lo pasa al punto pt
	 *	@param p punto origen
	 *	@patam pt punto transformado
	 */
	void transform(const TAffinePointSource& p,TAffinePointTarget& pt)const {
		TMatrix::multiply(m_transformation,p,pt);
	}
	/** 
	 */
	void transform(const TAffineVectorSource& v,TAffineVectorTarget& vt)const {
		TMatrix::multiply(m_transformation,p,vt);
	}		
private:
	/** Matriz de transformación inversa del espacio destino al espacio origen
	 */
	TMatrix m_inversetransformation;
	/** Matriz de transformación inversa del espacio origen al espacio destino
	 */
	TMatrix m_transformation;
	/** Bandera para marcar si la transformación tiene inversa
	 */
	bool m_isInvertible;

};

CGL_END

#endif
