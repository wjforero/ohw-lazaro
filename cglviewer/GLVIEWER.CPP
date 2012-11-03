#include "glviewer.h"

// Mapa de mensajes
FXDEFMAP(GLViewWindow) GLViewWindowMap[] = {
  // Tipo de mensaje					ID														Manejador del mensaje
  FXMAPFUNC(SEL_COMMAND,     GLViewWindow::ID_ABOUT,			GLViewWindow::onCmdAbout),
  FXMAPFUNC(SEL_PAINT,       0,														GLViewWindow::onPaint),
	FXMAPFUNC(SEL_COMMAND,     GLViewWindow::ID_PHONG,			GLViewWindow::drawPhong),
	FXMAPFUNC(SEL_COMMAND,     GLViewWindow::ID_GORAUD,			GLViewWindow::drawGoraud),
	FXMAPFUNC(SEL_COMMAND,     GLViewWindow::ID_SHADEOFF,		GLViewWindow::drawShadeOff),
	FXMAPFUNC(SEL_COMMAND,     GLViewWindow::ID_ZOOMIN,			GLViewWindow::zoomIn),
	FXMAPFUNC(SEL_COMMAND,     GLViewWindow::ID_ZOOMOUT,		GLViewWindow::zoomOut),
	FXMAPFUNC(SEL_COMMAND,     GLViewWindow::ID_WIREFRAME,	GLViewWindow::drawWireFrame),
	FXMAPFUNC(SEL_COMMAND,		 GLViewWindow::ID_ALPHA,			GLViewWindow::onAlpha),
	FXMAPFUNC(SEL_UPDATE,      GLViewWindow::ID_ALPHA,			GLViewWindow::onUpdAlphaShapeDiag),
	FXMAPFUNC(SEL_COMMAND,		 GLViewWindow::ID_KOHONEN,		GLViewWindow::onKohonen),		
	FXMAPFUNC(SEL_COMMAND,		 GLViewWindow::ID_SLICES,			GLViewWindow::onSlices),	
	FXMAPFUNC(SEL_COMMAND,		 GLViewWindow::ID_DELAUNAY,		GLViewWindow::onDelaunay),		
	FXMAPFUNC(SEL_COMMAND,		 GLViewWindow::ID_CONVEXHULL,	GLViewWindow::onConvexHull),
	FXMAPFUNC(SEL_COMMAND,		 GLViewWindow::ID_HOPPE,			GLViewWindow::onHoppe),	
	FXMAPFUNC(SEL_COMMAND,		 GLViewWindow::ID_OPENMESH,		GLViewWindow::onOpenMesh),		
	FXMAPFUNC(SEL_COMMAND,		 GLViewWindow::ID_OPENSLICES,	GLViewWindow::onOpenSlices),

	FXMAPFUNC(SEL_COMMAND,		 GLViewWindow::ID_SELECTPOINTS,	GLViewWindow::onSelectPoints),
	FXMAPFUNC(SEL_COMMAND,		 GLViewWindow::ID_SELECTMESH,	GLViewWindow::onSelectMesh),
	FXMAPFUNC(SEL_COMMAND,		 GLViewWindow::ID_SELECTDELAUNAY,	GLViewWindow::onSelectDelaunay),
	FXMAPFUNC(SEL_COMMAND,		 GLViewWindow::ID_SELECTCONVEXHULL,	GLViewWindow::onSelectDelaunay),
	FXMAPFUNC(SEL_COMMAND,		 GLViewWindow::ID_SELECTOCTREES,	GLViewWindow::onSelectDelaunay),		


	FXMAPFUNC(SEL_COMMAND,     FXWindow::ID_QUERY_MENU,	  GLViewWindow::onQueryMenu),

	FXMAPFUNC(SEL_COMMAND,     GLViewWindow::ID_ANTTECNIQUE,	  GLViewWindow::onAntTecniqueReconst),
	FXMAPFUNC(SEL_COMMAND,     GLViewWindow::ID_SIGTECNIQUE,	  GLViewWindow::onSigTecniqueReconst),

	FXMAPFUNC(SEL_COMMAND,     GLViewWindow::ID_ANTTECNIQUECGVOL,	  GLViewWindow::onAntTecniqueCGVol),
	FXMAPFUNC(SEL_COMMAND,     GLViewWindow::ID_SIGTECNIQUECGVOL,	  GLViewWindow::onSigTecniqueCGVol),
	FXMAPFUNC(SEL_COMMAND,     GLViewWindow::ID_EXPORTMESH,	  GLViewWindow::onExportMesh),

	FXMAPFUNC(SEL_COMMAND,     GLViewWindow::ID_CLEARVIEW,	  GLViewWindow::onClearView),	
	FXMAPFUNC(SEL_COMMAND,     GLViewWindow::ID_COMPUTENORMALS,	  GLViewWindow::onComputeNormals),	
	
};

// Implementación
FXIMPLEMENT(GLViewWindow,FXMainWindow,GLViewWindowMap,ARRAYNUMBER(GLViewWindowMap))

/**
 */
