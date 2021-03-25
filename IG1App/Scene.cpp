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
	//gObjects.push_back(new CajaConFondo(400.0));

	//crear una textura
	auto t = new Texture();
	t->load("..\\Bmps\\baldosaC.bmp");
	gTextures.push_back(t);

	//el objeto
	/*
	gObjects.push_back(new Suelo(400.0, 600.0, 10, 10));

	//el objeto añadir textura
	gObjects.back()->setTexture(t);
	//gObjects.back()->setColor(dvec4(1.0, 0.5, 1.0, 1));
	gObjects.back()->setModelMat(glm::rotate(dmat4(1), radians(-90.0), dvec3(1, 0, 0)));
	*/
	/*
	//crear textura
	auto e = new Texture();
	e->load("..\\Bmps\\baldosaP.bmp");
	gTextures.push_back(e);

	//objeto
	gObjects.push_back(new EstrellaTexCor(400.0, 4, 20.0));

	//objeto con textura
	gObjects.back()->setTexture(e);
	*/


	//crear textura
	auto exterior = new Texture();
	exterior->load("..\\Bmps\\container.bmp");
	gTextures.push_back(exterior);
	//
	auto interior = new Texture();
	interior->load("..\\Bmps\\papelE.bmp");
	gTextures.push_back(interior);

	//la caja
	auto caja = new CajaConFondoTx(50.0, 200);
	gObjects.push_back(caja);
	//darles las texturas
	//tmb & tal vez deberia de haber hecho un cast aqui
	//TODO
	caja->setTexture(exterior, interior);


	gObjects.push_back(new Foto(100.0, 150.0));

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

void Scene::scene3() {
	//SUELO
	Texture* t = new Texture();
	t->load("..\\Bmps\\baldosaC.bmp");
	gTextures.push_back(t);

	Suelo* h = new Suelo(500, 500, 3, 3);
	h->setTexture(t);
	h->setModelMat(glm::rotate(dmat4(1), radians(-90.0), dvec3(1, 0, 0)));
	gObjects.push_back(h);

	//CAJA
	Texture* interior = new Texture();
	interior->load("..\\Bmps\\papelE.bmp");
	gTextures.push_back(interior);

	Texture* exterior = new Texture();
	exterior->load("..\\Bmps\\container.bmp");
	gTextures.push_back(exterior);

	CajaConFondoTx* g = new CajaConFondoTx(100, 50);
	g->setColor({ 255.0 / 255.0,0.0 / 255.0,0.0 / 255.0,1 });
	g->setTexture(exterior, interior);
	g->setModelMat(glm::translate(dmat4(1), dvec3(100, 50, 100)));
	gObjects.push_back(g);

	//ESTRELLA3D
	Texture* w = nullptr;
	w = new Texture();
	w->load("..\\Bmps\\baldosaP.bmp");
	gTextures.push_back(w);

	EstrellaTexCor* f = new EstrellaTexCor(50, 4, 0);
	f->setTexture(w);	//Ej13
	f->setColor({ 255.0 / 255.0,0.0 / 255.0,0.0 / 255.0,1 });
	f->setModelMat(glm::translate(dmat4(1), dvec3(1000, 200, 100)));
	gObjects.push_back(f);

	//CAJA TRASLUCIDA
	Texture* i = new Texture();
	i->load("..\\Bmps\\windowV.bmp", 100);
	gTextures.push_back(i);

	ContCuboTexCo* j = new ContCuboTexCo(500);
	j->setTexture(i, i);
	f->setModelMat(glm::translate(dmat4(1), dvec3(0, 250, 0)));
	gObjects.push_back(j);

	//FOTO
	Foto* u = new Foto(100.0, 150.0);
	u->setModelMat(glm::translate(dmat4(1), dvec3(0, 10, 0)));
	u->setModelMat(glm::rotate(u->modelMat(), radians(90.0), dvec3(1, 0, 0)));
	gObjects.push_back(u);
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
		else if (mId == 3) {
			scene3();
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
