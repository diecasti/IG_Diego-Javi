#include "Scene.h"
#include "CheckML.h"
#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>

using namespace glm;
//-------------------------------------------------------------------------

void Scene::init()
{
	setGL(mId);

	if (mId == 1)
		scene1();
	else if (mId == 0)
		scene0();
	else if(mId == 2)
		scene2();

}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void Scene::scene0() {
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

void Scene::scene1() {
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
	f->setModelMat(glm::translate(dmat4(1), dvec3(100, 200, 100)));	//! AQUI SOLO AFECTA LA Y WTF

	gObjects.push_back(f);


	//hierba
	Texture* hierbaText = new Texture();
	hierbaText->load("..\\Bmps\\grass.bmp", { 0,0,0 }, 255);
	gTextures.push_back(hierbaText);
	Hierba* hierba = new Hierba(100, 100);
	hierba->setTexture(hierbaText);
	hierba->setModelMat(glm::translate(dmat4(1), dvec3(-200, 60, 200)));

	gObjects.push_back(hierba);


	//FOTO
	Foto* u = new Foto(100.0, 150.0);
	u->setModelMat(glm::translate(dmat4(1), dvec3(0, 10, 0)));
	u->setModelMat(glm::rotate(u->modelMat(), radians(90.0), dvec3(1, 0, 0)));
	gObjects.push_back(u);

	//CAJA TRASLUCIDA
	Texture* i = new Texture();
	i->load("..\\Bmps\\windowV.bmp", 100);
	gTextures.push_back(i);

	ContCuboTexCo* j = new ContCuboTexCo(500);
	j->setTexture(i, i);
	f->setModelMat(glm::translate(dmat4(1), dvec3(0, 250, 0)));
	gObjectsTranslucid.push_back(j);
}
//-------------------------------------------------------------------------
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
	if (id == 0)
		glClearColor(0.0, 0.0, 0.0, 1.0);  // background color (alpha=1 -> opaque)
	else if (id == 1)
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
	glEnable(GL_COLOR_MATERIAL);
	sceneDirLight(cam);
	cam.upload();

	for (Abs_Entity* el : gObjects)
	{
		el->render(cam.viewMat());
	}
	for (Abs_Entity* el : gObjectsTranslucid)
	{
		el->render(cam.viewMat());
	}
	glDisable(GL_COLOR_MATERIAL);
}
//-------------------------------------------------------------------------
//Llamamos uno por uno a los updates de los elementos que componen la escena
void Scene::update() {
	for (Abs_Entity* el : gObjects)
	{
		el->update();
	}for (Abs_Entity* el : gObjectsTranslucid)
	{
		el->update();
	}
}
//-------------------------------------------------------------------------
//!Métodos practica 2
void Scene::sceneDirLight(Camera const& cam) const {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glm::fvec4 posDir = { 1, 1, 1, 0 };
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(cam.viewMat()));
	glLightfv(GL_LIGHT0, GL_POSITION, value_ptr(posDir));
	glm::fvec4 ambient = { 0, 0, 0, 1 };
	glm::fvec4 diffuse = { 1, 1, 1, 1 };
	glm::fvec4 specular = { 0.5, 0.5, 0.5, 1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, value_ptr(ambient));
	glLightfv(GL_LIGHT0, GL_DIFFUSE, value_ptr(diffuse));
	glLightfv(GL_LIGHT0, GL_SPECULAR, value_ptr(specular));
}

void Scene::scene2() {
	Texture* noche = new Texture();
	noche->load("..\\Bmps\\noche.bmp");
	gTextures.push_back(noche);


	//# Para ver comentarios de estos enunciados en toda la solucion
	gObjects.push_back(new Sphere(100, 50, 50));

	gObjects.push_back(new Cylinder(80, 80, 50, 50, 50));

	gObjects.back()->setModelMat(glm::translate(gObjects.back()->modelMat(), dvec3(60, 0, 0)));
	gObjects.back()->setModelMat(glm::rotate(gObjects.back()->modelMat(), radians(90.0), dvec3(0, 1, 0)));

	gObjects.push_back(new Disk(0, 80, 50, 50));
	gObjects.back()->setModelMat(glm::translate(gObjects.back()->modelMat(), dvec3(110, 0, 0)));
	gObjects.back()->setModelMat(glm::rotate(gObjects.back()->modelMat(), radians(90.0), dvec3(0, 1, 0)));

	gObjects.push_back(new Cylinder(20, 20, 400, 50, 50));
	gObjects.back()->setModelMat(glm::translate(gObjects.back()->modelMat(), dvec3(0, 0, -200)));

	//ALAS
	gObjects.push_back(new DiskText(0, 300, 6, 6));
	gObjects.back()->setModelMat(glm::translate(gObjects.back()->modelMat(), dvec3(0, 0, -200)));
	gObjects.back()->setModelMat(glm::rotate(gObjects.back()->modelMat(), radians(30.0), dvec3(0, 0, 1)));
	gObjects.back()->setTexture(noche);

	gObjects.push_back(new Disk(0, 300, 6, 6));
	gObjects.back()->setModelMat(glm::translate(gObjects.back()->modelMat(), dvec3(0, 0, 200)));
	gObjects.back()->setModelMat(glm::rotate(gObjects.back()->modelMat(), radians(30.0), dvec3(0, 0, 1)));
}