GLViewWindow::GLViewWindow(FXApp* a) 
  : FXMainWindow(a,"Reconstruction",NULL,NULL,DECOR_NONE,0,0,720,525) {	
	
	// Configuración de los valores de omisión
	iniDefaultValues();
	m_labelTecnique=0;

	// Fija el color de fondo
  setBackColor(m_backColor);	
	
	// Inicializa el visor de objetos 3d
	iniViewer();  

	m_skin=new Skin();
	m_skin->setPathSkin("skin1/");
	// Carga el skin
	loadSkin();
	
	// Construye el panel izquierdo con los datos de camaras
	// Fija el boton de la vista	
	int dy=60;
	ButtonView* bview = new ButtonView(0,viewer,this,"\tCAMARAS\tControl\nSelecciona vista", 0,0,LAYOUT_EXPLICIT, 29,90+dy,90,90);
	bview->setKeySkin("viewicon");
	bview->setValSkin(m_skin);

	// Fija los botones de translacion
	ButtonTranslate* imbutras = new ButtonTranslate(viewer,this,"\tCAMARAS\tControl\nDesplaza en y",0, 0, 0, LAYOUT_EXPLICIT, 63,189+dy,28,23);
	imbutras->setOptionTranslate(ButtonTranslate::TRANSLATINGY);
	imbutras->setKeySkin("buttrasy");
	imbutras->setValSkin(m_skin);		
	imbutras = new ButtonTranslate(viewer,this,"\tCAMARAS\tControl\nDesplaza en x",0, 0, 0, LAYOUT_EXPLICIT, 100,247+dy,23,23);
	imbutras->setOptionTranslate(ButtonTranslate::TRANSLATINGX);
	imbutras->setKeySkin("buttrasx");
	imbutras->setValSkin(m_skin);
	imbutras = new ButtonTranslate(viewer,this,"\tCAMARAS\tControl\nDesplaza en z",0, 0, 0, LAYOUT_EXPLICIT, 29,249+dy,23,23);
	imbutras->setOptionTranslate(ButtonTranslate::TRANSLATINGZ);
	imbutras->setKeySkin("buttrasz");
	imbutras->setValSkin(m_skin);

	imbutras = new ButtonTranslate(viewer,this,"\tCAMARAS\tControl\nDesplaza en xy",0, 0, 0, LAYOUT_EXPLICIT, 65,230+dy,23,23);
	imbutras->setOptionTranslate(ButtonTranslate::TRANSLATINGXY);
	imbutras->setKeySkin("buttrasxy");
	imbutras->setValSkin(m_skin);

	FXImageButton* imbu=NULL;
	dy=50;

	imbu = new ButtonRotate(viewer,this,"\tCAMARAS\tControl\nRota",0, 0, 0, LAYOUT_EXPLICIT, 30,287+dy,91,87);
	imbu->setKeySkin("buttrot");
	imbu->setValSkin(m_skin);
	
  imbu = new FXImageButton(this,"\tCAMARAS\tRender\nPhong", 0 ,this,ID_PHONG,LAYOUT_EXPLICIT,49,377+dy,44,44);
	imbu->setKeySkin("butphong");
	imbu->setValSkin(m_skin);
	
  imbu = new FXImageButton(this,"\tCAMARAS\tRender\nGouraud", 0,this,ID_GORAUD,LAYOUT_EXPLICIT,20,384+dy,30,30);  
	imbu->setKeySkin("butgoraud");
	imbu->setValSkin(m_skin);
	
  imbu = new FXImageButton(this,"\tCAMARAS\tRender\nPlano", 0,this,ID_SHADEOFF,LAYOUT_EXPLICIT,94,384+dy,30,30);
	imbu->setKeySkin("butrederplane");
	imbu->setValSkin(m_skin);

  Statusbar* tStatusbar=new Statusbar(this,LAYOUT_EXPLICIT,0,460,145,68);
  tStatusbar->setBackColor(m_backColor);
	tStatusbar->setTextColor(m_fontColor);
  tStatusbar->setFont(m_font,m_fontTitle);  

	int inic=150;	
  imbu = new FXImageButton(this,"\tVISUALIZACION\tVisualización\nVisualiza malla", 0,this,ID_WIREFRAME,LAYOUT_EXPLICIT,170,490,20,20);  
	imbu->setKeySkin("butviewmesh");
	imbu->setValSkin(m_skin);
	imbu = new FXImageButton(this,"\tLIMPIEZA\tLimpieza\nLimpia las mallas actuales", 0,this,GLViewWindow::ID_CLEARVIEW,LAYOUT_EXPLICIT,190,490,20,20);
	imbu->setKeySkin("butclean");
	imbu->setValSkin(m_skin);		
  
	ColorWell* cw=new ColorWell(this,0,viewer,FXGLViewer::ID_BACK_COLOR,COLORWELL_OPAQUEONLY|LAYOUT_EXPLICIT,230,490,18,19,0,0,0,0,m_skin);
	cw->setHelpText("\tColor\nColor de fondo");
	cw=new ColorWell(this,0,viewer,FXGLViewer::ID_AMBIENT_COLOR,COLORWELL_OPAQUEONLY|LAYOUT_EXPLICIT,250,490,18,19,0,0,0,0,m_skin);
	cw->setHelpText("\tColor\nColor de ambiente");
	cw=new ColorWell(this,0,viewer,FXGLViewer::ID_LIGHT_AMBIENT,COLORWELL_OPAQUEONLY|LAYOUT_EXPLICIT,270,490,18,19,0,0,0,0,m_skin);

	cw->setHelpText("\tColor\nLuz ambiente");
	cw=new ColorWell(this,0,viewer,FXGLViewer::ID_LIGHT_DIFFUSE,COLORWELL_OPAQUEONLY|LAYOUT_EXPLICIT,290,490,18,19,0,0,0,0,m_skin);
	cw->setHelpText("\tColor\nLuz difusa");
	cw=new ColorWell(this,0,viewer,FXGLViewer::ID_LIGHT_SPECULAR,COLORWELL_OPAQUEONLY|LAYOUT_EXPLICIT,310,490,18,19,0,0,0,0,m_skin);	
	cw->setHelpText("\tColor\nLuz difusa");	  

  imbu = new FXImageButton(this,"\tIMPORTAR\tImportar\nImporta malla", 0,this,ID_OPENMESH,LAYOUT_EXPLICIT,140,-10,79,79);
	imbu->setKeySkin("butimportmesh");
	imbu->setValSkin(m_skin);
  imbu = new FXImageButton(this,"\tIMPORTAR\tImportar\nImporta slices", 0,this,ID_OPENSLICES,LAYOUT_EXPLICIT,230,-10,79,79);
	imbu->setKeySkin("butimportslices");
	imbu->setValSkin(m_skin);
	
	imbu = new FXImageButton(this,"\tEXPORTAR\tExportar\nExporta la malla", 0,this,ID_EXPORTMESH,LAYOUT_EXPLICIT,320,-10,79,79);
	imbu->setKeySkin("butexportmesh");
	imbu->setValSkin(m_skin);

  // laboratorios de reconstrucción
  imbu = new FXImageButton(this,"\tLABORATORIO\tReconstrucción\nAnt técnica", 0,this,ID_ANTTECNIQUE,LAYOUT_EXPLICIT,546,225,40,23);
	imbu->setKeySkin("butanttecre");
	imbu->setValSkin(m_skin);
  imbu = new FXImageButton(this,"\tLABORATORIO\tReconstrucción\nSig técnica", 0,this,ID_SIGTECNIQUE,LAYOUT_EXPLICIT,682,225,40,23);
	imbu->setKeySkin("butsigtecre");
	imbu->setValSkin(m_skin);

	m_labelTecniqueRecons=new FXLabel(this, "", 0, LAYOUT_EXPLICIT, 590,225,90,23);
	m_labelTecniqueRecons->setFont(m_font);
	m_labelTecniqueRecons->setTextColor(m_fontColor);
	m_labelTecniqueRecons->setBackColor(m_backColor);			  

	SIZERECONSTRUCTIONTECNIQUES=4;	
	m_packerTecniques=new Packer*[SIZERECONSTRUCTIONTECNIQUES];
	WIDTH_RECONSTRUCTION=160;
	HEIGHT_RECONSTRUCTION=160;	
	unsigned int cx=546,cy=61;	
	
	// Crea el formulario del alpha shape
	Packer* ft=0;
	FXLabel* l=0;
	TextField* m_tfalpha=0;
	ft=new Packer(this, LAYOUT_EXPLICIT, cx,cy,WIDTH_RECONSTRUCTION,HEIGHT_RECONSTRUCTION);
	ft->setHelpText("\tAlpha\nAlpha-shape");
	ft->setKeySkin("backalphashape");
	ft->setValSkin(m_skin);	

	m_tfalpha=new TextField(ft,6,this,GLViewWindow::ID_ALPHA,TEXTFIELD_NORMAL|LAYOUT_EXPLICIT,80,75,40,20);
	m_tfalpha->setHelpText("Reconstrucción\nAlpha");
  m_tfalpha->setTextColor(m_backColor);
	m_tfalpha->setKeySkin("icontextalpha");
	m_tfalpha->setValSkin(m_skin);

	imbu = new FXImageButton(ft,"\tOK\tReconstruye Alpha", 0,m_tfalpha,SEL_COMMAND,LAYOUT_EXPLICIT,130,135,30,30);
	imbu->setKeySkin("butrecalpha");
	imbu->setValSkin(m_skin);	
	ft->setWidth(0);	

	m_packerTecniques[0]=ft;
	
	// Crea el formulario de kohonen
	ft=new Packer(this, LAYOUT_EXPLICIT, cx,cy,WIDTH_RECONSTRUCTION,HEIGHT_RECONSTRUCTION);
	ft->setHelpText("\tKOHONEN\nKohonen");
	ft->setKeySkin("backkohonen");
	ft->setValSkin(m_skin);		
	m_tfalpha=new TextField(ft,6,this,GLViewWindow::ID_KOHONEN,TEXTFIELD_NORMAL|LAYOUT_EXPLICIT,56,65,40,20);	
  m_tfalpha->setHelpText("Reconstrucción\nEscala");
  m_tfalpha->setTextColor(m_fontColor);
	m_tfalpha->setBackColor(m_backColor);
	imbu = new FXImageButton(ft,"\tOK\tReconstruye Kohonen", 0,m_tfalpha,SEL_COMMAND,LAYOUT_EXPLICIT,130,135,30,30);
	imbu->setKeySkin("butrecalpha");	
	imbu->setValSkin(m_skin);
	ft->setWidth(0);	

	m_packerTecniques[1]=ft;

	// Crea el formulario de hoppe
	ft=new Packer(this, LAYOUT_EXPLICIT, cx,cy,WIDTH_RECONSTRUCTION,HEIGHT_RECONSTRUCTION);	
	ft->setHelpText("\tHoppe\nHoppe");
	ft->setKeySkin("backhoppe");
	ft->setValSkin(m_skin);
	m_tfalpha=new TextField(ft,6,this,GLViewWindow::ID_HOPPE,TEXTFIELD_NORMAL|LAYOUT_EXPLICIT,0,120,60,20);
  m_tfalpha->setHelpText("Reconstrucción\nDensidad");
  m_tfalpha->setTextColor(m_fontColor);
	m_tfalpha->setBackColor(m_backColor);
	imbu = new FXImageButton(ft,"\tOK\tReconstruye con Hoppe", 0,m_tfalpha,SEL_COMMAND,LAYOUT_EXPLICIT,130,135,30,30);
	imbu->setKeySkin("butrecalpha");
	imbu->setValSkin(m_skin);

	m_packerTecniques[2]=ft;

	// Crea el formulario de kohonen
	ft=new Packer(this, LAYOUT_EXPLICIT, cx,cy,WIDTH_RECONSTRUCTION,HEIGHT_RECONSTRUCTION);
	ft->setHelpText("\tSLICES\nSlices");
	ft->setKeySkin("backslices");
	ft->setValSkin(m_skin);	
	imbu = new FXImageButton(ft,"\tOK\tReconstruye con slices", 0,this,ID_SLICES,LAYOUT_EXPLICIT,130,135,30,30);
	imbu->setKeySkin("butrecalpha");
	imbu->setValSkin(m_skin);
	m_packerTecniques[3]=ft;	
		
	m_indexActualTecnique=0;
	m_labelTecnique=m_labelTecniqueRecons;
	viewPacker(m_packerTecniques,m_indexActualTecnique,SIZERECONSTRUCTIONTECNIQUES,0);

	imbu = new FXImageButton(this,"\tLABORATORIO\tReconstrucción\nAnt técnica", 0,this,ID_ANTTECNIQUECGVOL,LAYOUT_EXPLICIT,546,437,40,23);
	imbu->setKeySkin("butanttec");
	imbu->setValSkin(m_skin);
  imbu = new FXImageButton(this,"\tLABORATORIO\tReconstrucción\nSig técnica", 0,this,ID_SIGTECNIQUECGVOL,LAYOUT_EXPLICIT,682,437,40,23);
	imbu->setKeySkin("butsigtec");
	imbu->setValSkin(m_skin);

	m_labelTecniqueGeom=new FXLabel(this, "", 0, LAYOUT_EXPLICIT, 590,437,90,23);
	m_labelTecniqueGeom->setFont(m_font);
	m_labelTecniqueGeom->setTextColor(m_fontColor);
	m_labelTecniqueGeom->setBackColor(m_backColor);	

	SIZERECONSTRUCTIONTECNIQUESCGVOL=3;	
	m_packerTecniquesCGVol=new Packer*[SIZERECONSTRUCTIONTECNIQUESCGVOL];
	cx=546;cy=280;	

	// Crea el formulario de Delaunay	
	ft=new Packer(this, LAYOUT_EXPLICIT, cx,cy,WIDTH_RECONSTRUCTION,HEIGHT_RECONSTRUCTION);
	ft->setHelpText("\tDelanuay\nDelaunay");
	ft->setKeySkin("backdelaunay");
	ft->setValSkin(m_skin);		

	imbu = new FXImageButton(ft,"\tOK\tCalcula Delaunay", 0,this,ID_DELAUNAY,LAYOUT_EXPLICIT,130,135,30,30);
	imbu->setKeySkin("butrecalpha");
	imbu->setValSkin(m_skin);
	m_packerTecniquesCGVol[0]=ft;	

	// Crea el formulario de ConvexHull
	ft=new Packer(this, LAYOUT_EXPLICIT, cx,cy,WIDTH_RECONSTRUCTION,HEIGHT_RECONSTRUCTION);
	ft->setHelpText("\tEnvolvente\nEnvolvente");
	ft->setKeySkin("backconvexhull");
	ft->setValSkin(m_skin);		
	imbu = new FXImageButton(ft,"\tOK\tEnvolvente Convexa", 0,this,ID_CONVEXHULL,LAYOUT_EXPLICIT,130,135,30,30);
	imbu->setKeySkin("butrecalpha");
	imbu->setValSkin(m_skin);
	
	m_packerTecniquesCGVol[1]=ft;


	// Crea el formulario de las normales
	ft=new Packer(this, LAYOUT_EXPLICIT, cx,cy,WIDTH_RECONSTRUCTION,HEIGHT_RECONSTRUCTION);
	ft->setHelpText("\tNormales\nNormales");
	ft->setKeySkin("backnormals");
	ft->setValSkin(m_skin);		
	imbu = new FXImageButton(ft,"\tOK\tNormales", 0,this,ID_COMPUTENORMALS,LAYOUT_EXPLICIT,130,135,30,30);
	imbu->setKeySkin("butrecalpha");
	imbu->setValSkin(m_skin);

	m_tfalpha=new TextField(ft,6,this,GLViewWindow::ID_COMPUTENORMALS,TEXTFIELD_NORMAL|LAYOUT_EXPLICIT,59,71,40,20);
  m_tfalpha->setHelpText("Normales\nDensidad");
  m_tfalpha->setTextColor(m_fontColor);
	m_tfalpha->setBackColor(m_backColor);

	m_packerTecniquesCGVol[2]=ft;

	imbu = new FXImageButton(this,"\tSale\tSale", 0,getApp(),FXApp::ID_QUIT,LAYOUT_EXPLICIT,0,0,30,30);
	ft->setHelpText("\tEnvolvente\nEnvolvente");
	imbu->setKeySkin("butquit");	
	imbu->setValSkin(m_skin);

	


	m_indexActualTecniqueCGVol=0;
	m_labelTecnique=m_labelTecniqueGeom;
	viewPacker(m_packerTecniquesCGVol,m_indexActualTecniqueCGVol,SIZERECONSTRUCTIONTECNIQUESCGVOL,1);

	createDocument();
}

