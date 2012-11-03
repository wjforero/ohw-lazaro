#ifndef INPUTCLOUDPOINTS_H
#define INPUTCLOUDPOINTS_H

#include "input.h"

CGL_BEGIN

/** Modela una entrada de nubes de puntos
*/
class InputCloudPoints: public Input{
  public:    
    InputCloudPoints();    

    unsigned int getNumberPoints();

  protected:
    virtual bool loadData(const string& file)=0;    
    unsigned int n_points;
};

InputCloudPoints::InputCloudPoints():n_points(0){

}

unsigned int InputCloudPoints::getNumberPoints(){
  return n_points;
}

CGL_END

#endif