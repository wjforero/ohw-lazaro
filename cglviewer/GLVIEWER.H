#ifndef GLVIEWER_H
#define GLVIEWER_H

#include "fx.h"
#include "fx3d.h"
#include <stdio.h>

// Si tienen el contexto compartido
#define SHARE_CONTEXT

// Si tiene soporte para OpenGL
#ifdef HAVE_OPENGL

// inclusiones especificas
#include "cglview.h"
#include "cgldocument3d.h"

#include "FXImageButton.h"
#include "FXGLTriangleM3D.h"

#include "statusbar.h"
//#include "icons.h"
#include "frame.h"
#include "buttonview.h"
#include "buttontranslate.h"
#include "buttonrotate.h"
#include "colorwell.h"
#include "skin.h"
#include "sale.h"
#include "packer.h"

#include "alphaballdialog.h"



/** Ventana principal de la aplicación
 */
class GLViewWindow : public FXMainWindow,CglView {
  FXDECLARE(GLViewWindow)
protected:
  /** Constructor
   */
  GLViewWindow(){
  }  
public:
  /** Enumeracion para las acciones
   */
  enum{
    ID_ABOUT=FXMainWindow::ID_LAST,
    ID_GLVIEWER,
		ID_PHONG,
		ID_GORAUD,
		ID_SHADEOFF,
		ID_ZOOMIN,
		ID_ZOOMOUT,
		ID_WIREFRAME,		
		ID_ALPHA,
		ID_KOHONEN,
		ID_SLICES,
		ID_DELAUNAY,
		ID_CONVEXHULL,
		ID_HOPPE,
		ID_POINTS,
		ID_OPENSLICES,
		ID_OPENMESH,
		ID_EXPORTMESH,
		ID_SELECTPOINTS,
		ID_SELECTMESH,
		ID_SELECTDELAUNAY,		
		ID_SELECTCONVEXHULL,
		ID_SELECTOCTREES,
		ID_SETALPHAVALUE,
		ID_ANTTECNIQUE,
		ID_SIGTECNIQUE,		
		ID_ANTTECNIQUECGVOL,
		ID_SIGTECNIQUECGVOL,
		ID_CLEARVIEW,
		ID_COMPUTENORMALS,
  };
  
public:
  /** Constructor
   */
  GLViewWindow(FXApp* a);
  /** Destructor
   */
  virtual ~GLViewWindow();
public:
  /** Crea los recursos
   */
  virtual void create();
public: 
	// Acciones de pintado
  long onPaint(FXObject*,FXSelector,void*);
	long drawPhong(FXObject* obj, FXSelector sel,void* ptr);
	long drawGoraud(FXObject* obj, FXSelector sel,void* ptr);
	long drawShadeOff(FXObject* obj, FXSelector sel,void* ptr);
	long zoomIn(FXObject* obj, FXSelector sel,void* ptr);
	long zoomOut(FXObject* obj, FXSelector sel,void* ptr);
	
	/** Ejecuta una serie de acciones
	*/
	long onAlpha(FXObject* obj, FXSelector sel,void* ptr);	
	long onKohonen(FXObject* obj, FXSelector sel,void* ptr);		
	long onSlices(FXObject* obj, FXSelector sel,void* ptr);			
	long onOpenMesh(FXObject* obj, FXSelector sel,void* ptr);		
	long onDelaunay(FXObject* obj, FXSelector sel,void* ptr);				
	long onConvexHull(FXObject* obj, FXSelector sel,void* ptr);					
	long onHoppe(FXObject* obj, FXSelector sel,void* ptr);					
	long onOpenSlices(FXObject* obj, FXSelector sel,void* ptr);			
	long drawWireFrame(FXObject* obj, FXSelector sel,void* ptr);	
	long onQueryMenu(FXObject* sender,FXSelector,void* ptr);	
	long onUpdAlphaShapeDiag(FXObject* sender,FXSelector,void* ptr);
	long onClearView(FXObject* sender,FXSelector,void* ptr);
	long onComputeNormals(FXObject* sender,FXSelector,void* ptr);	

	// Exporta la malla
	long onExportMesh(FXObject* obj, FXSelector sel,void* ptr);			
	long onCmdAbout(FXObject*,FXSelector,void*);

	// Acciones en el documento
	void createDocument();
	void destroyDocument();
	void drawPhong();
	void drawShadeOff();
	void drawPoints();

	// Acciones de seleccion
	void onSelectedFigure(const unsigned int& type);
	long onSelectPoints(FXObject* obj, FXSelector sel,void* ptr);
	long onSelectMesh(FXObject* obj, FXSelector sel,void* ptr);
	long onSelectDelaunay(FXObject* obj, FXSelector sel,void* ptr);
	long onSelectConvexHull(FXObject* obj, FXSelector sel,void* ptr);	
	long onSelectOctrees(FXObject* obj, FXSelector sel,void* ptr);		

	// Acciones de cambio de tecnica
	long onAntTecniqueReconst(FXObject* obj, FXSelector sel,void* ptr);		
	long onSigTecniqueReconst(FXObject* obj, FXSelector sel,void* ptr);
	long onAntTecniqueCGVol(FXObject* obj, FXSelector sel,void* ptr);		
	long onSigTecniqueCGVol(FXObject* obj, FXSelector sel,void* ptr);
	
	

	// Soporte para skins
	void updateView();
	void iniDefaultValues();
	void iniViewer();
	void loadSkin();	

private:
	void viewPacker(Packer** packers,const unsigned int& indexActualTecnique,const unsigned int& sizetec,const unsigned int casePacker);	  
  FXGLVisual *glvisual;  
  FXGLViewer *viewer;
	FXGLViewer *viewerPoints;
#ifdef SHARE_CONTEXT  
  FXGLContext *glcontext;
#endif
private:
	CglDocument3d* m_document3d;
	FXGLGroup *m_scene;	
	FXGLGroup *m_scenePoints;		
	Packer** m_packerTecniques;
	Packer** m_packerTecniquesCGVol;
	unsigned int SIZERECONSTRUCTIONTECNIQUES;
	unsigned int SIZERECONSTRUCTIONTECNIQUESCGVOL;	
	unsigned int WIDTH_RECONSTRUCTION;
	unsigned int HEIGHT_RECONSTRUCTION;
	unsigned int m_indexActualTecnique;
	unsigned int m_indexActualTecniqueCGVol;	
private:
	// Valor de la fuente
	FXFont* m_font;
	// Valor de la fuente para el titulo
  FXFont* m_fontTitle;
	// Color de fondo
	FXColor m_backColor;
	// Color de fondo
	FXColor m_fontColor;
	// Color del menu
	FXColor m_menuColor;
	// Color de menu
	FXColor m_backColorDialog;
	Skin* m_skin;
	static const char* m_tecniquesRecons[5];
	static const char* m_tecniquesGeom[5];
	FXLabel* m_labelTecnique;
	FXLabel* m_labelTecniqueRecons;		
	FXLabel* m_labelTecniqueGeom;		

	void viewTimeElapsed(CglDocument3d& cgld);
};
#endif

const char* GLViewWindow::m_tecniquesRecons[5]={"ALPHA-SHAPE","KOHONEN","HOPPE","SLICES","OCTDEL"};
const char* GLViewWindow::m_tecniquesGeom[5]={"DELAUNAY","CONVEXHULL","NORMALES","SLICES","OCTDEL"};

#endif