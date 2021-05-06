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
	else if (mId == 2)
		scene2();

	//gObjects.push_back(new EjesRGB(400.0));
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
		if (mId == 0) {
			scene0();
		}
		else if (mId == 1) {
			scene1();
		}

		else if (mId == 2) {
			scene2();
		}
		else if (mId == 3) {
			scene3();
		}
		else if (mId == 4) {
			scene4();
		}
		else if (mId == 5) {
			scene5();
		}
		else if (mId == 6) {
			scene6();
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
	for (Abs_Entity* el : gObjectsTranslucid)
	{
		delete el;  el = nullptr;
	}
	gObjectsTranslucid.clear();
}
//-------------------------------------------------------------------------
void Scene::setGL(int id)
{
	// OpenGL basic setting
	if (id == 0)
		glClearColor(0.0, 0.0, 0.0, 1.0);  // background color (alpha=1 -> opaque)
	else if (id == 1)
		glClearColor(1.0, 1.0, 1.0, 1.0);  // background color (alpha=1 -> opaque)
	else if (id == 2 || id == 3 || id == 4 || id == 5 || id == 6)
		glClearColor(0.7, 0.8, 0.9, 1.0);
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
	//TEXTURAS
	Texture* noche = new Texture();
	noche->load("..\\Bmps\\noche.bmp", 200);
	gTextures.push_back(noche);

	gObjects.push_back(new EjesRGB(400.0));

	auto ce = new CompoundEntity();
	gObjects.push_back(ce);
	//NAVE
	auto esfera = new Sphere(100, 50, 50);
	ce->addEntity(esfera);
	//gObjects.push_back(new Sphere(100, 50, 50));

	//VENTANA
	auto escotilla = new CompoundEntity();
	auto cilindro = new Cylinder(80, 80, 50, 50, 50); 
	cilindro->setModelMat(glm::rotate(cilindro->modelMat(), radians(90.0), dvec3(0, 1, 0)));
	escotilla->addEntity(cilindro);

	
	auto tapa = new Disk(0, 80, 50, 50);
	tapa->setModelMat(glm::translate(tapa->modelMat(), dvec3(50, 0, 0)));
	tapa->setModelMat(glm::rotate(tapa->modelMat(), radians(90.0), dvec3(0, 1, 0)));
	escotilla->addEntity(tapa);

	//mover la escotilla a su posicion
	escotilla->setModelMat(glm::translate(escotilla->modelMat(), dvec3(60, 0, 0)));
	ce->addEntity(escotilla);

	/*
	gObjects.push_back(new Cylinder(80, 80, 50, 50, 50));
	gObjects.back()->setModelMat(glm::translate(gObjects.back()->modelMat(), dvec3(60, 0, 0)));
	gObjects.back()->setModelMat(glm::rotate(gObjects.back()->modelMat(), radians(90.0), dvec3(0, 1, 0)));

	//CRISTALVENTANA
	gObjects.push_back(new Disk(0, 80, 50, 50));
	gObjects.back()->setModelMat(glm::translate(gObjects.back()->modelMat(), dvec3(110, 0, 0)));
	gObjects.back()->setModelMat(glm::rotate(gObjects.back()->modelMat(), radians(90.0), dvec3(0, 1, 0)));
	*/

	//UNION ALAS
	auto cilindro_Centro = new Cylinder(20, 20, 400, 50, 50);
	cilindro_Centro->setModelMat(glm::translate(cilindro_Centro->modelMat(), dvec3(0, 0, -200)));
	ce->addEntity(cilindro_Centro);

	/*
	gObjects.push_back(new Cylinder(20, 20, 400, 50, 50));
	gObjects.back()->setModelMat(glm::translate(gObjects.back()->modelMat(), dvec3(0, 0, -200)));
	*/

	//ALAS
	auto ala_izquierda = new DiskText(0, 300, 6, 6);
	ala_izquierda->setModelMat(glm::translate(ala_izquierda->modelMat(), dvec3(0, 0, -200)));
	ala_izquierda->setModelMat(glm::rotate(ala_izquierda->modelMat(), radians(30.0), dvec3(0, 0, 1)));
	ala_izquierda->setTexture(noche);
	ce->addEntityTranslucid(ala_izquierda);

	/*
	gObjectsTranslucid.push_back(new DiskText(0, 300, 6, 6));
	gObjectsTranslucid.back()->setModelMat(glm::translate(gObjectsTranslucid.back()->modelMat(), dvec3(0, 0, -200)));
	gObjectsTranslucid.back()->setModelMat(glm::rotate(gObjectsTranslucid.back()->modelMat(), radians(30.0), dvec3(0, 0, 1)));
	gObjectsTranslucid.back()->setTexture(noche);
	*/
	auto ala_derecha = new DiskText(0, 300, 6, 6);
	ala_derecha->setModelMat(glm::translate(ala_derecha->modelMat(), dvec3(0, 0, 200)));
	ala_derecha->setModelMat(glm::rotate(ala_derecha->modelMat(), radians(30.0), dvec3(0, 0, 1)));
	ala_derecha->setTexture(noche);
	ce->addEntityTranslucid(ala_derecha);


	ce->setModelMat(glm::translate(ce->modelMat(), dvec3(0, 0, 200)));

	/*
	gObjectsTranslucid.push_back(new DiskText(0, 300, 6, 6));
	gObjectsTranslucid.back()->setModelMat(glm::translate(gObjectsTranslucid.back()->modelMat(), dvec3(0, 0, 200)));
	gObjectsTranslucid.back()->setModelMat(glm::rotate(gObjectsTranslucid.back()->modelMat(), radians(30.0), dvec3(0, 0, 1)));
	gObjectsTranslucid.back()->setTexture(noche);
	*/

	//Escena2
	//gObjects.push_back(new AnilloCuadrado());

	//Escena 3
	//gObjects.push_back(new Cubo(100));

	//escena entidad compuesta
	/*
	gObjects.push_back(ce);
	//el cubo (coordenada 0,0,0)
	ce->addEntity(new Cubo(100));
	//translacion de la entidad compuesta
	gObjects.back()->setModelMat(glm::translate(gObjects.back()->modelMat(), dvec3(0, 0, 200)));
	//segundo cubo
	auto cubo2 = new Cubo(100);
	cubo2->setModelMat(glm::translate(cubo2->modelMat(), dvec3(0, 0, 200)));
	ce->addEntity(cubo2);
	//rotate de la entidad compuesta

	ce->setModelMat(glm::rotate(ce->modelMat(), radians(30.0), dvec3(0, 0, 1)));
	*/
	/*
	*/
}