/** Inicializa los valores por omisión
*/
void GLViewWindow::iniDefaultValues(){
	m_backColor = FXRGB(0,0,0);
	m_fontColor = FXRGB(255,255,255);
	m_menuColor = FXRGB(255,255,255);
	m_backColorDialog = FXRGB(40,40,40);
	m_font=new FXFont(getApp(),"arial",8);
  m_fontTitle=new FXFont(getApp(),"helvetica",15);
}

/** Incializa el visor de objetos
*/
void GLViewWindow::iniViewer(){
	glvisual=new FXGLVisual(getApp(),VISUAL_DOUBLEBUFFER);  
  viewer=new FXGLViewer(this,glvisual,this,ID_GLVIEWER,LAYOUT_EXPLICIT,147,60,390,420);  	
	m_scene=new FXGLGroup();
	m_scenePoints=new FXGLGroup();
	viewerPoints = new FXGLViewer(this,glvisual,this,ID_GLVIEWER,LAYOUT_EXPLICIT,25,40,90,90);
	viewer->setScene(m_scene);	
	viewerPoints ->setScene(m_scenePoints);	

	viewer->setBackgroundColor(FXHVec(FXRGB(30,30,30)));
	viewerPoints->setBackgroundColor(FXHVec(FXRGB(30,30,30)));
}

