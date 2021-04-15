#include "Camera.h"

#include <gtc/matrix_transform.hpp>  
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_access.hpp>
#include <iostream>

using namespace glm;

//-------------------------------------------------------------------------

Camera::Camera(Viewport* vp) : mViewPort(vp), mViewMat(1.0), mProjMat(1.0),
xRight(vp->width() / 2.0), xLeft(-xRight),
yTop(vp->height() / 2.0), yBot(-yTop)
{
	setPM();
	mAng = 270;		//#INICIALIZACION DE NUEVAS VARIABLES 270 es el angulo de giro que mira al plano de frente
	mRadio = 2000;	//#1000 porque si lo pone
}
//-------------------------------------------------------------------------

void Camera::uploadVM() const
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(value_ptr(mViewMat)); // transfers view matrix to the GPU 
}
//-------------------------------------------------------------------------

void Camera::setVM()
{
	mViewMat = lookAt(mEye, mLook, mUp);  // glm::lookAt defines the view matrix 
	setAxes();							  //# Actualiza los ejes de la camara
}
//-------------------------------------------------------------------------

void Camera::set2D()
{
	mEye = dvec3(0, 0, mRadio);
	mLook = dvec3(0, 0, 0);
	mUp = dvec3(0, 1, 0);
	setVM();
}
//-------------------------------------------------------------------------

void Camera::setCenital()
{
	mEye = dvec3(0, mRadio, 0);
	mLook = dvec3(0, 0, 0);
	mUp = dvec3(1, 0, 0);
	setVM();
}
//-------------------------------------------------------------------------

void Camera::set3D()
{
	mEye = dvec3(500, 500, 500);
	mLook = dvec3(0, 10, 0);
	mUp = dvec3(0, 1, 0);
	setVM();
}
//-------------------------------------------------------------------------
//# Se supone que hay que comentar todo esto
//void Camera::pitch(GLdouble a) 
//{  
//	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(1.0, 0, 0));
//	// glm::rotate returns mViewMat * rotationMatrix
//}
////-------------------------------------------------------------------------
//
//void Camera::yaw(GLdouble a) 
//{
//	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(0, 1.0, 0));
//	// glm::rotate returns mViewMat * rotationMatrix
//}
////-------------------------------------------------------------------------
//
//void Camera::roll(GLdouble a) 
//{
//	mViewMat = rotate(mViewMat, glm::radians(a), glm::dvec3(0, 0, 1.0));
//	// glm::rotate returns mViewMat * rotationMatrix
//}
//-------------------------------------------------------------------------

void Camera::setSize(GLdouble xw, GLdouble yh)
{
	xRight = xw / 2.0;
	xLeft = -xRight;
	yTop = yh / 2.0;
	yBot = -yTop;
	setPM();
}
//-------------------------------------------------------------------------

void Camera::setScale(GLdouble s)
{
	mScaleFact -= s;
	if (mScaleFact < 0) mScaleFact = 0.01;
	setPM();
}
//-------------------------------------------------------------------------

void Camera::setPM()
{
	if (bOrto) { //  if orthogonal projection
		mProjMat = ortho(xLeft * mScaleFact, xRight * mScaleFact, yBot * mScaleFact, yTop * mScaleFact, mNearVal, mFarVal);
		// glm::ortho defines the orthogonal projection matrix
	}
	else {	//#Cambiar ortho por frustum si queremos una proyección con perspectiva
		mProjMat = frustum(xLeft * mScaleFact, xRight * mScaleFact, yBot * mScaleFact, yTop * mScaleFact, yTop * 2, mFarVal);	//yTop * 2 porque tag(30) = T/N => 0,5 = T/N => N = T*2
	}
}
//-------------------------------------------------------------------------

void Camera::uploadPM() const
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(value_ptr(mProjMat)); // transfers projection matrix to the GPU
	glMatrixMode(GL_MODELVIEW);
}
//-------------------------------------------------------------------------
//# Nuevos metodos
void Camera::setAxes() {
	mRight = row(mViewMat, 0);
	mUpward = row(mViewMat, 1);
	mFront = -row(mViewMat, 2);
}


//# Metodos ejercicio 21
void Camera::moveLR(GLdouble cs) {
	mEye += mRight * cs;
	mLook += mRight * cs;
	setVM();
}
void Camera::moveFB(GLdouble cs) {	//#Esto no se si es así, hago *= y funciona 😎👌 en verda no funciona uwu
	mEye += mFront * cs;
	mLook += mFront * cs;
	setVM();
}
void Camera::moveUD(GLdouble cs) {
	mEye += mUpward * cs;
	mLook += mUpward * cs;
	setVM();
}
//# Metodos ejercicio 22
void Camera::orbit(GLdouble incAng, GLdouble incY) {
	mAng += incAng;
	mEye.x = mLook.x + cos(radians(mAng)) * mRadio;
	mEye.z = mLook.z - sin(radians(mAng)) * mRadio;
	mEye.y += incY;
	setVM();
}
//# Metodo ejercicio 23
void Camera::changePrj() {
	bOrto = !bOrto;
	setPM();
}