#ifndef SCENE3DS_H
#define SCENE3DS_H

// inclusiones especificas
#include <stdlib.h>
#include <stdio.h>
#include <string>

#include "../utilformat/readdatabinary.h"

#include "base.h"

CGL_BEGIN

/** Esta clase carga una scena 3ds (3d studio max)
*/
class Scene3ds
{
	public:
		/** tipos proveidos por sencillez
		*/
		typedef unsigned char	byte;			// 1
		typedef unsigned short	word;			// 2
		typedef unsigned long	dword;		// 4
		typedef float			float32;	// 4

		/** Face.
		*/		
		typedef struct
		{
			word p0,p1,p2; // Indice de los puntos
			word flags;		 // Bandera
		} H3dsFace;

		/** Vertice
		*/
		typedef struct
		{
	    union { float32 x; dword ix; }; //Coordenada X
  	  union { float32 y; dword iy; }; //Coordenada Y
    	union { float32 z; dword iz; }; //Coordenada Z
		} H3dsVert;

		/** Map
		*/
		typedef struct
		{
	    union { float32 u; dword iu; }; //Coordenada U
  	  union { float32 v; dword iv; }; //Coordenada V
		} H3dsMap;

		/** 3d Mesh object
		*/
		typedef struct
		{	
			int faces;              // Numero de faces en el facelist
			int verts;              // Numero de vertices en el vert list	
			int maps;               // Numero de coordenadas mapeadas en el maplist	
			int matrix;             // 1 si una matriz de transformación es cargada
			H3dsFace*	facelist;			// Lista de faces
			H3dsVert*	vertlist;			// Lista de vertices
			H3dsMap*	maplist;			// Lista de maplist
			float32 TraMatrix[3*4]; // 3*3 matriz de rotación, 3*1 matriz de translación
			char name[16];          // Nombre del objeto, zero terminated	
			H3dsVert centre;				// Centro del objeto
		} H3dsMeshObj;

		/** Scene
		*/
		typedef struct
		{
			int meshobjs;								// Número de objetos en la lista de objetos
			H3dsMeshObj * meshobjlist;	// Lista de objetos
			H3dsVert centre;						// Centro de la escena
		} H3dsScene;

		enum {
			CHUNK_RGBF      	= 0x0010,
			CHUNK_RGBB      	= 0x0011,
			CHUNK_MAIN      	= 0x4D4D,
  		CHUNK_OBJMESH		= 0x3D3D,
	  	CHUNK_BKGCOLOR		= 0x1200,
			CHUNK_AMBCOLOR  	= 0x2100,
			CHUNK_OBJBLOCK  	= 0x4000,
			CHUNK_TRIMESH   	= 0x4100,
			CHUNK_VERTLIST		= 0x4110,
			CHUNK_FACELIST		= 0x4120,
			CHUNK_FACEMAT		= 0x4130,
			CHUNK_MAPLIST		= 0x4140,
			CHUNK_SMOOLIST		= 0x4150,
			CHUNK_TRMATRIX		= 0x4160,
			CHUNK_LIGHT     	= 0x4600,
			CHUNK_SPOTLIGHT		= 0x4610,
			CHUNK_CAMERA    	= 0x4700,
			CHUNK_MATERIAL  	= 0xAFFF,
			CHUNK_MATNAME   	= 0xA000,
			CHUNK_AMBIENT   	= 0xA010,
			CHUNK_DIFFUSE   	= 0xA020,
			CHUNK_SPECULAR  	= 0xA030,
			CHUNK_TEXTURE   	= 0xA200,
			CHUNK_BUMPMAP   	= 0xA230,
			CHUNK_MAPFILE   	= 0xA300,
			CHUNK_KEYFRAMER 	= 0xB000,
			CHUNK_FRAMES		= 0xB008
		};

	public:

		/** Constructor por omision
		*/
		Scene3ds();

		/** Construye un cargador a partir de los datos binarios
		*/
		Scene3ds(ReadDataBinary* databinary);

		/** Destructor
		*/
		~Scene3ds();
		
		/** Carga el modelo
		*/
		void loadModel();
		
		/** Retorna la scena, retorna 0 si la escena no existe
		*/
		H3dsScene* getScene();
		