/** Carga el skin
*/
void GLViewWindow::loadSkin(){		
	m_skin->setApp(getApp());	
	m_skin->loadIconsResource("viewicon0","viewicon0");
	m_skin->loadIconsResource("viewicon1","viewicon1");
	m_skin->loadIconsResource("viewicon2","viewicon2");
	m_skin->loadIconsResource("viewicon3","viewicon3");
	m_skin->loadIconsResource("viewicon4","viewicon4");
	m_skin->loadIconsResource("viewicon5","viewicon5");	
	// Carga el skin del icono de atras	
	m_skin->loadIconsButton("buttrot");	
	m_skin->loadIconsButton("buttras");
	m_skin->loadIconsButton("buttrasx");	
	m_skin->loadIconsButton("buttrasxy");	
	m_skin->loadIconsButton("buttrasy");
	m_skin->loadIconsButton("buttrasz");	
	m_skin->loadIconsButton("butphong");	
	m_skin->loadIconsButton("butgoraud");	
	m_skin->loadIconsButton("butrederplane");	
	m_skin->loadIconsButton("butselpoint");	
	m_skin->loadIconsButton("butselmesh");	
	m_skin->loadIconsButton("butseldel");	
	m_skin->loadIconsButton("butselconvexhull");	
	m_skin->loadIconsButton("butseloctrees");	
	m_skin->loadIconsButton("butselfirst");	
	m_skin->loadIconsButton("butsellast");	
	m_skin->loadIconsButton("butselnext");	
	m_skin->loadIconsButton("butselprev");	
	m_skin->loadIconsButton("butvispoints");
	m_skin->loadIconsButton("butvismesh");
	m_skin->loadIconsButton("butfitpoints");
	m_skin->loadIconsButton("butzoomin");
	m_skin->loadIconsButton("butzoomout");
	m_skin->loadIconsButton("butimportmesh");
	m_skin->loadIconsButton("butimportslices");
	m_skin->loadIconsButton("butexportmesh");
	m_skin->loadIconsButton("butexportmesh");
	m_skin->loadIconsButton("butanttecre");
	m_skin->loadIconsButton("butsigtecre");
	m_skin->loadIconsButton("butquit");
	m_skin->loadIconsButton("butclean");	
	m_skin->loadIconsResource("backalphashape","backalphashape");
	m_skin->loadIconsButton("butrecalpha");
	m_skin->loadIconsResource("icontextalpha","icontextalpha");

	m_skin->loadIconsResource("backkohonen","backkohonen");	
	m_skin->loadIconsButton("butsigteckoho");	
	m_skin->loadIconsButton("butviewmesh");		

	m_skin->loadIconsResource("backhoppe","backhoppe");	
	m_skin->loadIconsButton("butsighoppe");
	
	m_skin->loadIconsResource("backslices","backslices");	
	m_skin->loadIconsButton("butslicesrec");

	m_skin->loadIconsButton("butanttec");
	m_skin->loadIconsButton("butsigtec");
	m_skin->loadIconsButton("butreckoho");		
	
	m_skin->loadIconsButton("butcancelcolorselector");
	m_skin->loadIconsButton("butokcolorselector");

	

	m_skin->loadIconsResource("backdelaunay","backdelaunay");	
	m_skin->loadIconsResource("backconvexhull","backconvexhull");	
	m_skin->loadIconsResource("backnormals","backnormals");	
	

	m_skin->setBackColorDialog(m_backColorDialog);
}


