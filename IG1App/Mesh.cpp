#include "Mesh.h"
#include "CheckML.h"
#include <fstream>
using namespace std;
using namespace glm;

//-------------------------------------------------------------------------

void Mesh::draw() const
{
	glDrawArrays(mPrimitive, 0, size());   // primitive graphic, first index and number of elements to be rendered
}
//-------------------------------------------------------------------------

void Mesh::render() const
{
	if (vVertices.size() > 0) {  // transfer data
	  // transfer the coordinates of the vertices
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_DOUBLE, 0, vVertices.data());  // number of coordinates per vertex, type of each coordinate, stride, pointer 
		if (vColors.size() > 0) { // transfer colors
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(4, GL_DOUBLE, 0, vColors.data());  // components number (rgba=4), type of each component, stride, pointer  
		}
		if (vTexCoords.size() > 0) {
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_DOUBLE, 0, vTexCoords.data());
		}
		draw();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}
//-------------------------------------------------------------------------

Mesh* Mesh::createRGBAxes(GLdouble l)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_LINES;

	mesh->mNumVertices = 6;
	mesh->vVertices.reserve(mesh->mNumVertices);

	// X axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(l, 0.0, 0.0);
	// Y axis vertices
	mesh->vVertices.emplace_back(0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, l, 0.0);
	// Z axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, 0.0, l);

	mesh->vColors.reserve(mesh->mNumVertices);
	// X axis color: red  (Alpha = 1 : fully opaque)
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	// Y axis color: green
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	// Z axis color: blue
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}
//-------------------------------------------------------------------------
Mesh* Mesh::generaPoligono(GLuint numL, GLdouble rd) {
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_LINE_LOOP;
	mesh->mNumVertices = numL;						//Definimos el nº de vertices
	mesh->vVertices.reserve(mesh->mNumVertices);	//Y los reservamos
	//mesh->vColors.reserve(mesh->mNumVertices);		//Reservamos espacio para los colores	-> No hace falta definir 

	double ang = 90;	//El primer vértice siempre se genera con este angulo

	for (int i = 0; i < numL; i++) {
		double x = 0 + rd * cos(radians(ang));	//x = Cx + rd*cos(ang) Cx es 0 porque la circunferencia tiene centro en (0,0)
		double y = 0 + rd * sin(radians(ang));	//No podemos olvidar pasar de grados a radianes	
		//Creamos los vertices y asignamos los colores
		mesh->vVertices.emplace_back(x, y, 0.0);
		//mesh->vColors.emplace_back(0.0, 0.0, 0.0, 1.0);
		//Actualizamos el angulo
		ang += 360.0 / numL;
	}

	return mesh;
}
//-------------------------------------------------------------------------
Mesh* Mesh::generaSierpinski(GLdouble rd, GLuint numP) {
	Mesh* triangulo = generaPoligono(3, rd);
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_POINTS;
	mesh->mNumVertices = numP;						//Definimos el nº de vertices
	mesh->vVertices.reserve(mesh->mNumVertices);	//Y los reservamos

	//Crea la malla de Sierpinski
		//Selecionamos 2 puntos diferentes del triangulo inicial
	glm::dvec3 vAux = triangulo->vVertices[rand() % 3];
	glm::dvec3 Pk = triangulo->vVertices[rand() % 3];
	while (Pk == vAux) {
		Pk = triangulo->vVertices[rand() % 3];
	}
	//Creamos el punto medio entre esos dos puntos y actualizamos los puntos
	for (int i = 0; i < numP; i++) {
		glm::dvec3 Pm = (vAux + Pk) / 2.0;
		mesh->vVertices.emplace_back(Pm.x, Pm.y, Pm.z);

		Pk = triangulo->vVertices[rand() % 3];
		vAux = Pm;
	}

	delete triangulo;
	triangulo = nullptr;
	return mesh;
}
//-------------------------------------------------------------------------
Mesh* Mesh::generaTrianguloRGB(GLdouble rd) {
	Mesh* mesh = generaPoligono(3.0, rd);
	mesh->mPrimitive = GL_TRIANGLES;
	//añadimos vector de colores
	mesh->vColors.reserve(mesh->mNumVertices);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}
