#include "IG1App.h"
#include "CheckML.h"
#include <iostream>

using namespace std;

//-------------------------------------------------------------------------
// static single instance (singleton pattern)

IG1App IG1App::s_ig1app;  // default constructor (constructor with no parameters)

//-------------------------------------------------------------------------

void IG1App::close()
{
	if (!mStop) {  // if main loop has not stopped
		cout << "Closing glut...\n";
		glutLeaveMainLoop();  // stops main loop and destroy the OpenGL context
		mStop = true;   // main loop stopped  
	}
	free();
}
//-------------------------------------------------------------------------

void IG1App::run()   // enters the main event processing loop
{
	if (mWinId == 0) { // if not initialized
		init();       // initialize the application 
		glutMainLoop();      // enters the main event processing loop 
		mStop = true;  // main loop has stopped  
	}
}
//-------------------------------------------------------------------------

void IG1App::init()
{
	// create an OpenGL Context
	iniWinOpenGL();

	// create the scene after creating the context 
	// allocate memory and resources
	mViewPort = new Viewport(mWinW, mWinH); //glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT)
	mCamera = new Camera(mViewPort);
	mScene = new Scene(0);

	m2Vistas = false;

	mCamera->set2D();
	mScene->init();
}
//-------------------------------------------------------------------------

void IG1App::iniWinOpenGL()
{  // Initialization
	cout << "Starting glut...\n";
	int argc = 0;
	glutInit(&argc, nullptr);

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);  // GLUT_CORE_PROFILE
	glutInitContextFlags(GLUT_DEBUG);		// GLUT_FORWARD_COMPATIBLE

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	glutInitWindowSize(mWinW, mWinH);   // window size
	//glutInitWindowPosition (140, 140);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH /*| GLUT_STENCIL*/); // RGBA colors, double buffer, depth buffer and stencil buffer   

	mWinId = glutCreateWindow("IG1App");  // with its associated OpenGL context, return window's identifier 

	// Callback registration
	glutReshapeFunc(s_resize);
	glutKeyboardFunc(s_key);
	glutSpecialFunc(s_specialKey);
	glutDisplayFunc(s_display);
	glutIdleFunc(s_update);
	glutMouseFunc(s_mouse);
	glutMotionFunc(s_motion);
	glutMouseWheelFunc(s_mouseWheel);

	cout << glGetString(GL_VERSION) << '\n';
	cout << glGetString(GL_VENDOR) << '\n';
}
//-------------------------------------------------------------------------

void IG1App::free()
{  // release memory and resources
	delete mScene; mScene = nullptr;
	delete mCamera; mCamera = nullptr;
	delete mViewPort; mViewPort = nullptr;
}
//-------------------------------------------------------------------------

