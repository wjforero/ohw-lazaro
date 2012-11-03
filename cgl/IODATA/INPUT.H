#ifndef INPUT_H
#define INPUT_H

#include "base.h"

CGL_BEGIN


/** Modela una entrada de disco
*/
class Input{
  public:
		/** Carga los datos
		*/
    bool load(const string& file);

  protected:
		/** Carga los datos
		*/
    virtual bool loadData(const string& file)=0;    
};

bool Input::load(const string& file){    
  return loadData(file);
}

CGL_END

#endif