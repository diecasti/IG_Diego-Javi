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


void Sphere::render(glm::dmat4 const& modelViewMat) const
{
		dmat4 aMat = modelViewMat * mModelMat;     
		upload(aMat);      
		glEnable(GL_COLOR_MATERIAL);     
		gluQuadricDrawStyle(q, GL_FILL);    
		glColor4f(0.0, 0.25, 0.41, 0.0);
		gluSphere(q, 100, 50, 50);
		glDisable(GL_COLOR_MATERIAL);
}
//!Clases práctica 2
 