void Scene::scene3(){
	//TEXTURAS
	Texture* noche = new Texture();
	noche->load("..\\Bmps\\noche.bmp", 200);
	gTextures.push_back(noche);

	//EJES RGB
	gObjects.push_back(new EjesRGB(400.0));

	//NAVE
	gObjects.push_back(new Sphere(100, 50, 50));

	//VENTANA
	gObjects.push_back(new Cylinder(80, 80, 50, 50, 50));
	gObjects.back()->setModelMat(glm::translate(gObjects.back()->modelMat(), dvec3(60, 0, 0)));
	gObjects.back()->setModelMat(glm::rotate(gObjects.back()->modelMat(), radians(90.0), dvec3(0, 1, 0)));

	//CRISTALVENTANA
	gObjects.push_back(new Disk(0, 80, 50, 50));
	gObjects.back()->setModelMat(glm::translate(gObjects.back()->modelMat(), dvec3(110, 0, 0)));
	gObjects.back()->setModelMat(glm::rotate(gObjects.back()->modelMat(), radians(90.0), dvec3(0, 1, 0)));

	//UNION ALAS
	gObjects.push_back(new Cylinder(20, 20, 400, 50, 50));
	gObjects.back()->setModelMat(glm::translate(gObjects.back()->modelMat(), dvec3(0, 0, -200)));

	//ALAS
	gObjectsTranslucid.push_back(new DiskText(0, 300, 6, 6));
	gObjectsTranslucid.back()->setModelMat(glm::translate(gObjectsTranslucid.back()->modelMat(), dvec3(0, 0, -200)));
	gObjectsTranslucid.back()->setModelMat(glm::rotate(gObjectsTranslucid.back()->modelMat(), radians(30.0), dvec3(0, 0, 1)));
	gObjectsTranslucid.back()->setTexture(noche);

	gObjectsTranslucid.push_back(new DiskText(0, 300, 6, 6));
	gObjectsTranslucid.back()->setModelMat(glm::translate(gObjectsTranslucid.back()->modelMat(), dvec3(0, 0, 200)));
	gObjectsTranslucid.back()->setModelMat(glm::rotate(gObjectsTranslucid.back()->modelMat(), radians(30.0), dvec3(0, 0, 1)));
	gObjectsTranslucid.back()->setTexture(noche);

}