		/** Libera la escena
		*/
		void HFree3dsScene();

		/** Fija los datos binarios
		*/
		void setDataBinary(ReadDataBinary* databinary);

	private:
	
		/** Retorna memoria
		*/
 		void* getmem(const int& size);
	
		/**
		*/
		H3dsMeshObj* getMeshObj();

	private:

		/** Lee los datos del flujo binario
		*/
		void dread(void * dest, const int& len);

		/** Fija la posición del seek
		*/
		void dsetpos(const dword& pos);

		/** Retorna la posición del seek
		*/
		dword dgetpos(void);

	private:

		/** Retorna el identificador
		*/
		bool getIdentifier(word& id3ds, dword& len);

		/** Lee el main
		*/
		void readMain(const dword& p);

		/** Lee el meshObj
		*/
		void readMeshObj(const dword& p);

		/** Lee el objBlock
		*/
		void readObjBlock(const dword& p);

		/** Lee el trimesjBlock
		*/
		void readTriMeshBlocks(const dword& p, char * name );

		/** Lee la lista de vertices
		*/
		void readVertList(const dword&, H3dsMeshObj * meshobj);
		
		/** Lee la lista de faces
		*/
		void readFaceList(const dword&, H3dsMeshObj * meshobj);

		/** Lee la lista de maps
		*/
		void readMapList(const dword&, H3dsMeshObj * meshobj);

		/** Lee la matriz de translación
		*/
		void readTraMatrix(const dword&, H3dsMeshObj * meshobj);

	private:

		// Scena
		H3dsScene* m_scene;

	private:

	  	// Apuntador a los datos datos binarios
		ReadDataBinary* m_databinary;

		// modelo activo
		unsigned int m_activeModel;
};



Scene3ds::Scene3ds() : m_scene(0), m_databinary(0)
{

}

Scene3ds::Scene3ds(ReadDataBinary* databinary) : m_scene(0), m_databinary(databinary)
{

}

Scene3ds::~Scene3ds()
{
	HFree3dsScene();
}



void Scene3ds::setDataBinary(ReadDataBinary* databinary)
{
	m_databinary=databinary;
}

Scene3ds::H3dsMeshObj* Scene3ds::getMeshObj()
{
	void* mem;
	mem=realloc(m_scene->meshobjlist, sizeof(H3dsMeshObj)*(m_scene->meshobjs+1));
	m_scene->meshobjlist=(H3dsMeshObj*) mem;
	H3dsMeshObj* obj=&m_scene->meshobjlist[m_scene->meshobjs++];
	memset(obj, 0, sizeof(H3dsMeshObj));
	return obj;
}

void* Scene3ds::getmem(const int& size)
{
	void * mem;
	mem=malloc(size);
	return mem;
}

void Scene3ds::dread(void* dest, const int& len)
{
	m_databinary->read(dest,len);
}

void Scene3ds::dsetpos(const dword& pos)
{
	m_databinary->setpos(pos);
}

Scene3ds::dword Scene3ds::dgetpos(void)
{
	return m_databinary->getpos();
}

void Scene3ds::loadModel()
{
	if(m_databinary && m_databinary->ready())
	{
		word id3ds;
		dword len;
		HFree3dsScene();		
		if(!getIdentifier(id3ds,len))
		{			
			return;
		}		
		m_scene = new H3dsScene;
		memset(m_scene, 0, sizeof(H3dsScene));		
		readMain(len);
	}else{
		cout<<"Los datos binarios no estan listos\n";
	}
}

bool Scene3ds::getIdentifier(word& id3ds, dword& len)
{
	dread(&id3ds,sizeof(id3ds));
	dread(&len,sizeof(len));
	if(id3ds!=CHUNK_MAIN)
	{
		return false;
	}
	return true;		
};


void Scene3ds::readMain(const dword& p)
{
	word id;
	dword len,pc;
	while((pc=dgetpos()) < p)
	{
		dread(&id,sizeof(id));
		dread(&len,sizeof(len));
		switch((int) id)
		{
    	case CHUNK_OBJMESH: readMeshObj(len); break;
			// case CHUNK_MATERIAL: break;
			// case CHUNK_KEYFRAMER: break;
			default: dsetpos(pc+len);
		}
	}
}

