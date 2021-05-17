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
	else if (mId == 3)
		scene3();

}
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

void Scene::scene3() {
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

void Scene::scene1() {
	//EJES RGB
	gObjects.push_back(new EjesRGB(400.0));

	//ANILLO MULTICOLOR
	gObjects.push_back(new AnilloCuadrado());
}
void Scene::scene2() {
	//EJES RGB
	gObjects.push_back(new EjesRGB(400.0));

	//CUBO VERDE
	gObjects.push_back(new Cubo(100));
}
//void Scene::scene0(){
//	//TEXTURAS
//	Texture* noche = new Texture();
//	noche->load("..\\Bmps\\noche.bmp", 200);
//	gTextures.push_back(noche);
//
//	//EJES RGB
//	gObjects.push_back(new EjesRGB(400.0));
//
//	//CREACION DE CUERPO COMPUESTO
//	auto ce = new CompoundEntity();
//	gObjects.push_back(ce);
//
//	//NAVE
//	auto esfera = new Sphere(100, 50, 50);
//	ce->addEntity(esfera);
//
//	//VENTANA
//	auto escotilla = new CompoundEntity();
//	auto cilindro = new Cylinder(80, 80, 50, 50, 50);
//	cilindro->setModelMat(glm::rotate(cilindro->modelMat(), radians(90.0), dvec3(0, 1, 0)));
//	escotilla->addEntity(cilindro);
//
//	auto tapa = new Disk(0, 80, 50, 50);
//	tapa->setModelMat(glm::translate(tapa->modelMat(), dvec3(50, 0, 0)));
//	tapa->setModelMat(glm::rotate(tapa->modelMat(), radians(90.0), dvec3(0, 1, 0)));
//	escotilla->addEntity(tapa);
//
//	//mover la escotilla a su posicion
//	escotilla->setModelMat(glm::translate(escotilla->modelMat(), dvec3(60, 0, 0)));
//	ce->addEntity(escotilla);
//
//	//UNION ALAS
//	auto cilindro_Centro = new Cylinder(20, 20, 400, 50, 50);
//	cilindro_Centro->setModelMat(glm::translate(cilindro_Centro->modelMat(), dvec3(0, 0, -200)));
//	ce->addEntity(cilindro_Centro);
//
//	//ALAS
//	auto ala_izquierda = new DiskText(0, 300, 6, 6);
//	ala_izquierda->setModelMat(glm::translate(ala_izquierda->modelMat(), dvec3(0, 0, -200)));
//	ala_izquierda->setModelMat(glm::rotate(ala_izquierda->modelMat(), radians(30.0), dvec3(0, 0, 1)));
//	ala_izquierda->setTexture(noche);
//	ce->addEntityTranslucid(ala_izquierda);
//
//	auto ala_derecha = new DiskText(0, 300, 6, 6);
//	ala_derecha->setModelMat(glm::translate(ala_derecha->modelMat(), dvec3(0, 0, 200)));
//	ala_derecha->setModelMat(glm::rotate(ala_derecha->modelMat(), radians(30.0), dvec3(0, 0, 1)));
//	ala_derecha->setTexture(noche);
//	ce->addEntityTranslucid(ala_derecha);
//
//	//TRASLACION OBJETO COMPUESTO
//	ce->setModelMat(glm::translate(ce->modelMat(), dvec3(0, 200, 0)));
//	//ce->setModelMat(glm::translate(ce->modelMat(), dvec3(0, 0, 200)));
//}

//ESCENAS PRACTICA 2.2
void Scene::scene0() {
	//EJES RGB
	gObjects.push_back(new EjesRGB(400.0));

	//escena cono
	//auto arbeloa = new Cone(500, 100, 6);

	//gObjects.push_back(arbeloa);

	//escena bola
	//auto bola = new Esfera(100, 10, 10);

	//gObjects.push_back(bola);

	//escena dos bolas

	auto bola = new Esfera(100, 50, 50);

	gObjects.push_back(bola);
	gObjects.back()->setModelMat(glm::translate(gObjects.back()->modelMat(), dvec3(0, 0, +200)));
	gObjects.back()->setColor(dvec4(0.0,0.0,1.0,1.0));

	//cuadratica

	gObjects.push_back(new Sphere(100, 50, 50));
	gObjects.back()->setModelMat(glm::translate(gObjects.back()->modelMat(), dvec3(+200, 0, 0)));
	gObjects.back()->setColor(dvec4(0.0, 0.0, 1.0, 1.0));
}