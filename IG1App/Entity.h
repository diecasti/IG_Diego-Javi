//#pragma once
#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

//-------------------------------------------------------------------------

class Abs_Entity  // abstract class
{
public:
	Abs_Entity() : mModelMat(1.0) {};  // 4x4 identity matrix
	virtual ~Abs_Entity() {};			//!No se como definir la constructora -> delete this;??

	Abs_Entity(const Abs_Entity& e) = delete;  // no copy constructor
	Abs_Entity& operator=(const Abs_Entity& e) = delete;  // no copy assignment

	virtual void render(glm::dmat4 const& modelViewMat) const = 0;  // abstract method

	// modeling matrix
	glm::dmat4 const& modelMat() const { return mModelMat; };
	void setModelMat(glm::dmat4 const& aMat) { mModelMat = aMat; };

	glm::dvec4 mColor = { 1.0, 1.0, 1.0, 1 };				//Atributo para el color
	void setColor(glm::dvec4 color) { mColor = color; };	//Metodo para cambiar color

	virtual void update() {};	//Definimos el metodo vacio

	//texturas
	virtual void setTexture(Texture* tex) { mTexture = tex; };
protected:

	Mesh* mMesh = nullptr;   // the mesh
	glm::dmat4 mModelMat;    // modeling matrix

	Texture* mTexture = nullptr; // la textura to flamas

