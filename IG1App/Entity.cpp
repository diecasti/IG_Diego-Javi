#include "Entity.h"

#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>

using namespace glm;

//-------------------------------------------------------------------------

void Abs_Entity::upload(dmat4 const& modelViewMat) const 
{ 
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(modelViewMat));  // transfers modelView matrix to the GPU
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

EjesRGB::EjesRGB(GLdouble l): Abs_Entity()
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
		glLineWidth(2);
		mMesh->render();
		glLineWidth(1);
	}
}
//-------------------------------------------------------------------------
Sphere::Sphere(GLuint r, GLuint s, GLuint st): QuadricEntity() {
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
		gluQuadricDrawStyle(q, GL_FILL);    
		glColor4f(0.0, 0.25, 0.41, 0.0);
		gluSphere(q, radio, slices, stacks);
		glDisable(GL_COLOR_MATERIAL);
}//-------------------------------------------------------------------------
Cylinder::Cylinder() : QuadricEntity() {

}
Cylinder::~Cylinder() {

}

void Cylinder::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	glEnable(GL_COLOR_MATERIAL);
	gluQuadricDrawStyle(q, GL_FILL);
	glColor4f(0.0, 0.25, 0.41, 0.0);
	gluCylinder(q, 100, 100, 50, 3, 3);
	glDisable(GL_COLOR_MATERIAL);
}
//-------------------------------------------------------------------------
Disk::Disk() : QuadricEntity() {

}
Disk::~Disk() {

}

void Disk::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	glEnable(GL_COLOR_MATERIAL);
	gluQuadricDrawStyle(q, GL_FILL);
	glColor4f(0.0, 0.25, 0.41, 0.0);
	gluDisk(q, 50, 100, 50, 50);
	glDisable(GL_COLOR_MATERIAL);
}
//-------------------------------------------------------------------------
PartialDisk::PartialDisk() : QuadricEntity() {

}
PartialDisk::~PartialDisk() {

}

void PartialDisk::render(glm::dmat4 const& modelViewMat) const
{
	dmat4 aMat = modelViewMat * mModelMat;
	upload(aMat);
	glEnable(GL_COLOR_MATERIAL);
	gluQuadricDrawStyle(q, GL_FILL);
	glColor4f(0.0, 0.25, 0.41, 0.0);
	gluPartialDisk(q, 50, 100, 50, 1, 0, 90);
	glDisable(GL_COLOR_MATERIAL);
}
//!Clases práctica 2
 