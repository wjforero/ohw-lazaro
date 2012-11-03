#ifndef ALGEBRA_H
#define ALGEBRA_H

#include "base.h"
#include "numericalvector.h"
#include "matrix.h"
#include <math.h>

NUM_BEGIN

#define ROTATE(a,i,j,k,l) g=a(i,j);h=a(k,l);a(i,j)=g-s*(h+g*tau);a(k,l)=h+s*(g-h*tau);

/** Esta clase modela las distintas operaciones de un algebra
 */
template<class T>
class Algebra {
public:
	/**
	 */
	typedef typename Matrix<T> TMatrix;
	/**
	 */
	typedef typename NumericalVector<T> TVector;
public:
	/** Calcula los vectores y valores propios de una matriz real simetrica
	 * @param a la matriz origen
	 */
	static eigenValues(TMatrix& a, TVector& d) {
	}

	/**
	 * @param a entrada
	 * @param d valores propios
	 * @param v vectores propios 
	 * @param nrot número de rotaciones
	 * @param n dimension de la matriz
	 */
	static void jacobi(TMatrix& a, TVector& d, TMatrix& v, int& nrot, const unsigned int& n) {		
    int j,iq,ip,i;
		float tresh,theta,tau,t,sm,s,h,g,c;
		TVector b(n+1);
		TVector z(n+1);
		for (ip=1;ip<=n;ip++) { //Initialize to the identity matrix.
			for (iq=1;iq<=n;iq++) {
				v(ip,iq)=0.0;
			}
			v(ip,ip)=1.0;
		}
    for (ip=1;ip<=n;ip++) { //Initialize b and d to the diagonal of a.       
			b[ip]=d[ip]=a(ip,ip);
			z[ip]=0.0; //This vector will accumulate terms of the form tapq as in equation (11.1.14).
		}
		nrot=0;
		for (i=1;i<=50;i++) {
			sm=0.0;
			for (ip=1;ip<=n-1;ip++) { //Sum of-diagonal elements.
				for (iq=ip+1;iq<=n;iq++) {
					sm += fabs(a(ip,iq));
				}
			}
			if (sm == 0.0) { //The normal return, which relies on quadratic convergence to machine underflow.
				return;
			}
			if (i < 4)
				tresh=0.2*sm/(n*n);
			else
				tresh=0.0; 
			for (ip=1;ip<=n-1;ip++) {
				for (iq=ip+1;iq<=n;iq++) {
					g=100.0*fabs(a(ip,iq));	
					if ( i > 4 && 
							(float)(fabs(d[ip])+g) == (float)fabs(d[ip]) && 
							(float)(fabs(d[iq])+g) == (float)fabs(d[iq]) )
						a(ip,iq)=0.0;

						else if (fabs(a(ip,iq)) > tresh) {
							h=d[iq]-d[ip];
							if ((float)(fabs(h)+g) == (float)fabs(h))
								t=(a(ip,iq))/h; 
							else {
								theta=0.5*h/(a(ip,iq)); 
								t=1.0/(fabs(theta)+sqrt(1.0+theta*theta));
								if (theta < 0.0) 
									t = -t;
							}
							c=1.0/sqrt(1+t*t);
							s=t*c;
							tau=s/(1.0+c);
							h=t*a(ip,iq);
							z[ip] -= h;
							z[iq] += h;
							d[ip] -= h;
							d[iq] += h;
							a(ip,iq)=0.0;
							for (j=1;j<=ip-1;j++) {			
								ROTATE(a,j,ip,j,iq)
							}
							for (j=ip+1;j<=iq-1;j++) {	
								ROTATE(a,ip,j,j,iq)
							}
							for (j=iq+1;j<=n;j++) {			
								ROTATE(a,ip,j,iq,j)
							}
							for (j=1;j<=n;j++) {
								ROTATE(v,j,ip,j,iq)
							}
							++(nrot);
						}
					}
				}
				for (ip=1;ip<=n;ip++) {
					b[ip] += z[ip];
					d[ip]=b[ip];								//Update d with the sum of tapq,
					z[ip]=0.0;									//and reinitialize z.
				}
			}
		cout<<("Too many iterations in routine jacobi")<<endl;
	};
	/** ordena los valores propios y sus vectores en orden descendente
	 */ 
	static void eigsrt(TVector& d, TMatrix& v, const unsigned int& n) {
    //cout<<"eigsrt"<<endl;
		int k,j,i;
		float p;
		for (i=1;i<n;i++) {
			p=d[k=i];
			for (j=i+1;j<=n;j++)
				if (d[j] >= p) 
					p=d[k=j];
			if (k != i) {
				d[k]=d[i];
				d[i]=p;
				for (j=1;j<=n;j++) {
					p=v(j,i);
					v(j,i)=v(j,k);
					v(j,k)=p;
				}
			}
		}
	};

};

NUM_END

#endif