void Scene::scene4() {
	//EJES RGB
	gObjects.push_back(new EjesRGB(400.0));

	//ANILLO MULTICOLOR
	gObjects.push_back(new AnilloCuadrado());
}
void Scene::scene5() {
	//EJES RGB
	gObjects.push_back(new EjesRGB(400.0));

	//CUBO VERDE
	gObjects.push_back(new Cubo(100));
}
void Scene::scene6(){
	//TEXTURAS
	Texture* noche = new Texture();
	noche->load("..\\Bmps\\noche.bmp", 200);
	gTextures.push_back(noche);

	//EJES RGB
	gObjects.push_back(new EjesRGB(400.0));

	//CREACION DE CUERPO COMPUESTO
	auto ce = new CompoundEntity();
	gObjects.push_back(ce);

	//NAVE
	auto esfera = new Sphere(100, 50, 50);
	ce->addEntity(esfera);

	//VENTANA
	auto escotilla = new CompoundEntity();
	auto cilindro = new Cylinder(80, 80, 50, 50, 50);
	cilindro->setModelMat(glm::rotate(cilindro->modelMat(), radians(90.0), dvec3(0, 1, 0)));
	escotilla->addEntity(cilindro);

	auto tapa = new Disk(0, 80, 50, 50);
	tapa->setModelMat(glm::translate(tapa->modelMat(), dvec3(50, 0, 0)));
	tapa->setModelMat(glm::rotate(tapa->modelMat(), radians(90.0), dvec3(0, 1, 0)));
	escotilla->addEntity(tapa);

	//mover la escotilla a su posicion
	escotilla->setModelMat(glm::translate(escotilla->modelMat(), dvec3(60, 0, 0)));
	ce->addEntity(escotilla);

	//UNION ALAS
	auto cilindro_Centro = new Cylinder(20, 20, 400, 50, 50);
	cilindro_Centro->setModelMat(glm::translate(cilindro_Centro->modelMat(), dvec3(0, 0, -200)));
	ce->addEntity(cilindro_Centro);

	//ALAS
	auto ala_izquierda = new DiskText(0, 300, 6, 6);
	ala_izquierda->setModelMat(glm::translate(ala_izquierda->modelMat(), dvec3(0, 0, -200)));
	ala_izquierda->setModelMat(glm::rotate(ala_izquierda->modelMat(), radians(30.0), dvec3(0, 0, 1)));
	ala_izquierda->setTexture(noche);
	ce->addEntityTranslucid(ala_izquierda);

	auto ala_derecha = new DiskText(0, 300, 6, 6);
	ala_derecha->setModelMat(glm::translate(ala_derecha->modelMat(), dvec3(0, 0, 200)));
	ala_derecha->setModelMat(glm::rotate(ala_derecha->modelMat(), radians(30.0), dvec3(0, 0, 1)));
	ala_derecha->setTexture(noche);
	ce->addEntityTranslucid(ala_derecha);

	//TRASLACION OBJETO COMPUESTO
	ce->setModelMat(glm::translate(ce->modelMat(), dvec3(0, 200, 0)));
	//ce->setModelMat(glm::translate(ce->modelMat(), dvec3(0, 0, 200)));
}