//-------------------------------------------------------------------------
Mesh* Mesh::generaRectangulo(GLdouble w, GLdouble h) {
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 4;							//Definimos el nº de vertices
	mesh->vVertices.reserve(mesh->mNumVertices);	//Y los reservamos
	mesh->vVertices.emplace_back(-w / 2, h / 2, 0);
	mesh->vVertices.emplace_back(-w / 2, -h / 2, 0);
	mesh->vVertices.emplace_back(w / 2, h / 2, 0);
	mesh->vVertices.emplace_back(w / 2, -h / 2, 0);
	return mesh;
}
Mesh* Mesh::generaRectanguloRGB(GLdouble w, GLdouble h) {
	Mesh* a = generaRectangulo(w, h);
	a->vColors.reserve(a->mNumVertices);
	a->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	a->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	a->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	a->vColors.emplace_back(1.0, 0.0, 1.0, 1.0);
	return a;
}

//-------------------------------------------------------------------------
Mesh* Mesh::generaEstrella3D(GLdouble re, GLuint numL, GLdouble h) {
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLE_FAN;
	mesh->mNumVertices = numL * 2 + 2;						//Definimos el nº de vertices
	mesh->vVertices.reserve(mesh->mNumVertices);	//Y los reservamos

	double x;
	double y;
	double z;
	double ang = 0;	//El primer vértice siempre se genera con este angulo
	mesh->vVertices.emplace_back(0, 0, 0.0);

	for (int i = 0; i < numL * 2 + 1; i++) {
		if (i % 2 == 0) {
			x = 0 + re * cos(radians(ang));	//x = Cx + rd*cos(ang) Cx es 0 porque la circunferencia tiene centro en (0,0)
			y = 0 + re * sin(radians(ang));	//No podemos olvidar pasar de grados a radianes
			z = 250;
		}
		else {
			x = 0 + (re / 2) * cos(radians(ang));	//x = Cx + rd*cos(ang) Cx es 0 porque la circunferencia tiene centro en (0,0)
			y = 0 + (re / 2) * sin(radians(ang));	//No podemos olvidar pasar de grados a radianes
			z = 250;
		}
		//Creamos los vertices y asignamos los colores
		mesh->vVertices.emplace_back(x, y, z);
		//mesh->vColors.emplace_back(0.0, 0.0, 0.0, 1.0);
		//Actualizamos el angulo
		ang += 360.0 / (numL * 2);

	}
	return mesh;
}
Mesh* Mesh::generaContCubo(GLdouble ld) {
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 10;							//Definimos el nº de vertices
	mesh->vVertices.reserve(mesh->mNumVertices);	//Y los reservamos
	mesh->vVertices.emplace_back(-ld / 2, ld / 2, ld / 2);
	mesh->vVertices.emplace_back(-ld / 2, -ld / 2, ld / 2);
	mesh->vVertices.emplace_back(ld / 2, ld / 2, ld / 2);
	mesh->vVertices.emplace_back(ld / 2, -ld / 2, ld / 2);
	//esto genera la primera cara, ahora la de la lado serian
	mesh->vVertices.emplace_back(ld / 2, ld / 2, -ld / 2);
	mesh->vVertices.emplace_back(ld / 2, -ld / 2, -ld / 2);
	//la siguiente cara igual al contrario
	mesh->vVertices.emplace_back(-ld / 2, ld / 2, -ld / 2);
	mesh->vVertices.emplace_back(-ld / 2, -ld / 2, -ld / 2);
	//finalmente los mismos dos ultimos puntos que son los mismos que el principio
	mesh->vVertices.emplace_back(-ld / 2, ld / 2, ld / 2);
	mesh->vVertices.emplace_back(-ld / 2, -ld / 2, ld / 2);
	//cubo cerrado
	return mesh;
}
Mesh* Mesh::generaRectanguloTexCor(GLdouble w, GLdouble h, GLuint rw, GLuint rh) {
	Mesh* mesh = generaRectangulo(w, h);

	//los vertices para las texturas que avs como xd¿?
	mesh->vTexCoords.reserve(mesh->mNumVertices);
	mesh->vTexCoords.emplace_back(0, rh);
	mesh->vTexCoords.emplace_back(0, 0);
	mesh->vTexCoords.emplace_back(rw, rh);
	mesh->vTexCoords.emplace_back(rw, 0);


	return mesh;
 }

