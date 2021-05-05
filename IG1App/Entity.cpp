#include "Entity.h"

#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>
#include "IG1App.h"

using namespace glm;

//-------------------------------------------------------------------------

void Abs_Entity::upload(dmat4 const& modelViewMat) const
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(modelViewMat));  // transfers modelView matrix to the GPU
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

EjesRGB::EjesRGB(GLdouble l) : Abs_Entity()
{
	mMesh = Mesh::createRGBAxes(l);
}
//-------------------------------------------------------------------------

EjesRGB::~EjesRGB()
{
	delete mMesh; mMesh = nullptr;
};
//-------------------------------------------------------------------------

void EjesRGB::render(dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		glEnable(GL_COLOR_MATERIAL);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
		glDisable(GL_COLOR_MATERIAL);
	}
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
Poligono::Poligono(GLuint numL, GLdouble l) : Abs_Entity()
{
	mMesh = Mesh::generaPoligono(numL, l);
}
//-------------------------------------------------------------------------
Poligono::~Poligono()
{
	delete mMesh; mMesh = nullptr;
};
//-------------------------------------------------------------------------
void Poligono::render(glm::dmat4 const& modelViewMat)const
{
	if (mMesh != nullptr) {
		glm::dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glLineWidth(2);
		glColor3d(mColor.r, mColor.g, mColor.b);
		mMesh->render();
		glLineWidth(1);
		glColor3d(1, 1, 1);
	}
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
Sierpinski::Sierpinski(GLdouble rd, GLuint numP) {
	mMesh = Mesh::generaSierpinski(rd, numP);
}
//-------------------------------------------------------------------------
Sierpinski::~Sierpinski()
{
	delete mMesh; mMesh = nullptr;
};
//-------------------------------------------------------------------------
void Sierpinski::render(glm::dmat4 const& modelViewMat)const
{
	if (mMesh != nullptr) {
		glm::dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glLineWidth(2);
		glColor4dv(value_ptr(mColor));
		mMesh->render();
		glLineWidth(1);
		glColor4d(1.0, 1.0, 1.0, 1);
	}
}
//-------------------------------------------------------------------------
//Qué significa la v de 'glColor4dv(value_ptr(mColor));'
//Para qué sirve value_ptr();
//-------------------------------------------------------------------------
TrianguloRGB::TrianguloRGB(GLdouble rd) {
	mMesh = Mesh::generaTrianguloRGB(rd);
	anguloDesplazamiento = 0;
}
//-------------------------------------------------------------------------
TrianguloRGB::~TrianguloRGB() {
	delete mMesh; mMesh = nullptr;
}
//-------------------------------------------------------------------------
void TrianguloRGB::render(glm::dmat4 const& modelViewMat)const
{
	if (mMesh != nullptr) {
		glm::dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glLineWidth(2);
		//glColor4dv(value_ptr(mColor));
		glPolygonMode(GL_BACK, GL_LINE);
		glPolygonMode(GL_FRONT, GL_FILL);
		mMesh->render();
		glLineWidth(1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
//-------------------------------------------------------------------------
void TrianguloRGB::update() {
	//Coordenadas de traslación
	double x = 0;
	double y = 0;

	anguloDesplazamiento++;
	//Actualizamos las coordenadas de traslación respecto al radio del círculo (100) y el angulo de desplazamiento
	x = 200 * cos(radians(anguloDesplazamiento));
	y = 200 * sin(radians(anguloDesplazamiento));


	//Primero se realiza la traslación con la matriz identidad 
	setModelMat(translate(dmat4(1), dvec3(x, y, 0.0)));
	//Después se rota utilizando la propia matriz (para que rote sobre si mismo)
	setModelMat(rotate(modelMat(), radians(-anguloDesplazamiento), dvec3(0, 0, 1)));
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
RectanguloRGB::RectanguloRGB(GLdouble w, GLdouble h) {
	mMesh = Mesh::generaRectanguloRGB(w, h);
}
//-------------------------------------------------------------------------
RectanguloRGB::~RectanguloRGB() {
	delete mMesh; mMesh = nullptr;
}
//-------------------------------------------------------------------------
void RectanguloRGB::render(glm::dmat4 const& modelViewMat)const
{
	if (mMesh != nullptr) {
		glm::dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glLineWidth(2);
		glPolygonMode(GL_BACK, GL_LINE);
		glPolygonMode(GL_FRONT, GL_FILL);
		mMesh->render();
		glLineWidth(1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}
//-------------------------------------------------------------------------
Estrella3D::Estrella3D(GLdouble re, GLuint np, GLdouble h) {
	mMesh = Mesh::generaEstrella3D(re, np, h);
	anguloDesplazamiento = 0;
}
//-------------------------------------------------------------------------
Estrella3D::~Estrella3D() {
	delete mMesh; mMesh = nullptr;
}
//-------------------------------------------------------------------------
void Estrella3D::render(glm::dmat4 const& modelViewMat)const
{
	if (mMesh != nullptr) {
		glm::dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		//upload(aMat);
		glLineWidth(2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mMesh->render();

		//Segunda matriz
		aMat = rotate(aMat, radians(180.0), dvec3(1, 0, 0));
		upload(aMat);
		mMesh->render();

		glLineWidth(1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}

}
//-------------------------------------------------------------------------



void Estrella3D::update() {
	anguloDesplazamiento = anguloDesplazamiento + 0.5;
	//Actualizamos giro en Z
	dmat4 aMat = rotate(dmat4(1), radians(anguloDesplazamiento), dvec3(0, 1.0, 0));
	aMat = rotate(aMat, radians(anguloDesplazamiento), dvec3(0, 0, 1.0));


	//Actualizar giro en Y
	setModelMat(aMat);
	upload(aMat);
}
//-------------------------------------------------------------------------
ContCubo::ContCubo(GLdouble ld) {
	mMesh = Mesh::generaContCubo(ld);
}
//-------------------------------------------------------------------------
ContCubo::~ContCubo() {
	delete mMesh; mMesh = nullptr;
}
//-------------------------------------------------------------------------
void ContCubo::render(glm::dmat4 const& modelViewMat)const
{
	if (mMesh != nullptr) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glm::dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
	}
}
//-------------------------------------------------------------------------
CajaConFondo::CajaConFondo(GLdouble ld) {
	mMesh = Mesh::generaContCubo(ld);
	fondo = Mesh::generaRectangulo(ld, ld);
	ld_ = ld;
}
//-------------------------------------------------------------------------
CajaConFondo::~CajaConFondo() {
	delete mMesh; mMesh = nullptr;
	delete fondo; fondo = nullptr;
}
//-------------------------------------------------------------------------
void CajaConFondo::render(glm::dmat4 const& modelViewMat)const
{
	if (mMesh != nullptr) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glm::dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glLineWidth(2);
		mMesh->render();
		//ahora el fondo hayq ue ahcer la translacion y bla bla
		auto bMat = rotate(aMat, radians(-90.0), dvec3(1.0, 0, 0));
		bMat = translate(bMat, dvec3(0, 0, -ld_ / 2));
		upload(bMat);
		fondo->render();
		glLineWidth(1);
	}
}
//-------------------------------------------------------------------------
Suelo::Suelo(GLdouble w, GLdouble h, GLuint rw, GLuint rh) {
	mMesh = Mesh::generaRectanguloTexCor(w, h, rw, rh);
}
//-------------------------------------------------------------------------
Suelo::~Suelo() {
	delete mMesh; mMesh = nullptr;
}
//-------------------------------------------------------------------------
void Suelo::render(glm::dmat4 const& modelViewMat)const
{
	if (mMesh != nullptr) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glm::dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		//mTexture->bind(GL_REPLACE);
		glColor4dv(value_ptr(mColor));
		mTexture->bind(GL_MODULATE);
		mMesh->render();
		glLineWidth(1);
		mTexture->unbind();
	}
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
EstrellaTexCor::EstrellaTexCor(GLdouble re, GLuint np, GLdouble h) {
	mMesh = Mesh::generaEstrellaTexCor(re, np, h);
	anguloDesplazamiento = 0;
}
//-------------------------------------------------------------------------
EstrellaTexCor::~EstrellaTexCor() {
	delete mMesh; mMesh = nullptr;
}
//-------------------------------------------------------------------------
void EstrellaTexCor::render(glm::dmat4 const& modelViewMat)const
{
	if (mMesh != nullptr) {
		glm::dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		//upload(aMat);
		glLineWidth(2);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor4dv(value_ptr(mColor));
		mTexture->bind(GL_REPLACE);
		mMesh->render();

		//Segunda matriz
		aMat = rotate(aMat, radians(180.0), dvec3(0, 1, 0));
		upload(aMat);
		mMesh->render();

		glLineWidth(1);
		mTexture->unbind();

	}

}
//-------------------------------------------------------------------------



void EstrellaTexCor::update() {
	anguloDesplazamiento = anguloDesplazamiento + 0.5;
	//Actualizamos giro en Z
	mModelMat = translate(dmat4(1), dvec3(100, 200, 100));

	mModelMat = rotate(mModelMat, radians(anguloDesplazamiento), dvec3(0, 1.0, 0));
	mModelMat = rotate(mModelMat, radians(anguloDesplazamiento), dvec3(0, 0, 1.0));
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
ContCuboTexCo::ContCuboTexCo(GLdouble nl) {
	mMesh = Mesh::generaContCuboTexCor(nl);
}
//-------------------------------------------------------------------------
ContCuboTexCo::~ContCuboTexCo() {
	delete mMesh; mMesh = nullptr;
}
//-------------------------------------------------------------------------
void ContCuboTexCo::render(glm::dmat4 const& modelViewMat)const
{
	if (mMesh != nullptr) {

		glDepthMask(GL_FALSE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glm::dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glLineWidth(2);


		//las texturas, supongo que ahbra que hacer dos renders segun he entendido


		glColor4dv(value_ptr(mColor));
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		mTexture->bind(GL_MODULATE);
		mMesh->render();
		mTexture->unbind();

		//INTERIOR
		glCullFace(GL_BACK);
		interior->bind(GL_MODULATE);
		mMesh->render();
		interior->unbind();

		glLineWidth(1);
		glDisable(GL_CULL_FACE);
		//interior->unbind();

		glDepthMask(GL_TRUE);
	}
}
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
CajaConFondoTx::CajaConFondoTx(GLdouble nl, GLdouble dist) {
	mMesh = Mesh::generaContCuboTexCor(nl);
	fondo = Mesh::generaRectanguloTexCor(nl, nl, 1, 1);
	ld_ = nl;
	distancia = dist;
	anguloDesplazamiento = 0;
}
//-------------------------------------------------------------------------
CajaConFondoTx::~CajaConFondoTx() {
	delete mMesh; mMesh = nullptr;
	delete fondo; fondo = nullptr;
}
//-------------------------------------------------------------------------
void CajaConFondoTx::render(glm::dmat4 const& modelViewMat)const
{
	if (mMesh != nullptr) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glm::dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		auto bMat = rotate(aMat, radians(-90.0), dvec3(1.0, 0, 0));
		bMat = translate(bMat, dvec3(0, 0, -ld_ / 2));


		upload(aMat);
		glLineWidth(2);


		//las texturas, supongo que ahbra que hacer dos renders segun he entendido


		glColor4dv(value_ptr(mColor));
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		mTexture->bind(GL_REPLACE);
		mMesh->render();
		upload(bMat);
		fondo->render();


		mTexture->unbind();

		//INTERIOR
		glCullFace(GL_BACK);
		interior->bind(GL_REPLACE);
		fondo->render();
		upload(aMat);
		mMesh->render();
		interior->unbind();

		glLineWidth(1);
		glDisable(GL_CULL_FACE);
		//interior->unbind();

	}
}
//-------------------------------------------------------------------------
void CajaConFondoTx::update() {
	anguloDesplazamiento = 1;

	//Primero se realiza la traslación con la matriz identidad 
	setModelMat(rotate(modelMat(), radians(anguloDesplazamiento), dvec3(0, 0, 1)));
}

//-------------------------------------------------------------------------
Foto::Foto(GLdouble w, GLdouble h) {
	mMesh = Mesh::generaRectanguloTexCor(w, h, 1, 1);
	mTexture = new Texture();
	mTexture->loadColorBuffer(IG1App::s_ig1app.winWidth(), IG1App::s_ig1app.winHeight(), GL_FRONT);
}
//-------------------------------------------------------------------------
Foto::~Foto() {
	delete mMesh; mMesh = nullptr;
	delete mTexture; mTexture = nullptr;
}
//-------------------------------------------------------------------------
void Foto::render(glm::dmat4 const& modelViewMat)const
{
	if (mMesh != nullptr) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glm::dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);

		//mTexture->bind(GL_REPLACE);
		glColor4dv(value_ptr(mColor));
		mTexture->bind(GL_MODULATE);
		mMesh->render();
		glLineWidth(1);
		mTexture->unbind();
	}
}
//-------------------------------------------------------------------------
void Foto::update() {
	mTexture->loadColorBuffer(IG1App::s_ig1app.winWidth(), IG1App::s_ig1app.winHeight(), GL_FRONT);
}


//-------------------------------------------------------------------------
Hierba::Hierba(GLdouble w, GLdouble h) {
	mMesh = Mesh::generaRectanguloTexCor(w, h, 1, 1);
}
//-------------------------------------------------------------------------
Hierba::~Hierba() {
	delete mMesh; mMesh = nullptr;
}
//-------------------------------------------------------------------------
void Hierba::render(glm::dmat4 const& modelViewMat)const
{

	if (mMesh != nullptr) {

		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0);


		glm::dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		//upload(aMat);
		glLineWidth(2);
		glColor4dv(value_ptr(mColor));

		mTexture->bind(GL_MODULATE);
		mMesh->render();


		for (int i = 0; i < 2; i++) {
			aMat = rotate(aMat, radians(60.0), dvec3(0, 1, 0));
			upload(aMat);
			mMesh->render();
		}


		glLineWidth(1);
		mTexture->unbind();

		glDisable(GL_ALPHA_TEST);

	}
}
//-------------------------------------------------------------------------
//!PRACTICA 2
//!Clases prÃ¡ctica 2
//-------------------------------------------------------------------------
Sphere::Sphere(GLuint r, GLuint s, GLuint st) : QuadricEntity() {
	radio = r;
	slices = s;
	stacks = st;
}
Sphere::~Sphere() {
}

void Sphere::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(0.0, 0.25, 0.41);
	gluQuadricDrawStyle(q, GL_FILL);
	gluSphere(q, radio, slices, stacks);
	glColor3f(1.0, 1.0, 1.0);
	glDisable(GL_COLOR_MATERIAL);

}
//-------------------------------------------------------------------------
Cylinder::Cylinder(GLdouble br, GLdouble tr, GLdouble h, GLint s, GLint st) : QuadricEntity() {
	baseRadius = br;
	topRadius = tr;
	height = h;
	slices = s;
	stacks = st;
}
Cylinder::~Cylinder() {

}

void Cylinder::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(0.0, 0.25, 0.41);
	gluQuadricDrawStyle(q, GL_FILL);

	gluCylinder(q, baseRadius, topRadius, height, slices, stacks);

	glColor3f(1.0, 1.0, 1.0);
	glDisable(GL_COLOR_MATERIAL);

}
//-------------------------------------------------------------------------
Disk::Disk(GLdouble ir, GLdouble ors, GLint s, GLint  l) : QuadricEntity() {
	innerRadius = ir;
	outerRadius = ors;
	slices = s;
	loops = l;
}
Disk::~Disk() {

}

void Disk::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(0.0, 0.25, 0.41);
	gluQuadricDrawStyle(q, GL_FILL);
	gluDisk(q, innerRadius, outerRadius, slices, loops);
	glColor3f(1.0, 1.0, 1.0);
	glDisable(GL_COLOR_MATERIAL);

}
//-------------------------------------------------------------------------
DiskText::DiskText(GLdouble ir, GLdouble ors, GLint s, GLint  l) : QuadricEntity() {
	innerRadius = ir;
	outerRadius = ors;
	slices = s;
	loops = l;
	gluQuadricTexture(q, GL_TRUE);
}
DiskText::~DiskText() {

}

void DiskText::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);

	glEnable(GL_COLOR_MATERIAL);
	mTexture->bind(GL_MODULATE);
	gluQuadricTexture(q, GL_TRUE);

	gluDisk(q, innerRadius, outerRadius, slices, loops);

	glDisable(GL_COLOR_MATERIAL);
	mTexture->unbind();
}
//-------------------------------------------------------------------------
PartialDisk::PartialDisk(GLdouble            ir,
	GLdouble            ors,
	GLint               s,
	GLint               l,
	GLdouble            sa,
	GLdouble            swa) : QuadricEntity() {
	innerRadius = ir;
	outerRadius = ors;
	slices = s;
	loops = l;
	startAngle = sa;
	sweepAngle = swa;
}
PartialDisk::~PartialDisk() {

}

