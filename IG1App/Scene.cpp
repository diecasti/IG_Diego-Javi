#include "Scene.h"
#include "CheckML.h"
#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>

using namespace glm;
//-------------------------------------------------------------------------

void Scene::init()
{
	setGL(mId);
	// allocate memory and load resources
	// Lights
	// Textures
	//Texture::load();
	
	/*
	texture algo = new texture();
	algo.load(direccion);
	  */
	//#Scene selection
	if (mId == 1)
		scene1();
	else if (mId == 0)
		scene0();

}
//-------------------------------------------------------------------------
void Scene::scene2() {
	// Graphics objects (entities) of the scene
	gObjects.push_back(new EjesRGB(400.0));
	gObjects.push_back(new CajaConFondo(400.0));
	auto t = new Texture();
	t->load("..\\Bmps\\baldosaC.bmp");
	gTextures.push_back(t);
	gObjects.push_back(new Suelo(400.0, 600.0, 10, 10));
	gObjects.back()->setTexture(t);
	gObjects.back()->setColor(dvec4(1.0, 0.5, 1.0, 1));
	gObjects.back()->setModelMat(glm::rotate(dmat4(1), radians(-90.0), dvec3(1, 0, 0)));

	//gObjects.push_back(new Estrella3D(400.0,6,20.0));
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void Scene::scene0() {
	// Graphics objects (entities) of the scene
	gObjects.push_back(new EjesRGB(400.0));
}
//-------------------------------------------------------------------------
void Scene::scene1() {
	//EJERCICIO 1
	//Creamos triangulo
	Poligono* a = new Poligono(3.0, 200);
	a->setColor({ 255.0 / 255.0,255.0 / 255.0,0.0 / 255.0,1 });
	gObjects.push_back(a);
	//Creamos circulo
	Poligono* b = new Poligono(60.0, 200);
	b->setColor({ 255.0 / 255.0,0.0 / 255.0,255.0 / 255.0,1 });
	gObjects.push_back(b);

	//EJERCICIO 2
	Sierpinski* c = new Sierpinski(200, 10000);
	c->setColor({ 255.0 / 255.0,233.0 / 255.0,0.0 / 255.0,1 });
	gObjects.push_back(c);

	//EJERCICIO 3
	TrianguloRGB* d = new TrianguloRGB(50);
	gObjects.push_back(d);

	//Ejercicio 4
	RectanguloRGB* e = new RectanguloRGB(800, 400);
	gObjects.push_back(e);

	e->setModelMat(translate(e->modelMat(), dvec3(0.0, 0.0, -100.0)));
}

//-------------------------------------------------------------------------
void Scene::changeScene(int id) {
	if (mId != id) {
		mId = id;
		free();	//freeFire();
		resetGL();

		setGL(mId);
		if (mId == 1) {
			scene1();
		}
		else if (mId == 0) {
			scene0();
		}
		else if (mId == 2) {
			scene2();
		}
	}
}
//-------------------------------------------------------------------------
void Scene::free()
{ // release memory and resources   

	for (Abs_Entity* el : gObjects)
	{
		delete el;  el = nullptr;
	}
	gObjects.clear();
	for (Texture* el : gTextures)
	{
		delete el;  el = nullptr;
	}
	gTextures.clear();
}
//-------------------------------------------------------------------------
void Scene::setGL(int id)
{
	// OpenGL basic setting
	if (id == 1)
		glClearColor(0.0, 0.0, 0.0, 1.0);  // background color (alpha=1 -> opaque)
	else if (id == 0)
		glClearColor(1.0, 1.0, 1.0, 1.0);  // background color (alpha=1 -> opaque)

	glEnable(GL_DEPTH_TEST);  // enable Depth test 
	//Textures
	glEnable(GL_TEXTURE_2D);

}
//-------------------------------------------------------------------------
void Scene::resetGL()
{
	glClearColor(.0, .0, .0, .0);  // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);  // disable Depth test 	
	glDisable(GL_TEXTURE_2D); //desactivar texturitas
}
//-------------------------------------------------------------------------

void Scene::render(Camera const& cam) const
{
	cam.upload();

	for (Abs_Entity* el : gObjects)
	{
		el->render(cam.viewMat());
	}
}
//-------------------------------------------------------------------------
//#Llamamos uno por uno a los updates de los elementos que componen la escena
void Scene::update() {
	for (Abs_Entity* el : gObjects)
	{
		el->update();
	}
}
//-------------------------------------------------------------------------
