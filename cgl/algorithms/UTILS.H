#ifndef UTILS_H
#define UTILS_H

CGL_BEGIN

#include <vector>
using std::vector;

/** Función template para barajar un vector
*/
template <class T>
void shuffle(vector<T>& vp, const unsigned int& seed){
	unsigned int i,j;
	srand(seed);
	for(i=0;i<vp.size();i++){		
		j=(int) ((vp.size()-1)*(((float)rand()/RAND_MAX)));
		T t = vp[i];
		vp[i] = vp[j];
		vp[j] = t;
	}	
}

CGL_END


#endif