/** Destructor
 */
GLViewWindow::~GLViewWindow(){	
}

/** Crea e inicializa
 */
void GLViewWindow::create(){
  FXMainWindow::create();
  show(PLACEMENT_SCREEN);
}

/**
 */
long GLViewWindow::onPaint(FXObject* obj, FXSelector sel,void* ptr){
  FXMainWindow::onPaint( obj, sel, ptr);
	
	FXEvent *ev=(FXEvent*)ptr;	
  FXDCWindow dc(this,ev);

	dc.drawIcon(m_skin->getIcon("buttrasdisable"),0,-20);
	return 1;
}

/** About
 */
long GLViewWindow::onCmdAbout(FXObject*,FXSelector,void*){
  FXMessageBox::information(this,MBOX_OK,"About Lazaro","Lazaro.\2003 Universidad Nacional de Colombia (OHWAHA-Grupo de computación grafica y procesamiento de imagenes)");
  return 1;
}

/** Dibuja con phong
*/
long GLViewWindow::drawPhong(FXObject* obj, FXSelector sel,void* ptr){
	int i=0;	
	FXGLGroup* glg=(FXGLGroup*)viewer->getScene();
	if(glg){
		for(i=0;i<glg->no();i++){
			FXGLObject* fxgo=glg->child(i);
			fxgo->handle(this,MKUINT(FXGLShape::ID_SHADESMOOTH,SEL_COMMAND),0);			
		}
		viewer->update();
	}
return 1;	
}

/** Dibuja con goraud
*/
long GLViewWindow::drawGoraud(FXObject* obj, FXSelector sel,void* ptr){
	int i=0;	
	FXGLGroup* glg=(FXGLGroup*)viewer->getScene();
	if(glg){
		for(i=0;i<glg->no();i++){
			FXGLObject* fxgo=glg->child(i);
			fxgo->handle(this,MKUINT(FXGLShape::ID_SHADEON,SEL_COMMAND),0);			
		}
		viewer->update();
	}
return 1;	
}

/** Desabilita el sombreado
*/
long GLViewWindow::drawShadeOff(FXObject* obj, FXSelector sel,void* ptr){
	int i=0;	
	FXGLGroup* glg=(FXGLGroup*)viewer->getScene();
	if(glg){
		for(i=0;i<glg->no();i++){
			FXGLObject* fxgo=glg->child(i);			
			fxgo->handle(this,MKUINT(FXGLShape::ID_SHADEOFF,SEL_COMMAND),0);
		}
		viewer->update();
	}
	return 1;	
}

/** Realiza zoom in
*/
long GLViewWindow::zoomIn(FXObject* obj, FXSelector sel,void* ptr){
	if(viewer)
		viewer->setZoom(viewer->getZoom()+5*viewer->worldPix() );
	return 1;
}

