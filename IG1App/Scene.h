//#pragma once
#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Camera.h"
#include "Entity.h"
#include "Texture.h"
#include "Light.h"

#include <vector>

//-------------------------------------------------------------------------

class Scene	
{ 
public:
	Scene(int id) { mId = id; };
	~Scene() { free(); resetGL(); lucesEncendidas = false;

	if (dirLight != nullptr) {
		delete dirLight; dirLight = nullptr;
	}
	if (spotLight != nullptr) {
		delete spotLight; spotLight = nullptr;
	}
	if (posLight != nullptr) {
		delete posLight; posLight = nullptr;
	}
	};

	Scene(const Scene& s) = delete;  // no copy constructor
	Scene& operator=(const Scene& s) = delete;  // no copy assignment
		
	void init();

    void render(Camera const& cam) const;
	void update();
	int getId() const { return mId; };

	void changeScene(int id);
	static bool lucesEncendidas;
	DirLight* dirLight = nullptr;
	PosLight* posLight = nullptr;
	SpotLight* spotLight = nullptr;


	CompoundEntity* tieGroup;
	GLdouble tieLocalAngle;

	void TIEsLightsOff();
	void TIEsLightsOn();

	void defaultLighting();
	void darkScene();
	void orbita();
	void rota();

protected:
	void free();
	void setGL(int id);
	void resetGL();

	std::vector<Abs_Entity*> gObjects;  // Entities (graphic objects) of the scene
	std::vector<Abs_Entity*> gObjectsTranslucid;  // Entities (translucid objects) of the scene
	std::vector<Texture*> gTextures; //textures (imagenes a pinyon) of the scene
	int mId;
	void scene0();
	void scene1();
	void scene2();
	void scene3();
	void scene4();
	void scene5();
	void scene6();
	//PRACTICA 2
	void sceneDirLight(Camera const& cam) const;
	void createLights();
	void desactivarLuces();
};
//-------------------------------------------------------------------------

#endif //_H_Scene_H_

