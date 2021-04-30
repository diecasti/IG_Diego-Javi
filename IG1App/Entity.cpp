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
	gluQuadricDrawStyle(q, GL_FILL);
	glColor4f(0.0, 0.25, 0.41, 0.0);
	gluCylinder(q, baseRadius, topRadius, height, slices, stacks);
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
	gluQuadricDrawStyle(q, GL_FILL);
	glColor4f(0.0, 0.25, 0.41, 0.0);
	gluDisk(q, innerRadius, outerRadius, slices ,loops);
	glDisable(GL_COLOR_MATERIAL);
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
	gluQuadricDrawStyle(q, GL_FILL);
	glColor4f(0.0, 0.25, 0.41, 0.0);
	gluPartialDisk(q, innerRadius, outerRadius, slices, loops, startAngle, sweepAngle);
	glDisable(GL_COLOR_MATERIAL);
}
//!Clases práctica 2
 