/** Realiza zoom out
*/
long GLViewWindow::zoomOut(FXObject* obj, FXSelector sel,void* ptr){
	if(viewer)
		viewer->setZoom(viewer->getZoom()-5*viewer->worldPix() );
	return 1;
}

/** Dibuja con wireframe
*/
long GLViewWindow::drawWireFrame(FXObject* obj, FXSelector sel,void* ptr){
	int i=0;	
	FXGLGroup* glg=(FXGLGroup*)viewer->getScene();
	if(glg){
		for(i=0;i<glg->no();i++){
			FXGLTriangleM3D* fxgo=(FXGLTriangleM3D*)glg->child(i);								
			if(fxgo->getTypeMesh()!=CglDocument3d::ID_MESH && fxgo->getTypeMesh()!=CglDocument3d::ID_POINTS){
				fxgo->handle(this,MKUINT(FXGLShape::ID_SHADEOFF,SEL_COMMAND),0);			
				fxgo->handle(this,MKUINT(FXGLShape::ID_STYLE_SURFACE,SEL_COMMAND),0);			
				fxgo->handle(this,MKUINT(FXGLShape::ID_SHADESMOOTH,SEL_COMMAND),0);
				fxgo->handle(this,MKUINT(FXGLShape::ID_STYLE_WIREFRAME,SEL_COMMAND),0);
			}
		}
		viewer->update();
	}
	return 1;	
}

/** Crea el documento
*/
void GLViewWindow::createDocument(){
	m_document3d = new CglDocument3d();
	m_document3d->addView(this);
	setDocument(m_document3d);
}

/** Destruye el documento
*/
void GLViewWindow::destroyDocument(){
	delete m_document3d;
	m_document3d=0;
}

/** Limpia la vista
*/
long GLViewWindow::onClearView(FXObject* sender,FXSelector,void* ptr){
	m_scene->clear();
	return 1;
}

/** Actualiza la vista
*/
void GLViewWindow::updateView(){	
	CglDocument3d& cgld=(CglDocument3d&)getDocument();
	CglDocument3d::TMeshMdd* cglmesh=cgld.getMeshResult();				
	FXGLTriangleM3D* mesh=new FXGLTriangleM3D(0,0,0,cglmesh->getVectorPoints(),cglmesh->getVertices(),cglmesh->getSizeMesh());
	mesh->setTypeMesh(cglmesh->getTypeMesh());
	if(cglmesh->getTypeMesh()==CglDocument3d::ID_POINTS || cglmesh->getTypeMesh()==CglDocument3d::ID_SLICES){
		m_scene->clear();
		m_scenePoints->clear();
		m_scenePoints->append(mesh);		
		viewerPoints->handle(this,MKUINT(FXGLViewer::ID_PARALLEL,SEL_COMMAND),0);
		viewerPoints->handle(this,MKUINT(FXGLViewer::ID_FITVIEW,SEL_COMMAND),0);
		mesh->handle(this,MKUINT(FXGLShape::ID_SHADEOFF,SEL_COMMAND),0);
		mesh->handle(this,MKUINT(FXGLShape::ID_STYLE_SURFACE,SEL_COMMAND),0);
		mesh->handle(this,MKUINT(FXGLShape::ID_SHADESMOOTH,SEL_COMMAND),0);
		mesh->handle(this,MKUINT(FXGLShape::ID_STYLE_WIREFRAME,SEL_COMMAND),0);
	}else{
		m_scene->append(mesh);	
	}
	
	if(cglmesh->getTypeMesh()==CglDocument3d::ID_MESH || cglmesh->getTypeMesh()==CglDocument3d::ID_NORMALS){
		mesh->handle(this,MKUINT(FXGLShape::ID_SHADEOFF,SEL_COMMAND),0);
		mesh->handle(this,MKUINT(FXGLShape::ID_STYLE_SURFACE,SEL_COMMAND),0);
		mesh->handle(this,MKUINT(FXGLShape::ID_SHADESMOOTH,SEL_COMMAND),0);
		mesh->handle(this,MKUINT(FXGLShape::ID_STYLE_WIREFRAME,SEL_COMMAND),0);
	}else{
		
	}
	viewer->handle(this,MKUINT(FXGLViewer::ID_PARALLEL,SEL_COMMAND),0);
  viewer->handle(this,MKUINT(FXGLViewer::ID_FITVIEW,SEL_COMMAND),0);
}

/** Actualiza el alpha dialog
*/
long GLViewWindow::onUpdAlphaShapeDiag(FXObject* sender,FXSelector,void* ptr){
	CglDocument3d& cgld=(CglDocument3d&)getDocument();
	FXdouble alphaVal=cgld.getAlpha();
  sender->handle(this,MKUINT(FXWindow::ID_SETREALVALUE,SEL_COMMAND),(void*)&alphaVal);
  viewer->update();
  return 1;
}

/** Muestra el tiempo que paso en la ejecución de una tecnica
*/
void GLViewWindow::viewTimeElapsed(CglDocument3d& cgld){
	char valElap[50];
	sprintf(valElap,"%d s y %d s",cgld.getTimeElapsed1(),cgld.getTimeElapsed2());
	FXMessageBox::information(this,MBOX_OK,"Tiempo ",valElap);	
}

/** Calculo del alpha
*/
long GLViewWindow::onAlpha(FXObject* sender, FXSelector sel,void* ptr){
	FXdouble alphaVal=0; 
  sender->handle(this,MKUINT(FXWindow::ID_GETREALVALUE,SEL_COMMAND),(void*)&alphaVal);
	CglDocument3d& cgld = (CglDocument3d&)getDocument();	
	getApp()->beginWaitCursor();
	cgld.alpha(alphaVal);
	getApp()->endWaitCursor();
	viewTimeElapsed(cgld);
	return 1;
}

