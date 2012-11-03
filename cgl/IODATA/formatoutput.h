#ifndef FORMATOUTPUT_H
#define FORMATOUTPUT_H

// Inclusiones especificas
#include "base.h"

CGL_BEGIN

/** Construye el alpha-shape del conjunto de puntos
*/  
class FormatOutput
{    
  public:
    /** Enumeración para de formato a exportar
    */
    enum{OBJ,MXD,XML};    
  public:
    /**
    */
    FormatOutput(){
      m_modeDraw=MXD;
    }

    /** Fija el modo de dibujo
    *   @param m_modeDraw Modo de dibujo
    */
    virtual void setModeDraw(const unsigned int& modeDraw){
      m_modeDraw=modeDraw;
    }
    /** obtiene el modo de dibujo
    */
    virtual unsigned int getModeDraw(){
      return m_modeDraw;
    }
  protected:
    // Modo de dibujo
    int m_modeDraw;
};

CGL_END

#endif