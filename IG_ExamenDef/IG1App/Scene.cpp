#include "Scene.h"
#include "CheckML.h"
#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>

using namespace glm;
//-------------------------------------------------------------------------

bool Scene::lucesEncendidas = false;

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
	else if (mId == 7)
		scene7();
	if (!lucesEncendidas) {
		createLights();
	}

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
		else if (mId == 7) {
			scene7();
		}
		else if (mId == 8) {
			scene8();
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

	tieGroup = nullptr;


}
//-------------------------------------------------------------------------
void Scene::setGL(int id)
{
	// OpenGL basic setting
	glClearColor(0.7, 0.8, 0.9, 1.0);

	glEnable(GL_DEPTH_TEST);  // enable Depth test 

	//Textures
	if (sceneWithTexture[mId])
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);

	//Luces
	if (sceneWithLight[mId])
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);


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
	if (posLight != nullptr)
		posLight->upload(cam.viewMat());
	if (spotLight != nullptr)
		spotLight->upload(cam.viewMat());
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

void Scene::createLights()
{
	//No tiene posicion, tiene direccion
	dirLight = new DirLight();
	dirLight->setDiffuse({ 1, 1, 1, 1 });
	dirLight->setAmb({ 0, 0, 0, 1 });
	dirLight->setSpecular({ 0.5, 0.5, 0.5, 1 });
	dirLight->setPosDir({ 1, 1, 1 });

	//Luz omnidireccional
	posLight = new PosLight();
	posLight->setDiffuse({ 1, 0, 0, 1 });	
	posLight->setAmb({ 0.0, 0.0, 0, 1 });					//COLOR AMBIENTE
	posLight->setSpecular({ 0.5, 0.5, 0.5, 1 });
	posLight->setPosDir({ 600, 200, 0 });

	//Foco
	spotLight = new SpotLight();
	spotLight->setDiffuse({ 1, 1, 1, 1 });					//COLOR
	spotLight->setAmb({ 0, 0, 0, 1 });						//COLOR DE LA OSCURIDAD
	spotLight->setSpecular({ 0.5, 0.5, 0.5, 1 });
	spotLight->setPosDir({ 0, 0, 800 });
	spotLight->setSpot(glm::fvec3(0.0, 0.0, -1.0), 60, 0);

	lucesEncendidas = true;
}

void Scene::desactivarLuces()
{
	if (tieGroup != nullptr) TIEsLightsOff();
	if (dirLight != nullptr) dirLight->disable();
	if (posLight != nullptr) posLight->disable();
	if (spotLight != nullptr) spotLight->disable();
}

void Scene::TIEsLightsOn()
{
	for (Abs_Entity* tie : tieGroup->gObjects)
		static_cast<TIE*>(tie)->focoDeLuz()->enable();
}



void Scene::TIEsLightsOff()
{
	for (Abs_Entity* tie : tieGroup->gObjects)
		static_cast<TIE*>(tie)->focoDeLuz()->disable();
}