	// transfers modelViewMat to the GPU
	virtual void upload(glm::dmat4 const& mModelViewMat) const;
	double anguloDesplazamiento;
};
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
class EjesRGB : public Abs_Entity
{
public:
	explicit EjesRGB(GLdouble l);
	~EjesRGB();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
class Poligono : public Abs_Entity
{
public:
	Poligono(GLuint numL, GLdouble l);
	~Poligono();
	virtual void render(glm::dmat4 const& modelViewMat)const;
};
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
class Sierpinski :public Abs_Entity {
public:
	Sierpinski(GLdouble rd, GLuint numP);
	~Sierpinski();
	virtual void render(glm::dmat4 const& modelViewMat)const;
};
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
class TrianguloRGB : public Abs_Entity {
public:
	TrianguloRGB(GLdouble rd);
	~TrianguloRGB();
	virtual void render(glm::dmat4 const& modelViewMat)const;
	void update();
};
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
class RectanguloRGB : public Abs_Entity {
public:
	RectanguloRGB(GLdouble w, GLdouble h);
	~RectanguloRGB();
	virtual void render(glm::dmat4 const& modelViewMat)const;
};

//-------------------------------------------------------------------------
class ContCubo : public Abs_Entity {
public:
	ContCubo(GLdouble ld);
	~ContCubo();
	virtual void render(glm::dmat4 const& modelViewMat)const;
};
//-------------------------------------------------------------------------
class Estrella3D : public Abs_Entity {
public:
	Estrella3D(GLdouble re, GLuint np, GLdouble h);
	~Estrella3D();
	virtual void render(glm::dmat4 const& modelViewMat)const;
	void update();
};
//-------------------------------------------------------------------------
class CajaConFondo : public Abs_Entity {
public:
	CajaConFondo(GLdouble ld);
	~CajaConFondo();
	virtual void render(glm::dmat4 const& modelViewMat)const;
protected:
	Mesh* fondo = nullptr;   // the fondo
	GLdouble ld_ = NULL; // the lado
};

//-------------------------------------------------------------------------
class Suelo : public Abs_Entity {
public:
	Suelo(GLdouble w, GLdouble h, GLuint rw, GLuint rh);
	~Suelo();
	virtual void render(glm::dmat4 const& modelViewMat)const;
};
//-------------------------------------------------------------------------
class EstrellaTexCor : public Abs_Entity {
public:
	EstrellaTexCor(GLdouble re, GLuint np, GLdouble h);
	~EstrellaTexCor();
	virtual void render(glm::dmat4 const& modelViewMat)const;
	void update();
};
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
class ContCuboTexCo : public Abs_Entity {
public:
	ContCuboTexCo(GLdouble nl);
	~ContCuboTexCo();
	virtual void render(glm::dmat4 const& modelViewMat)const;
	void setTexture(Texture* tex, Texture* tex2) { mTexture = tex; interior = tex2; };
private:
	Texture* interior = nullptr;
};
//-------------------------------------------------------------------------//-------------------------------------------------------------------------
class CajaConFondoTx : public Abs_Entity {
public:
	CajaConFondoTx(GLdouble ld, GLdouble dist);
	~CajaConFondoTx();
	virtual void render(glm::dmat4 const& modelViewMat)const;
	void setTexture(Texture* tex, Texture* tex2) { mTexture = tex; interior = tex2; };
	void update();
protected:
	Mesh* fondo = nullptr;   // the fondo
	Texture* interior = nullptr;
	GLdouble ld_ = NULL; // the lado
	GLdouble distancia; // distancia al centro de coordenadas;
	GLdouble anguloDesplazamiento;

};
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
class Foto : public Abs_Entity {
public:
	Foto(GLdouble w, GLdouble h);
	~Foto();
	virtual void render(glm::dmat4 const& modelViewMat)const;
	void update();
};
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
class Hierba : public Abs_Entity {
public:
	Hierba(GLdouble w, GLdouble h);
	~Hierba();
	virtual void render(glm::dmat4 const& modelViewMat)const;
};
//-------------------------------------------------------------------------
//PRACTICA 2
//-------------------------------------------------------------------------
//!CLASES PRACTICA 2
class QuadricEntity : public Abs_Entity
{
public:
	QuadricEntity() { q = gluNewQuadric(); };
	virtual ~QuadricEntity() { gluDeleteQuadric(q); };

protected:
	GLUquadricObj* q;
};

class Sphere : public QuadricEntity {
public:
	Sphere(GLuint radio, GLuint slices, GLuint stacks);
	virtual ~Sphere();
	void render(glm::dmat4 const& modelViewMat) const;
private:
	GLuint radio;
	GLuint slices;
	GLuint stacks;

};

class Cylinder : public QuadricEntity {
public:
	Cylinder(GLdouble baseRadius, GLdouble topRadius, GLdouble height, GLint slices, GLint stacks);
	virtual ~Cylinder();
	void render(glm::dmat4 const& modelViewMat) const;
private:
	GLdouble baseRadius;
	GLdouble topRadius, height;
	GLint slices, stacks;
};

class Disk : public QuadricEntity {
public:
	Disk(GLdouble innerRadius,
		GLdouble outerRadius,
		GLint  slices,
		GLint loops);
	virtual ~Disk();
	void render(glm::dmat4 const& modelViewMat) const;
private:
	GLdouble innerRadius, outerRadius;
	GLint slices, loops;
};

class DiskText : public QuadricEntity {
public:
	DiskText(GLdouble innerRadius,
		GLdouble outerRadius,
		GLint  slices,
		GLint loops);
	virtual ~DiskText();
	void render(glm::dmat4 const& modelViewMat) const;
private:
	GLdouble innerRadius, outerRadius;
	GLint slices, loops;
};

class PartialDisk : public QuadricEntity {
public:
	PartialDisk(GLdouble            innerRadius,
		GLdouble            outerRadius,
		GLint               slices,
		GLint               loops,
		GLdouble            startAngle,
		GLdouble            sweepAngle);
	virtual ~PartialDisk();
	void render(glm::dmat4 const& modelViewMat) const;
private:
	GLdouble innerRadius, outerRadius, startAngle, sweepAngle;
	GLint slices, loops;
};
//----------------------------------------------------------
class AnilloCuadrado : public Abs_Entity {
public:
	AnilloCuadrado();
	virtual ~AnilloCuadrado();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};
//----------------------------------------------------------
class Cubo : public Abs_Entity {
public:
	Cubo(int l);
	virtual ~Cubo();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};
//----------------------------------------------------------
class EntityWithIndexMesh: public Abs_Entity {
public:
	EntityWithIndexMesh();
	~EntityWithIndexMesh();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};
//----------------------------------------------------------
class CompoundEntity : public Abs_Entity {
public:
	CompoundEntity();
	~CompoundEntity();
	void addEntity(Abs_Entity* ae) { gObjects.push_back(ae); }
	void addEntityTranslucid(Abs_Entity* ae) { gTranslucid.push_back(ae); }
	virtual void render(glm::dmat4 const& modelViewMat) const;
protected:
	std::vector<Abs_Entity*> gObjects;
	std::vector<Abs_Entity*> gTranslucid;

};
//----------------------------------------------------------
//----------------------------------------------------------
//ENTIDADES PRACTICA 2.2
class TIE : public CompoundEntity {
public:
	TIE(GLdouble tamanyo, Texture* textura, GLuint slices);
	~TIE() {}
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

class Cone : public Abs_Entity {
public:
	Cone(GLdouble h, GLdouble r, GLuint n);
	~Cone() {}
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

//class Esfera : public Abs_Entity {
//public:
//	Esfera(GLdouble r, GLdouble p, GLuint n);
//	~Esfera() {}
//	virtual void render(glm::dmat4 const& modelViewMat) const;
//};
//----------------------------------------------------------
class Rejilla : public Abs_Entity {
public:
	Rejilla(GLdouble lado, int nDiv);
	~Rejilla() {}
	virtual void render(glm::dmat4 const& modelViewMat) const;
};
//----------------------------------------------------------
class RejillaTex : public Abs_Entity {
public:
	RejillaTex(GLdouble lado, int nDiv);
	~RejillaTex() {}
	virtual void render(glm::dmat4 const& modelViewMat) const;
};
//----------------------------------------------------------
//----------------------------------------------------------
class GridCube : public CompoundEntity {
public:
	GridCube(GLdouble lado, int nDiv, Texture* tapa, Texture* pared);
	~GridCube() {}
	virtual void render(glm::dmat4 const& modelViewMat) const;
};
//----------------------------------------------------------
class EntityWithMaterial : public Abs_Entity {
public:
	EntityWithMaterial() : Abs_Entity() { };
	virtual ~EntityWithMaterial() { };
	void setMaterial(Material* matl) { material = matl; };
protected:
	Material* material = nullptr;
};
//--------------------------------------------------------------------
class Esfera : public EntityWithMaterial {
public:
	Esfera(GLdouble r, GLdouble p, GLuint n);
	~Esfera() {}
	virtual void render(glm::dmat4 const& modelViewMat) const;
};
#endif //_H_Entities_H_