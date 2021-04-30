//#pragma once
#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include "Mesh.h"

//-------------------------------------------------------------------------

class Abs_Entity  // abstract class
{
public:
	Abs_Entity(): mModelMat(1.0) {};  // 4x4 identity matrix
	virtual ~Abs_Entity() {};

	Abs_Entity(const Abs_Entity& e) = delete;  // no copy constructor
	Abs_Entity& operator=(const Abs_Entity& e) = delete;  // no copy assignment

	virtual void render(glm::dmat4 const& modelViewMat) const = 0;  // abstract method

	// modeling matrix
	glm::dmat4 const& modelMat() const { return mModelMat; };
	void setModelMat(glm::dmat4 const& aMat) { mModelMat = aMat; };
	
protected:

	Mesh* mMesh = nullptr;   // the mesh
	glm::dmat4 mModelMat;    // modeling matrix
	
	// transfers modelViewMat to the GPU
	virtual void upload(glm::dmat4 const& mModelViewMat) const; 
};
//-------------------------------------------------------------------------

class EjesRGB : public Abs_Entity 
{
public:
	explicit EjesRGB(GLdouble l);
	~EjesRGB();
	virtual void render(glm::dmat4 const& modelViewMat) const;
};

//-------------------------------------------------------------------------
//!CLASES PRACTICA 2
class QuadricEntity : public Abs_Entity
{
public:
	QuadricEntity() { q = gluNewQuadric();};
	~QuadricEntity() { gluDeleteQuadric(q); };

private:
	GLUquadricObj* q;
};

class Sphere : public QuadricEntity {
public:
	Sphere():QuadricEntity() { };
	~Sphere();
	//render(dmat4 const& modelViewMat) const { gluSphere(q, 100, 10, 10); };
};

class Cylinder : public QuadricEntity {
public:
	Cylinder();
	~Cylinder();

};

class Disk : public QuadricEntity {
public:
	Disk();
	~Disk();

};

class PartialDisk : public QuadricEntity {
public:
	PartialDisk();
	~PartialDisk();

};
#endif //_H_Entities_H_