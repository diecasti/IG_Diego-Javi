//#pragma once
#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Mesh.h"
#include "Texture.h"

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
	void setTexture(Texture* tex) { mTexture = tex; };
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
#endif //_H_Entities_H_