void PartialDisk::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(0.0, 0.25, 0.41);
	gluQuadricDrawStyle(q, GL_FILL);
	gluPartialDisk(q, innerRadius, outerRadius, slices, loops, startAngle, sweepAngle);
	glColor3f(1.0, 1.0, 1.0);
	glDisable(GL_COLOR_MATERIAL);

}
//--------------------------------------------------------------------------------------------------------------
EntityWithIndexMesh::EntityWithIndexMesh() { mMesh = nullptr; }
EntityWithIndexMesh::~EntityWithIndexMesh() {}
void EntityWithIndexMesh::render(glm::dmat4 const& modelViewMat) const
{
	//if (mMesh != nullptr) {
	//	glm::dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
	//	upload(aMat);
	//	glEnable(GL_COLOR_MATERIAL);

	//	glLineWidth(2);
	//	glColor4dv(value_ptr(mColor));
	//	mMesh->render();
	//	glLineWidth(1);
	//	glColor4d(1.0, 1.0, 1.0, 1);
	//	glDisable(GL_COLOR_MATERIAL);
	//}
}
//--------------------------------------------------------------------------------------------------------------

AnilloCuadrado::AnilloCuadrado() {
	mMesh = IndexMesh::generaAnilloCuadradoIndexado();

}
AnilloCuadrado::~AnilloCuadrado() {}

