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
<<<<<<< HEAD
	//.... // se añaden comandos para la tabla de normales:
=======
	// se aÃ±aden comandos para la tabla de normales:

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (vColors.size() > 0) { // transfer colors
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_DOUBLE, 0, vColors.data());  // components number (rgba=4), type of each component, stride, pointer  
	}
	if (vTexCoords.size() > 0) {
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_DOUBLE, 0, vTexCoords.data());
	}
>>>>>>> d097980 (done)
	if (vNormals.size() > 0) {
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_DOUBLE, 0, vNormals.data());
	}

	draw();

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	if (vVertices.size() > 0) {  // transfer data
	  // transfer the coordinates of the vertices
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_DOUBLE, 0, vVertices.data());  // number of coordinates per vertex, type of each coordinate, stride, pointer 

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
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
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);

	for (int i = 0; i < numL * 2 + 1; i++) {
		if (i % 2 == 0) {
			x = 0 + re * cos(radians(ang));	//x = Cx + rd*cos(ang) Cx es 0 porque la circunferencia tiene centro en (0,0)
			y = 0 + re * sin(radians(ang));	//No podemos olvidar pasar de grados a radianes
			z = re;
		}
		else {
			x = 0 + (re / 2) * cos(radians(ang));	//x = Cx + rd*cos(ang) Cx es 0 porque la circunferencia tiene centro en (0,0)
			y = 0 + (re / 2) * sin(radians(ang));	//No podemos olvidar pasar de grados a radianes
			z = re;
		}
		//Creamos los vertices y asignamos los colores
		mesh->vVertices.emplace_back(x, y, z);
		//mesh->vColors.emplace_back(0.0, 0.0, 0.0, 1.0);
		//Actualizamos el angulo
		ang += 360.0 / (numL * 2.0);

	}
	return mesh;
}
Mesh* Mesh::generaContCubo(GLdouble ld) {
	Mesh* mesh = new Mesh();
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->mNumVertices = 10;							//Definimos el nº de vertices
	mesh->vVertices.reserve(mesh->mNumVertices);	//Y los reservamos
	mesh->vVertices.emplace_back(-ld / 2, -ld / 2, ld / 2);
	mesh->vVertices.emplace_back(-ld / 2, ld / 2, ld / 2);
	mesh->vVertices.emplace_back(ld / 2, -ld / 2, ld / 2);
	mesh->vVertices.emplace_back(ld / 2, ld / 2, ld / 2);
	//esto genera la primera cara, ahora la de la lado serian
	mesh->vVertices.emplace_back(ld / 2, -ld / 2, -ld / 2);
	mesh->vVertices.emplace_back(ld / 2, ld / 2, -ld / 2);
	//la siguiente cara igual al contrario
	mesh->vVertices.emplace_back(-ld / 2, -ld / 2, -ld / 2);
	mesh->vVertices.emplace_back(-ld / 2, ld / 2, -ld / 2);
	//finalmente los mismos dos ultimos puntos que son los mismos que el principio
	mesh->vVertices.emplace_back(-ld / 2, -ld / 2, ld / 2);
	mesh->vVertices.emplace_back(-ld / 2, ld / 2, ld / 2);
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
Mesh* Mesh::generaEstrellaTexCor(GLdouble re, GLuint np, GLdouble h) {
	Mesh* mesh = generaEstrella3D(re, np, h);

	//los vertices para la textura

	mesh->vTexCoords.reserve(mesh->mNumVertices);

	//el de centro
	mesh->vTexCoords.emplace_back(0.5, 0.5);


	double x;
	double y;
	double ang = 0;	//El primer vértice siempre se genera con este angulo

	for (int i = 0; i < np * 2 + 1; i++) {
		x = 0.5 + 0.5 * cos(radians(ang));	//x = Cx + rd*cos(ang) Cx es 0 porque la circunferencia tiene centro en (0,0)
		y = 0.5 + 0.5 * sin(radians(ang));	//No podemos olvidar pasar de grados a radianes
	//Creamos los vertices y asignamos los colores
		mesh->vTexCoords.emplace_back(y, x);

		//Actualizamos el angulo
		ang += 360.0 / (np * 2.0);
	}
	return mesh;
}

Mesh* Mesh::generaContCuboTexCor(GLdouble nl) {
	Mesh* mesh = generaContCubo(nl);



	//Definimos el nº de vertices
	mesh->vTexCoords.reserve(mesh->mNumVertices);	//Y los reservamos
	mesh->vTexCoords.emplace_back(0, 0);
	mesh->vTexCoords.emplace_back(1, 0);
	mesh->vTexCoords.emplace_back(0, 1);
	mesh->vTexCoords.emplace_back(1, 1);
	//OTRAS CARAS
	mesh->vTexCoords.emplace_back(0, 0);
	mesh->vTexCoords.emplace_back(1, 0);
	mesh->vTexCoords.emplace_back(0, 1);
	mesh->vTexCoords.emplace_back(1, 1);
	//
	mesh->vTexCoords.emplace_back(0, 0);
	mesh->vTexCoords.emplace_back(1, 0);
	return mesh;
}

//-------------------------------------------------------------------------
<<<<<<< HEAD
//Clase Practica 2
void IndexMesh::render() const {
	Mesh::render();
=======
void IndexMesh::render() const {
	//… // Comandos OpenGL para enviar datos de arrays a GPU
>>>>>>> 58d69b5 (antonio te corot los webos)
	// Nuevos comandos para la tabla de índices
	if (vIndices != nullptr) {
		glEnableClientState(GL_INDEX_ARRAY);
		glIndexPointer(GL_UNSIGNED_INT, 0, vIndices);
	}
<<<<<<< HEAD
	//… // Comandos OpenGL para deshabilitar datos enviados
	// Nuevo comando para la tabla de índices:
=======
	// Comandos OpenGL para deshabilitar datos enviados
	// Nuevo comando para la tabla de ï¿½ndices:
>>>>>>> d097980 (done)
	glDisableClientState(GL_INDEX_ARRAY);
<<<<<<< HEAD

=======
>>>>>>> 58d69b5 (antonio te corot los webos)
}
//-------------------------------------------------------------------------

// Comando para renderizar la malla indexada enviada
void IndexMesh::draw() const {
<<<<<<< HEAD
	glDrawElements(mPrimitive, nNumIndices, GL_UNSIGNED_INT, vIndices);
}

IndexMesh* IndexMesh::generaAnilloCuadradoIndexado() {
	//Asignamos vIndices para que genere la forma
	IndexMesh* anilloMesh = new IndexMesh();
	anilloMesh->nNumIndices = 10;
	anilloMesh->vIndices = new GLuint[anilloMesh->nNumIndices]{ 0, 1, 2, 3, 4, 5, 6, 7, 0, 1 };

	//Definimos los vertices del poligono
	anilloMesh->mNumVertices = 8;								//Definimos el nº de vertices
	anilloMesh->vVertices.reserve(anilloMesh->mNumVertices);	//Y los reservamos
	anilloMesh->vVertices.emplace_back(-50, -50, 0);
	anilloMesh->vVertices.emplace_back(-100, -100, 0);
	anilloMesh->vVertices.emplace_back(50, -50, 0);
	anilloMesh->vVertices.emplace_back(100, -100, 0);
	anilloMesh->vVertices.emplace_back(50, 50, 0);
	anilloMesh->vVertices.emplace_back(100, 100, 0);
	anilloMesh->vVertices.emplace_back(-50, 50, 0);
	anilloMesh->vVertices.emplace_back(-100, 100, 0);

	anilloMesh->vColors.reserve(anilloMesh->mNumVertices);
	anilloMesh->vColors.emplace_back(0.0, 0.0, 0.0, 1.0);
	anilloMesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	anilloMesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	anilloMesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	anilloMesh->vColors.emplace_back(1.0, 1.0, 0.0, 1.0);
	anilloMesh->vColors.emplace_back(1.0, 0.0, 1.0, 1.0);
	anilloMesh->vColors.emplace_back(0.0, 1.0, 1.0, 1.0);
	anilloMesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);

	return anilloMesh;
<<<<<<< HEAD
=======
	glDrawElements(mPrimitive, nNumIndices,
		GL_UNSIGNED_INT, vIndices);
>>>>>>> 58d69b5 (antonio te corot los webos)
}
=======
}

//glm::dvec3* Mesh::devuelveNormal(Mesh* m) {
//	glm::dvec3 normal;
//	m->vVertices.
//	return normal;
//}
>>>>>>> d097980 (done)