void Scene::defaultLighting()
{
	if (tieGroup != nullptr) TIEsLightsOff();
	desactivarLuces();
	//luz ambiente tenue
	GLfloat amb[] = { 0.2, 0.2, 0.2, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
}

void Scene::darkScene()
{
	desactivarLuces();
	//no hay luz, todo negro
	GLfloat amb[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
}

////void Scene::scene3() {
////	//TEXTURAS
////	Texture* noche = new Texture();
////	noche->load("..\\Bmps\\noche.bmp", 200);
////	gTextures.push_back(noche);
////
////	//EJES RGB
////	gObjects.push_back(new EjesRGB(400.0));
////
////	//NAVE
////	gObjects.push_back(new TIE(100, noche, 50));
////
////}
////
////void Scene::scene1() {
////	//EJES RGB
////	gObjects.push_back(new EjesRGB(400.0));
////
////	//ANILLO MULTICOLOR
////	gObjects.push_back(new AnilloCuadrado());
////}
////void Scene::scene2() {
////	//EJES RGB
////	gObjects.push_back(new EjesRGB(400.0));
////
////	//CUBO VERDE
////	gObjects.push_back(new Cubo(100));
////}
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
////void Scene::scene0() {
////
////	glClearColor(.0, .0, .0, .0);  // background color (alpha=1 -> opaque)
////
////	//EJES RGB
////	gObjects.push_back(new EjesRGB(400.0));
////}
////void Scene::scene4() {
////	//EJES RGB
////	gObjects.push_back(new EjesRGB(400.0));
////	//escena dos bolas
////
////	auto bola = new Esfera(100, 50, 50);
////
////	gObjects.push_back(bola);
////	gObjects.back()->setModelMat(glm::translate(gObjects.back()->modelMat(), dvec3(0, 0, +200)));
////	gObjects.back()->setColor(dvec4(0.0, 0.0, 1.0, 1.0));
////
////	//cuadratica
////
////	gObjects.push_back(new Sphere(100, 50, 50));
////	gObjects.back()->setModelMat(glm::translate(gObjects.back()->modelMat(), dvec3(+200, 0, 0)));
////	gObjects.back()->setColor(dvec4(0.0, 0.0, 1.0, 1.0));
////
////}
////void Scene::scene5() {
////
////	//EJES RGB
////	gObjects.push_back(new EjesRGB(1000.0));
////	//Escena CUBO, esta es la escena 5
////
////	Texture* ajedrez = new Texture();
////	ajedrez->load("..\\Bmps\\checker.bmp");
////	gTextures.push_back(ajedrez);
////
////	Texture* piedra = new Texture();
////	piedra->load("..\\Bmps\\stones.bmp");
////	gTextures.push_back(piedra);
////
////	auto cubo = new GridCube(1200, 100, ajedrez, piedra);
////	gObjects.push_back(cubo);
////}
////
////void Scene::scene6() {
////
////	glClearColor(.0, .0, .0, .0);  // background color (alpha=1 -> opaque)
////
////	//TEXTURAS
////	Texture* noche = new Texture();
////	noche->load("..\\Bmps\\noche.bmp", 200);
////	gTextures.push_back(noche);
////
////
////	Material* laton = new Material();
////	laton->setCopper();
////
////	//EJES RGB
////	gObjects.push_back(new EjesRGB(4000.0));
////
////
////	auto bola = new Esfera(600, 200, 200);
////	bola->setMaterial(laton);
////
////	gObjects.push_back(bola);
////	gObjects.back()->setColor(dvec4(0.0, 1.0, 1.0, 1.0));
////
////	tieGroup = new CompoundEntity();
////	gObjects.push_back(tieGroup);
////
////
////
////	auto tie1 = new TIE(10, noche, 50);
////	tie1->setModelMat(glm::translate(tie1->modelMat(), dvec3(0, 110 + 650, 0)));
////	tieGroup->addEntity(tie1);
////
////
////	auto tie2 = new TIE(10, noche, 50);
////	tie2->setModelMat(glm::translate(tie2->modelMat(), dvec3(-50, 108 + 650, -50)));
////	tie2->setModelMat(glm::rotate(tie2->modelMat(), radians(15.0), dvec3(0, 1, 1)));
////	tieGroup->addEntity(tie2);
////
////
////	auto tie3 = new TIE(10, noche, 50);
////	tie3->setModelMat(glm::translate(tie3->modelMat(), dvec3(-60, 105 + 650, 50)));
////	tie3->setModelMat(glm::rotate(tie3->modelMat(), radians(10.0), dvec3(0, 1, -1)));
////	tieGroup->addEntity(tie3);
////
////	tieGroup->setModelMat(glm::translate(tieGroup->modelMat(), dvec3(100,0,0)));
////
////
////
////}


void Scene::orbita()
{
	dmat4 mat = tieGroup->modelMat();
	//mat = translate(mat, dvec3(0.0, -350.0, 0.0));
	mat = rotate(mat, radians(-1.0), dvec3(0, 0.0, 1));
	//mat = translate(mat, dvec3(0.0, 350.0, 0.0));

	tieGroup->setModelMat(mat);
}
//-------------------------------------------------------------------------

void Scene::rota()
{
	dmat4 mat = tieGroup->modelMat();
	mat = rotate(mat, radians(tieLocalAngle), dvec3(0.0, 1.0, 0.0));

	tieLocalAngle += 0.1;
	tieGroup->setModelMat(mat);
}


//---TODAS LAS ESCENAS---//
//PRACTICA 1 EJERCICIOS 1-7
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
//PRACTICA 1 EJERCICIOS 8-26
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
	gObjects.push_back(j);
}
//PRACTICA 2 EJERCICIOS 1-15
void Scene::scene2() {
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
	esfera->setColor(dvec4(0.0, 0.25, 0.41, 1.0));
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
void Scene::scene3() {
	//EJES RGB
	gObjects.push_back(new EjesRGB(400.0));

	//ANILLO MULTICOLOR
	gObjects.push_back(new AnilloCuadrado());
}
void Scene::scene4() {
	//EJES RGB
	gObjects.push_back(new EjesRGB(400.0));

	//CUBO VERDE
	gObjects.push_back(new Cubo(100));
}
//PRACTICA 2 EJERCICIOS 15-34

void Scene::scene5() {
	//EJES RGB
	gObjects.push_back(new EjesRGB(400.0));
	//escena dos bolas

	auto bola = new Esfera(100, 50, 50);

	gObjects.push_back(bola);
	gObjects.back()->setModelMat(glm::translate(gObjects.back()->modelMat(), dvec3(0, 0, +200)));
	gObjects.back()->setColor(dvec4(0.0, 0.0, 1.0, 1.0));

	//cuadratica

	gObjects.push_back(new Sphere(100, 50, 50));
	gObjects.back()->setModelMat(glm::translate(gObjects.back()->modelMat(), dvec3(+200, 0, 0)));
	gObjects.back()->setColor(dvec4(0.0, 0.0, 1.0, 1.0));

}
void Scene::scene6() {

	//EJES RGB
	gObjects.push_back(new EjesRGB(1000.0));
	//Escena CUBO, esta es la escena 5

	Texture* ajedrez = new Texture();
	ajedrez->load("..\\Bmps\\checker.bmp");
	gTextures.push_back(ajedrez);

	Texture* piedra = new Texture();
	piedra->load("..\\Bmps\\stones.bmp");
	gTextures.push_back(piedra);

	auto cubo = new GridCube(1200, 100, ajedrez, piedra);
	gObjects.push_back(cubo);
	//gObjects.back()->setModelMat(glm::rotate(gObjects.back()->modelMat(), radians(45.0), dvec3(0, 0, 1)));
}

void Scene::scene7() {

	glClearColor(.0, .0, .0, .0);  // background color (alpha=1 -> opaque)

	//TEXTURAS
	Texture* noche = new Texture();
	noche->load("..\\Bmps\\noche.bmp", 200);
	gTextures.push_back(noche);


	Material* laton = new Material();
	laton->setCopper();

	//EJES RGB
	gObjects.push_back(new EjesRGB(4000.0));


	auto bola = new Esfera(600, 200, 200);
	bola->setMaterial(laton);

	gObjects.push_back(bola);
	gObjects.back()->setColor(dvec4(0.0, 1.0, 1.0, 1.0));

	tieGroup = new CompoundEntity();
	gObjects.push_back(tieGroup);



	auto tie1 = new TIE(10, noche, 50);
	tie1->setModelMat(glm::translate(tie1->modelMat(), dvec3(0, 110 + 650, 0)));
	tieGroup->addEntity(tie1);


	auto tie2 = new TIE(10, noche, 50);
	tie2->setModelMat(glm::translate(tie2->modelMat(), dvec3(-50, 108 + 650, -50)));
	tie2->setModelMat(glm::rotate(tie2->modelMat(), radians(15.0), dvec3(0, 1, 1)));
	tieGroup->addEntity(tie2);


	auto tie3 = new TIE(10, noche, 50);
	tie3->setModelMat(glm::translate(tie3->modelMat(), dvec3(-60, 105 + 650, 50)));
	tie3->setModelMat(glm::rotate(tie3->modelMat(), radians(10.0), dvec3(0, 1, -1)));
	tieGroup->addEntity(tie3);

	tieGroup->setModelMat(glm::translate(tieGroup->modelMat(), dvec3(100, 0, 0)));
}
void Scene::scene8() {
	//TEXTURAS
	Texture* noche = new Texture();
	noche->load("..\\Bmps\\noche.bmp", 200);
	gTextures.push_back(noche);


	//EJES RGB
	gObjects.push_back(new EjesRGB(4000.0));

	auto partial = new PartialDiskText(100,
		200,
		50,
		10,
		0,
		90);
	gObjects.push_back(partial);
	gObjects.back()->setColor(dvec4(1.0, 1.0, 0.0, 1.0));
	gObjects.back()->setTexture(noche);

}