void AnilloCuadrado::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		glm::dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glEnable(GL_COLOR_MATERIAL);

		glLineWidth(2);
		glColor4dv(value_ptr(mColor));
		mMesh->render();
		glLineWidth(1);
		glColor4d(1.0, 1.0, 1.0, 1);
		glDisable(GL_COLOR_MATERIAL);
	}
}

//--------------------------------------------------------------------------------------------------------------
Cubo::Cubo(int l) {
	mMesh = IndexMesh::generaCuboConTapasIndexado(l);

}
Cubo::~Cubo() {}

void Cubo::render(glm::dmat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		glm::dmat4 aMat = modelViewMat *  mModelMat;  // glm matrix multiplication
		upload(aMat);
		glEnable(GL_COLOR_MATERIAL);

		glLineWidth(2);
		glColor4dv(value_ptr(mColor));
		mMesh->render();
		glLineWidth(1);
		glColor4d(1.0, 1.0, 1.0, 1);
		glDisable(GL_COLOR_MATERIAL);

	}
}
//--------------------------------------------------------------------------------------------------------------
CompoundEntity::CompoundEntity() {
	//no tiene mesh
}
CompoundEntity::~CompoundEntity() {
	////Literalmente free() de scene
	//for (Abs_Entity* el : gObjects)
	//{
	//	delete el;  el = nullptr;
	//}
	//gObjects.clear();
	//for (Texture* el : gTextures)
	//{
	//	delete el;  el = nullptr;
	//}
	//gTextures.clear();
	//for (Abs_Entity* el : gObjectsTranslucid)
	//{
	//	delete el;  el = nullptr;
	//}
	//gObjectsTranslucid.clear();
}

void CompoundEntity::render(glm::dmat4 const& modelViewMat) const
{
	glm::dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
	//TODO, en las diapos dice que las upladeemos, pero realmente no es necesario, asi que arrideverchi
	upload(aMat);
		//ahora renderizar el resto de objetos respecto a esta aMat
	
	for (Abs_Entity* el : gObjects)
	{
		el->render(aMat);
	}
	//los elementos translucidos
	for (Abs_Entity* el : gTranslucid)
	{
		el->render(aMat);
	}
	
	//y ya estaria segun als diapos

	//esto importa shiet
	/*
	if (mMesh != nullptr) {
		upload(aMat);
		glEnable(GL_COLOR_MATERIAL);

		glLineWidth(2);
		glColor4dv(value_ptr(mColor));
		mMesh->render();
		glLineWidth(1);
		glColor4d(1.0, 1.0, 1.0, 1);
		glDisable(GL_COLOR_MATERIAL);
	}
	*/
}