/** Calculo de kohonen
*/
long GLViewWindow::onKohonen(FXObject* sender, FXSelector sel,void* ptr){
	FXdouble kohonenVal=0; 
  sender->handle(this,MKUINT(FXWindow::ID_GETREALVALUE,SEL_COMMAND),(void*)&kohonenVal);
	CglDocument3d& cgld = (CglDocument3d&)getDocument();	
	getApp()->beginWaitCursor();
	cgld.kohonen(kohonenVal);
	getApp()->endWaitCursor();
	viewTimeElapsed(cgld);
	return 1;
}

/** Calculo de slices
*/
long GLViewWindow::onSlices(FXObject* obj, FXSelector sel,void* ptr){
	CglDocument3d& cgld=(CglDocument3d&)getDocument();
	getApp()->beginWaitCursor();
	cgld.slices();
	getApp()->endWaitCursor();
	viewTimeElapsed(cgld);		
	return 1;
}

/** Calculo de Delaunay
*/
long GLViewWindow::onDelaunay(FXObject* obj, FXSelector sel,void* ptr){
	CglDocument3d& cgld=(CglDocument3d&)getDocument();	
	getApp()->beginWaitCursor();
	cgld.delaunay();
	getApp()->endWaitCursor();
	viewTimeElapsed(cgld);		
	return 1;
}

/** Calculo de la envolvente convexa
*/
long GLViewWindow::onConvexHull(FXObject* obj, FXSelector sel,void* ptr){
	CglDocument3d& cgld=(CglDocument3d&)getDocument();	
	getApp()->beginWaitCursor();
	cgld.convexhull();
	getApp()->endWaitCursor();
	viewTimeElapsed(cgld);		
	return 1;
}

/** Calculo de hoppe
*/
long GLViewWindow::onHoppe(FXObject* sender, FXSelector sel,void* ptr){	
	FXdouble hoppeVal=0; 
  sender->handle(this,MKUINT(FXWindow::ID_GETREALVALUE,SEL_COMMAND),(void*)&hoppeVal);
	CglDocument3d& cgld = (CglDocument3d&)getDocument();	
	getApp()->beginWaitCursor();
	cgld.hoppe(hoppeVal);
	getApp()->endWaitCursor();
	viewTimeElapsed(cgld);		
	return 1;
}

/** Apertura de la malla
*/
long GLViewWindow::onOpenMesh(FXObject* obj, FXSelector sel,void* ptr){
	const FXchar *patterns[]={		
    "m3d Files","*.m3d","3ds Files","*.3ds"};
  FXFileDialog open(this,"Abre archivos de puntos");
  open.setPatternList(patterns);
  if(open.execute()){
		CglDocument3d& cgld=(CglDocument3d&)getDocument();
		if(open.getPattern()=="*.m3d"){
			cgld.openMesh3d(open.getFilename().text());
		}else if(open.getPattern()=="*.3ds"){
			cgld.openMesh3ds(open.getFilename().text());
		}
		viewer->update();
	}
	return 1;
}

/** Exporta la malla actual
*/
long GLViewWindow::onExportMesh(FXObject* obj, FXSelector sel,void* ptr){
	const FXchar *patterns[]={		
    "m3d Files","*.m3d","Aliaswavefront Files","*.obj","VRML","*.wrl"};
  FXFileDialog save(this,"Exporta archivos de puntos m3d");
  save.setPatternList(patterns);
  if(save.execute()){
		if(save.getPattern()=="*.m3d"){
			CglDocument3d& cgld=(CglDocument3d&)getDocument();
			cgld.exportMesh3d(save.getFilename().text());		
			cgld.exportMesh3dObj(save.getFilename().text());
			viewer->update();
		}else if(save.getPattern()=="*.obj"){
			CglDocument3d& cgld=(CglDocument3d&)getDocument();
			cgld.exportMesh3dObj(save.getFilename().text());		
			viewer->update();
		}else if(save.getPattern()=="*.wrl"){
			CglDocument3d& cgld=(CglDocument3d&)getDocument();
			cgld.exportMesh3dVrml(save.getFilename().text());		
			viewer->update();
		}
	}
	return 1;
}

/** Calculo a slices
*/
long GLViewWindow::onOpenSlices(FXObject* obj, FXSelector sel,void* ptr){
	const FXchar *patterns[]={		
    "m3d Slices Files",   "*.msd"};
  FXFileDialog open(this,"Abre archivos de slices");
  open.setPatternList(patterns);
  if(open.execute()){
		CglDocument3d& cgld=(CglDocument3d&)getDocument();
		cgld.openSlices(open.getFilename().text());		
		viewer->update();
	}
	return 1;
}

/** Permite seleccionar la figura
*/
void GLViewWindow::onSelectedFigure(const unsigned int& typeMesh){
	int i=0;
	FXGLGroup* glg=(FXGLGroup*)viewer->getScene();	
	if(glg){
		for(i=0;i<glg->no();i++){			
			FXGLTriangleM3D* fm3d= (FXGLTriangleM3D*)glg->child(i);			
			if(fm3d->getTypeMesh()==typeMesh){
				fm3d->setSelected(true);
			}else{
				fm3d->setSelected(false);
			}
		}
		viewer->update();
	}
}

/** Permite seleccionar los puntos
*/
long GLViewWindow::onSelectPoints(FXObject* obj, FXSelector sel,void* ptr){
	onSelectedFigure(CglDocument3d::ID_POINTS);	
	return 1;
}

/** Permite seleccionar las mallas
*/
long GLViewWindow::onSelectMesh(FXObject* obj, FXSelector sel,void* ptr){
	onSelectedFigure(CglDocument3d::ID_MESH);	
	onSelectedFigure(CglDocument3d::ID_KOHONEN);	
	onSelectedFigure(CglDocument3d::ID_ALPHASHAPE);	
	return 1;
}

/** Permite seleccionar delaunay
*/
long GLViewWindow::onSelectDelaunay(FXObject* obj, FXSelector sel,void* ptr){	
	onSelectedFigure(CglDocument3d::ID_DELAUNAY);	
	return 1;
}

