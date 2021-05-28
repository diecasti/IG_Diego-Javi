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
	else if (mId == 4)
		scene4();
	else if (mId == 5)
		scene5();
	else if (mId == 6)
		scene6();

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
	glClearColor(0.7, 0.8, 0.9, 1.0);

	glEnable(GL_DEPTH_TEST);  // enable Depth test 
	//Textures
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

}
//-------------------------------------------------------------------------
void Scene::resetGL()
{
	glClearColor(.0, .0, .0, .0);  // background color (alpha=1 -> opaque)
	glDisable(GL_DEPTH_TEST);  // disable Depth test 	
	glDisable(GL_TEXTURE_2D); //desactivar texturitas
	glDisable(GL_LIGHTING);
}
//-------------------------------------------------------------------------

void Scene::render(Camera const& cam) const
{
	if (dirLight != nullptr)
	dirLight->upload(cam.viewMat());
	//sceneDirLight(cam);
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

void Scene::createLight()
{
	dirLight = new DirLight;
	dirLight->setDiffuse({ 1, 1, 1, 1 });
	dirLight->setAmb({ 0, 0, 0, 1 });
	dirLight->setSpecular({ 0.5, 0.5, 0.5, 1 });
	dirLight->setPosDir({ 1, 1, 1 });
	dirLightOn = true;
}

void Scene::scene3() {
	//TEXTURAS
	Texture* noche = new Texture();
	noche->load("..\\Bmps\\noche.bmp", 200);
	gTextures.push_back(noche);

	//EJES RGB
	gObjects.push_back(new EjesRGB(400.0));

	//NAVE
	gObjects.push_back(new TIE(100, noche, 50));

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
	

}
void Scene::scene4() {
	//EJES RGB
	gObjects.push_back(new EjesRGB(400.0));
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
void Scene::scene5() {

	//EJES RGB
	gObjects.push_back(new EjesRGB(400.0));
	//Escena CUBO, esta es la escena 5

	Texture* ajedrez = new Texture();
	ajedrez->load("..\\Bmps\\checker.bmp");
	gTextures.push_back(ajedrez);

	Texture* piedra = new Texture();
	piedra->load("..\\Bmps\\stones.bmp");
	gTextures.push_back(piedra);

	auto cubo = new GridCube(100, 100, ajedrez, piedra);
	gObjects.push_back(cubo);
}

void Scene::scene6() {

	glClearColor(.0, .0, .0, .0);  // background color (alpha=1 -> opaque)

	//TEXTURAS
	Texture* noche = new Texture();
	noche->load("..\\Bmps\\noche.bmp", 200);
	gTextures.push_back(noche);

	//EJES RGB
	gObjects.push_back(new EjesRGB(4000.0));
	gObjects.back()->setModelMat(glm::translate(gObjects.back()->modelMat(), dvec3(0, -2000, 0)));


	auto bola = new Esfera(1000, 50, 50);

	gObjects.push_back(bola);
	gObjects.back()->setColor(dvec4(0.0, 1.0, 1.0, 1.0));

	auto ceTIEs = new CompoundEntity();
	gObjects.push_back(ceTIEs);


	auto tie1 = new TIE(10, noche, 50);
	tie1->setModelMat(glm::translate(tie1->modelMat(), dvec3(0, 1100, 0)));
	ceTIEs->addEntity(tie1);


	auto tie2 = new TIE(10, noche, 50);
	tie2->setModelMat(glm::translate(tie2->modelMat(), dvec3(-50, 1080, -50)));
	tie2->setModelMat(glm::rotate(tie2->modelMat(), radians(15.0), dvec3(0, 1, 1)));
	ceTIEs->addEntity(tie2);


	auto tie3 = new TIE(10, noche, 50);
	tie3->setModelMat(glm::translate(tie3->modelMat(), dvec3(-60, 1050, 50)));
	tie3->setModelMat(glm::rotate(tie3->modelMat(), radians(10.0), dvec3(0, 1, -1)));
	ceTIEs->addEntity(tie3);

	createLight();
}