/**
 * $Revision$
 * $Date$
 */

#ifndef BASE_H
#define BASE_H

/** Definición del namespace ohw
 */
#ifndef OHW_BEGIN
#define OHW_BEGIN namespace ohw {
#endif
#ifndef OHW_END
#define OHW_END };
#endif
#ifndef OHW_
#define OHW_ ohw::
#endif

/** Definición del namespace ohw::cgl
 */
#ifndef CGL_BEGIN
#define CGL_BEGIN OHW_BEGIN namespace cgl { 
#endif
#ifndef CGL_END
#define CGL_END }; OHW_END
#endif
#ifndef CGL_
#define CGL_ OHW_cgl::
#endif

/** Definición del namespace ohw::numerical
 */
#ifndef NUM_BEGIN
#define NUM_BEGIN OHW_BEGIN namespace numerical { 
#endif
#ifndef NUM_END
#define NUM_END }; OHW_END
#endif
#ifndef NUM_
#define NUM_ OHW_cgl::
#endif

/** Definición del namespace ohw::structures
 * Este espacio de nombres contiene las estructuras de 
 * datos genéricas de la libreria.
 */
#ifndef ST_BEGIN
#define ST_BEGIN OHW_BEGIN namespace structures { 
#endif
#ifndef ST_END
#define ST_END }; OHW_END
#endif
#ifndef ST_
#define ST_ OHW_cgl::
#endif

//#include "NTL/ZZ.h"
//#include <iostream.h>

#include <iostream>
#include <vector>
#include <list>
using namespace std;

#define EXCEPTION(a) cout<<a
#define CHECKMEMORY(a)

#ifndef PI
#define PI 3.14159265
#endif

#ifndef OHWDEBUG
#define OHWDEBUG 1
#endif

#endif