void IG1App::display() const
{  // double buffering

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clears the back buffer

	if (!m2Vistas)
		mScene->render(*mCamera);  // uploads the viewport and camera to the GPU
	else
		display2Vistas();

	glutSwapBuffers();	// swaps the front and back buffer
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------

void IG1App::display2Vistas() const
{  // double buffering
// para renderizar las vistas utilizamos una cámara auxiliar:
	Camera auxCam = *mCamera; // copiando mCamera
	// el puerto de vista queda compartido (se copia el puntero)
	Viewport auxVP = *mViewPort; // lo copiamos en una var. aux. para
	// el tamaño de los 2 puertos de vista es el mismo, lo configuramos
	mViewPort->setSize(mWinW / 2, mWinH);
	// igual que en resize, para que no cambie la escala,
	// tenemos que cambiar el tamaño de la ventana de vista de la cámara
	auxCam.setSize(mViewPort->width(), mViewPort->height());
	// vista izquierda
	mViewPort->setPos(0, 0);
	mScene->render(auxCam);
	//vista derecha
	mViewPort->setPos(mWinW / 2, 0);
	auxCam.setCenital();
	mScene->render(auxCam);

	*mViewPort = auxVP; // restaurar el puerto de vista 


}
//-------------------------------------------------------------------------

void IG1App::resize(int newWidth, int newHeight)
{
	mWinW = newWidth; mWinH = newHeight;

	// Resize Viewport to the new window size
	mViewPort->setSize(newWidth, newHeight);

	// Resize Scene Visible Area such that the scale is not modified
	mCamera->setSize(mViewPort->width(), mViewPort->height());
}
//-------------------------------------------------------------------------

void IG1App::key(unsigned char key, int x, int y)
{
	bool need_redisplay = true;

	switch (key) {
	case 27:  // Escape key 
		glutLeaveMainLoop();  // stops main loop and destroy the OpenGL context
	case '+':
		mCamera->setScale(+0.01);  // zoom in  (increases the scale)
		break;
	case '-':
		mCamera->setScale(-0.01);  // zoom out (decreases the scale)
		break;
	case 'l':
		mCamera->set3D();
		break;
	case 'o':
		mCamera->set2D();
		break;
	case 'u':
		if (animar)
			animar = false;
		else
			animar = true;
		break;
		//Controles camara
	case 'd':
		mCamera->moveLR(1.5);
		break;
	case 'a':
		mCamera->moveLR(-1.5);
		break;
	case 'e':
		mCamera->moveFB(1.5);
		break;
	case 'q':
		mCamera->moveFB(-1.5);
		break;
	case 'w':
		mCamera->moveUD(1);
		break;
	case 's':
		mCamera->moveUD(-1);
		break;
	case 'r':
		mCamera->orbit(5, 0);	//Angulo y altura por iteracion
		break;
	case 'p':
		mCamera->changePrj();	//Angulo y altura por iteracion
		break;
	case 'k':
		//cambair al modo dos camaras
		m2Vistas = !m2Vistas;
		break;
	default:
		mScene->changeScene(key - '0');
		break;
	}

	if (need_redisplay)
		glutPostRedisplay(); // marks the window as needing to be redisplayed -> calls to display()
}
//-------------------------------------------------------------------------

void IG1App::specialKey(int key, int x, int y)
{
	bool need_redisplay = true;
	int mdf = glutGetModifiers(); // returns the modifiers (Shift, Ctrl, Alt)

	//switch (key) {
	//case GLUT_KEY_RIGHT:
	//	if (mdf == GLUT_ACTIVE_CTRL)
	//		mCamera->moveLR(10);
	//		//mCamera->pitch(-1);   // rotates -1 on the X axis
	//	else
	//		mCamera->moveLR(-10);
	//		//mCamera->pitch(1);    // rotates 1 on the X axis
	//	break;
	//case GLUT_KEY_LEFT:
	//	if (mdf == GLUT_ACTIVE_CTRL)
	//		mCamera->moveFB(10);
	//		//mCamera->yaw(1);      // rotates 1 on the Y axis 
	//	else
	//		mCamera->moveFB(-10);
	//		//mCamera->yaw(-1);     // rotate -1 on the Y axis 
	//	break;
	//case GLUT_KEY_UP:
	//	//mCamera->roll(1);    // rotates 1 on the Z axis
	//	mCamera->moveUD(10);
	//	break;
	//case GLUT_KEY_DOWN:
	//	//mCamera->roll(-1);   // rotates -1 on the Z axis
	//	mCamera->moveUD(-10);
	//	break;
	//default:
	//	need_redisplay = false;
	//	break;
	//}//switch

	if (need_redisplay)
		glutPostRedisplay(); // marks the window as needing to be redisplayed -> calls to display()
}
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void IG1App::mouse(int button, int state, int x, int y) { //cosas del raton

	 //mientras esta pulsado //POR ALGUNA RAZON ENTRA DOS VECES
		mBot = button;
		mCoord.x = x;
		mCoord.y = glutGet(GLUT_WINDOW_HEIGHT) - y;


}
//-------------------------------------------------------------------------
void IG1App::motion(int x, int y) { //motion raton, cunado haces click y mueves

	/*
		1. Guarda en una variable auxiliar mp la diferencia entre mCoord y
		(x, y)
		2. Guarda en mCoord la posición (x, y) del ratón
		3. Si mBot es el botón izquierdo, la cámara orbita (mp.x*0.05,
		mp.y)
		4. Si mBot es el botón derecho, la cámara se desplaza moveUD()
		y moveLR() según indique mp
		5. glutPostRedisplay();
	*/

	glm::dvec2 mp(mCoord.x - x, mCoord.y - y);
	mCoord.x = x;
	mCoord.y = y;
	//GLUT_LEFT_BUTTON/GLUT_RIGHT_BUTTON
	if (mBot == GLUT_LEFT_BUTTON) {
		//cosas del 3
		mCamera->orbit(mp.x * 0.05, mp.y);
	}
	if (mBot == GLUT_RIGHT_BUTTON) {
		//cosas del 4
		mCamera->moveUD(-mp.y); //por alguna razon la diferencia me sale en nagativa asiq ue asi que va
		mCamera->moveLR(mp.x);
	}
	//cosas del 5 ya esta hecho
	glutPostRedisplay();

}
//-------------------------------------------------------------------------
void IG1App::mouseWheel(int wheelButtonNumber, int direction, int x, int y) { //la rueda del raton
	/*
		1. Averigua el número de teclas pulsadas con glutGetModifiers()
		2. Si no hay ninguna, la cámara se mueve con moveFB(), según el
		valor de d
		3. Si está pulsada la tecla Ctrl (GLUT_ACTIVE_CTRL), la cámara
		cambia la escala con setScale(), según el valor de d
		4. glutPostRedisplay();
	*/

	int m = glutGetModifiers();

	if (m == 0) {
		mCamera->moveFB(direction);
		
	}
	else if (m == GLUT_ACTIVE_CTRL) {
		mCamera->setScale(direction);
	}
	glutPostRedisplay();
}
//-------------------------------------------------------------------------

void IG1App::update() {

	if (animar) {
		if (glutGet(GLUT_ELAPSED_TIME) - mLastUpdateTime > 1000 / 60) {
			mScene->update();
			mLastUpdateTime = glutGet(GLUT_ELAPSED_TIME);
			glutPostRedisplay();
		}
	}
}