void Scene3ds::readMeshObj(const dword& p)
{
	word id;
	dword len, pc;	
	while((pc=dgetpos()) < p)
	{
		dread(&id, sizeof(id));
		dread(&len, sizeof(len));
		switch((int)id)
		{
			case CHUNK_OBJBLOCK: readObjBlock(pc+len); break;
			default: dsetpos(pc+len);
		}
	}
}

void Scene3ds::readObjBlock(const dword& p)
{
	word id;
	dword len, pc;
	char name[16];

	// El nombre del objeto es el primer item
	int n=0;
	do{
		dread(&name[n++], 1);
	}while(name[n-1]!='\0' && n<sizeof(name));
	name[n-1]='\0';

	while((pc=dgetpos()) < p)
	{
		dread(&id, sizeof(id));
		dread(&len, sizeof(len));
		switch((int)id)
		{
			case CHUNK_TRIMESH:	readTriMeshBlocks(pc+len, name); break;
			default: dsetpos(pc+len);
		}
	}
}

void Scene3ds::readTriMeshBlocks(const dword& p, char * name )
{
	word id;
	dword len, pc;
	H3dsMeshObj* meshobj=getMeshObj();
	strcpy(meshobj->name, name);
	while((pc=dgetpos()) < p)
	{
		dread(&id, sizeof(id));
		dread(&len, sizeof(len));
		switch((int)id)
		{
			case CHUNK_VERTLIST: readVertList (pc+len, meshobj); break;
			case CHUNK_FACELIST: readFaceList (pc+len, meshobj); break;
			case CHUNK_MAPLIST:  readMapList  (pc+len, meshobj); break;
			case CHUNK_TRMATRIX: readTraMatrix(pc+len, meshobj); break;
			default: dsetpos(pc+len);
		}
	}

}

void Scene3ds::readVertList(const dword&, H3dsMeshObj * meshobj)
{
	word nv;
	// lee los datos y los deja en memoria
	dread(&nv, sizeof(nv));
	// fija el número de verices
	meshobj->verts=nv;	
	int k=nv;
	// Reserva memoria
	meshobj->vertlist=(H3dsVert *) getmem(sizeof(H3dsVert)*k);
	// Lee de memoria los datos y los almacena en el objeto
	for(int n=0; n<k; n++) {
		dread(&meshobj->vertlist[n], sizeof(float32)*3);
	}
}

void Scene3ds::readFaceList(const dword&, H3dsMeshObj * meshobj)
{
	word nv;
	dread(&nv, sizeof(nv));
	meshobj->faces=nv;
	int k=nv;
	meshobj->facelist=(H3dsFace *) getmem(sizeof(H3dsFace)*k);
	for(int n=0; n<k; n++) {
		dread(&meshobj->facelist[n], sizeof(word)*4);
	}
}

void Scene3ds::readMapList(const dword&, H3dsMeshObj * meshobj)
{
	word nv;
	dread(&nv, sizeof(nv));
	meshobj->maps=nv;
	int k=nv;
	meshobj->maplist=(H3dsMap *) getmem(sizeof(H3dsMap)*k);
	for(int n=0; n<k; n++) {
		dread(&meshobj->maplist[n], sizeof(float32)*2);
	}
}

void Scene3ds::readTraMatrix(const dword&, H3dsMeshObj * meshobj)
{
	dread(&meshobj->TraMatrix, sizeof(float32)*12);
	meshobj->matrix=1;
}

void Scene3ds::HFree3dsScene()
{	
	if(m_scene!=0){
		if(m_scene->meshobjlist) {
			for(int n=m_scene->meshobjs-1; n>=0; n--) {
				H3dsMeshObj * mobj = &m_scene->meshobjlist[n];
				if(mobj->maplist)  free(mobj->maplist);
				if(mobj->facelist) free(mobj->facelist);
				if(mobj->vertlist) free(mobj->vertlist);
			}
			free(m_scene->meshobjlist);
		}
		delete m_scene;
		m_scene=0;
	}
}

Scene3ds::H3dsScene* Scene3ds::getScene(){
	return m_scene;
}

CGL_END

#endif