/** Permite seleccionar ConvexHull
*/
long GLViewWindow::onSelectConvexHull(FXObject* obj, FXSelector sel,void* ptr){
	onSelectedFigure(CglDocument3d::ID_CONVEXHULL);	
	return 1;
}

/** Permite seleccionar Octrees
*/
long GLViewWindow::onSelectOctrees(FXObject* obj, FXSelector sel,void* ptr){
	onSelectedFigure(CglDocument3d::ID_OCTREES);	
	return 1;
}

/** Muestra el panel actual
*/
void GLViewWindow::viewPacker(Packer** packers,const unsigned int& indexActualTecnique,const unsigned int& sizetec,const unsigned int casePacker){
	unsigned int i=0;		
	if(packers==0){
		return;
	}
	for(i=0;i<sizetec;i++){
		if(packers[i]!=0){
			if(i==indexActualTecnique){
				packers[i]->setWidth(WIDTH_RECONSTRUCTION);
				packers[i]->setHeight(HEIGHT_RECONSTRUCTION);								
			}else{
				packers[i]->setWidth(0);
				packers[i]->setWidth(0);
			}
		}
		packers[i]->repaint();		
	}
	if(m_labelTecnique!=0){
		if(casePacker==0)
			m_labelTecnique->setText(m_tecniquesRecons[indexActualTecnique]);		
		else
			m_labelTecnique->setText(m_tecniquesGeom[indexActualTecnique]);
	}
}

/** Muestra el anterior panel
*/
long GLViewWindow::onAntTecniqueReconst(FXObject* obj, FXSelector sel,void* ptr){	
	unsigned int i=0;
	if(m_indexActualTecnique>0){
		m_indexActualTecnique--;
		m_labelTecnique=m_labelTecniqueRecons;
		viewPacker(m_packerTecniques,m_indexActualTecnique,SIZERECONSTRUCTIONTECNIQUES,0);
	}
	return 1;
}

/** Muestra el siguiente panel
*/
long GLViewWindow::onSigTecniqueReconst(FXObject* obj, FXSelector sel,void* ptr){
	if(m_indexActualTecnique<(SIZERECONSTRUCTIONTECNIQUES-1)){
		m_indexActualTecnique++;
		m_labelTecnique=m_labelTecniqueRecons;
		viewPacker(m_packerTecniques,m_indexActualTecnique,SIZERECONSTRUCTIONTECNIQUES,0);
	}	
	return 1;
}

/** Muestra las tecnicas de geometria computacional y volumetria
*/
long GLViewWindow::onAntTecniqueCGVol(FXObject* obj, FXSelector sel,void* ptr){	
	unsigned int i=0;
	if(m_indexActualTecniqueCGVol>0){
		m_indexActualTecniqueCGVol--;
		m_labelTecnique=m_labelTecniqueGeom;
		viewPacker(m_packerTecniquesCGVol,m_indexActualTecniqueCGVol,SIZERECONSTRUCTIONTECNIQUESCGVOL,1);
	}
	return 1;
}

/** Muestra las tecnicas de geometria computacional y volumetria
*/
long GLViewWindow::onSigTecniqueCGVol(FXObject* obj, FXSelector sel,void* ptr){
	if(m_indexActualTecniqueCGVol<(SIZERECONSTRUCTIONTECNIQUESCGVOL-1)){
		m_indexActualTecniqueCGVol++;
		m_labelTecnique=m_labelTecniqueGeom;
		viewPacker(m_packerTecniquesCGVol,m_indexActualTecniqueCGVol,SIZERECONSTRUCTIONTECNIQUESCGVOL,1);
	}	
	return 1;
}

/** Menu del area de pintado
*/
long GLViewWindow::onQueryMenu(FXObject* sender,FXSelector,void* ptr){
  FXEvent *event=(FXEvent*)ptr;
  FXMenuPane pane(this);
	pane.setBackColor(m_menuColor);
  FXMenuCommand* fxmc=new FXMenuCommand(&pane,"Paralela\t\tParalela\nVista Paralela",NULL,viewer,FXGLViewer::ID_PARALLEL);
	fxmc->setBackColor(m_menuColor);
  fxmc=new FXMenuCommand(&pane,"Perspectiva\t\tPerspectiva\nVista Perspectiva",NULL,viewer,FXGLViewer::ID_PERSPECTIVE);
	fxmc->setBackColor(m_menuColor);
  new FXMenuSeparator(&pane);	
  fxmc=new FXMenuCommand(&pane,"Ajuste\t\tAjuste\nVista Ajustada",NULL,viewer,FXGLViewer::ID_FITVIEW);
	fxmc->setBackColor(m_menuColor);
  fxmc=new FXMenuCommand(&pane,"Reinicializar\t\tReinicializar\nReinicializar parametros",NULL,viewer,FXGLViewer::ID_RESETVIEW);  
	fxmc->setBackColor(m_menuColor);
  pane.create();
  pane.popup(NULL,event->root_x,event->root_y);
  getApp()->runModalWhileShown(&pane);
  return 1;
}

long GLViewWindow::onComputeNormals(FXObject* sender,FXSelector,void* ptr){
	FXdouble densityVal=0; 
  sender->handle(this,MKUINT(FXWindow::ID_GETREALVALUE,SEL_COMMAND),(void*)&densityVal);
	CglDocument3d& cgld=(CglDocument3d&)getDocument();	
	getApp()->beginWaitCursor();
	cgld.computeNormals(densityVal);
	getApp()->endWaitCursor();
	viewTimeElapsed(cgld);		
	return 1;
}


/** Aqui inicia
*/
int main(int argc,char *argv[]) {
  FXApp application("GGLViewer","OHWAHA");
  application.init(argc,argv);
  new GLViewWindow(&application);
  application.create();
  return application.run();
}