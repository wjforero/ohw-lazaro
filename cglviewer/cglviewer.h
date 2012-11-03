#ifndef CGLVIEWER_H
#define CGLVIEWER_H

#include "fx.h"
//#include "fx3d.h"

#define SHARE_CONTEXT

#ifdef HAVE_OPENGL

// inclusiones especificas
#include "cglview.h"
#include "cgldocumentmdd.h"

#include "base.h"
#include "../statusbar.h"
#include "../textfield.h"
#include "../colorselector.h"
#include "../colorwell.h"
#include "../alphaballdialog.h"
#include "../imagebutton.h"
#include "../backcomponent.h"


#include "icons.h"


class CglViewer : public CglView, FXMainWindow{
  FXDECLARE(CglViewer)
public:
  virtual void updateView() {

  }
  enum{
    ID_GLVIEWER=FXMainWindow::ID_LAST
    };

public:  
  // Constructor de la aplicación
  CglViewer(FXApp* a);   

  // Inicializa los controles
  virtual void create();
  
  // Destructor
  virtual ~CglViewer();

  enum{
    ID_SHOWDIALOGALPHA=FXMainWindow::ID_LAST
  };

  long onCmdShowDialogAlpha(FXObject*,FXSelector,void*);
  long onPaint(FXObject*,FXSelector,void*);
  long onUpdate(FXObject*,FXSelector,void*);  

protected:
  CglViewer(){};
private:
  // Carga la configuración de diseño
  void createConf();
  void createDialogs();  

  FXGLVisual        *glvisual;                // Visual for OpenGL
  FXGLViewer        *viewer;
  #ifdef SHARE_CONTEXT
    FXGLContext       *glcontext;               // Context for OpenGL
  #endif


  FXColor m_backColor;    
  FXFont* m_font;
  FXFont* m_fontTitle;
  AlphaBallDialog* m_dialogAlpha; 

  // Documento de la aplicación
  CglDocumentMdd m_doc;

  // Dialog de Color
  ColorWell* m_colorWell;
};

FXDEFMAP(CglViewer) MainWindowMap[]={    
  FXMAPFUNC(SEL_COMMAND,  CglViewer::ID_SHOWDIALOGALPHA,CglViewer::onCmdShowDialogAlpha),
};

FXIMPLEMENT(CglViewer,FXMainWindow,MainWindowMap,ARRAYNUMBER(MainWindowMap))

CglViewer::CglViewer(FXApp* a):FXMainWindow(a,"GL Viewer",NULL,NULL,(DECOR_TITLE|DECOR_MINIMIZE|DECOR_CLOSE|DECOR_RESIZE|DECOR_MENU),0,0,700,525){
  // Fija el documento
  setDocument(&m_doc);
  // Crea la configuración de diseño
  createConf();

  ImageButton* imb=0;
  //BackComponent* bc= new BackComponent(this);
  //FXVerticalFrame* frameBack=new FXVerticalFrame(this,LAYOUT_FILL_Y|LAYOUT_FILL_X,0,0,0,0, 0,0,0,0);
  //imb=new ImageButton(frameBack, new FXGIFIcon(getApp(),imaback)); 

  // Frame de operaciones de archivo o creación de nuevos objetos
  FXVerticalFrame* frameViewAndStateOperation=new FXVerticalFrame(this,LAYOUT_SIDE_LEFT|LAYOUT_CENTER_Y|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  imb=new ImageButton(frameViewAndStateOperation, new FXGIFIcon(getApp(),imleft));  
  //
  FXButton* fxb=0;
  FXHorizontalFrame* frameExport=new FXHorizontalFrame(this,LAYOUT_SIDE_TOP|LAYOUT_CENTER_X|LAYOUT_FILL_X,0,0,0,0, 0,0,0,0);
  imb=new ImageButton(frameExport, new FXGIFIcon(getApp(),imtop));  

  FXHorizontalFrame* frameSelect=new FXHorizontalFrame(this,LAYOUT_SIDE_BOTTOM|LAYOUT_CENTER_X|LAYOUT_FILL_X,0,0,0,0, 0,0,0,0);
  imb=new ImageButton(frameSelect, new FXGIFIcon(getApp(),imdown));

  FXVerticalFrame * frameReconstruct=new FXVerticalFrame(this,LAYOUT_SIDE_RIGHT|LAYOUT_CENTER_Y|LAYOUT_FILL_Y,0,0,0,0, 0,0,0,0);
  imb=new ImageButton(frameReconstruct, new FXGIFIcon(getApp(),imright));

  FXVerticalFrame* frameCenter=new FXVerticalFrame(this,LAYOUT_CENTER_X|LAYOUT_CENTER_Y|LAYOUT_FILL_Y|LAYOUT_FILL_X);  

}

CglViewer::~CglViewer(){
  
}

void CglViewer::createConf(){
  m_backColor= FXRGB(10,163,245);
  m_font=new FXFont(getApp(),"arial",8);
  m_fontTitle=new FXFont(getApp(),"helvetica",15);  
}

void CglViewer::createDialogs(){
  // Crea el dialogo de color  
}

void CglViewer::create(){
  FXMainWindow::create();
  show(PLACEMENT_SCREEN);
}


long CglViewer::onCmdShowDialogAlpha(FXObject*,FXSelector,void*ptr){  
  m_dialogAlpha->show();
  return 1;
}

long CglViewer::onPaint(FXObject* ob,FXSelector,void* ptr){
  setBackColor(FXRGB(10,163,245));
  return 1;
}

long CglViewer::onUpdate(FXObject* sender,FXSelector sel,void* ptr){  
  setBackColor(FXRGB(10,163,245));
  return 1;
}



#endif

#endif


