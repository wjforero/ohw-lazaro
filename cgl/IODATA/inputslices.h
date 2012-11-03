#ifndef INPUTSLICES_H
#define INPUTSLICES_H

// Inclusiones especificas
#include "inputcloudpoints.h"
#include "formats/polygonalslice/cloudslice.h"

CGL_BEGIN

/** Modela una malla m3d
*/
class InputSlices: public InputCloudPoints{
  public:
    typedef CGAffineSpace<float,3> Tfloat3DSpace;
    typedef CloudSlice<Tfloat3DSpace> TCloudSlice;
    typedef TCloudSlice::TVectorSlices TVectorSlices;
    typedef TCloudSlice::IterVectorSlices IterVectorSlices;
  public:
    /** Constructor por omisión
    */
    InputSlices();        
    /** Destructor
    */
    ~InputSlices();
    /** Retorna la referencia a los slices
    */
    IterVectorSlices getVectorSlices();
    /** Fija el vector en el cual se almacenan los puntos
    */
    void setVectorSlices(IterVectorSlices points);    
    
  protected:
    /** Carga los datos de los puntos
    */
    bool loadData(const string& file);
  private:
    IterVectorSlices m_vectorSlices;
};

InputSlices::InputSlices():m_vectorSlices(0){

}

InputSlices::~InputSlices(){

}

InputSlices::IterVectorSlices InputSlices::getVectorSlices(){
  return m_vectorSlices;
}

void InputSlices::setVectorSlices(IterVectorSlices vectorSlices){
  m_vectorSlices=vectorSlices;
}
    
bool InputSlices::loadData(const string& file){
  if(m_vectorSlices==0){
    EXCEPTION("El vector de almacenamiento es NULL");
    return false;
  }
  m_vectorSlices->clear();
  TCloudSlice cs;
  cs.loadModelSlices(file,1,*m_vectorSlices);
  return true;
}

CGL_END

#endif