//#pragma once
#ifndef _H_Scene_H_
#define _H_Scene_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Camera.h"
#include "Entity.h"
#include "Texture.h"

#include <vector>

//-------------------------------------------------------------------------

class Scene	
{ 
public:
	Scene(int id) { mId = id; };
	~Scene() { free(); resetGL(); };

	Scene(const Scene& s) = delete;  // no copy constructor
	Scene& operator=(const Scene& s) = delete;  // no copy assignment
		
	void init();

    void render(Camera const& cam) const;
	void update();

	void changeScene(int id);
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
};
//-------------------------------------------------------------------------

#endif //_H_Scene_H_

