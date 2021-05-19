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
	// se añaden comandos para la tabla de normales:

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
	mesh->mNumVertices = numL;						//Definimos el n� de vertices
	mesh->vVertices.reserve(mesh->mNumVertices);	//Y los reservamos
	//mesh->vColors.reserve(mesh->mNumVertices);		//Reservamos espacio para los colores	-> No hace falta definir 

	double ang = 90;	//El primer v�rtice siempre se genera con este angulo

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
	mesh->mNumVertices = numP;						//Definimos el n� de vertices
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
	//a�adimos vector de colores
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
	mesh->mNumVertices = 4;							//Definimos el n� de vertices
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
	mesh->mNumVertices = numL * 2 + 2;						//Definimos el n� de vertices
	mesh->vVertices.reserve(mesh->mNumVertices);	//Y los reservamos

	double x;
	double y;
	double z;
	double ang = 0;	//El primer v�rtice siempre se genera con este angulo
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
	mesh->mNumVertices = 10;							//Definimos el n� de vertices
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

	//los vertices para las texturas que avs como xd�?
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
	double ang = 0;	//El primer v�rtice siempre se genera con este angulo

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



	//Definimos el n� de vertices
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
//Clase Practica 2
void IndexMesh::render() const {
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
		if (vNormals.size() > 0) {
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_DOUBLE, 0, vNormals.data());
		}
		if (vIndices != nullptr) {
			glEnableClientState(GL_INDEX_ARRAY);
			glIndexPointer(GL_UNSIGNED_INT, 0, vIndices);
		}
		draw();

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_INDEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
	}
}
//-------------------------------------------------------------------------

// Comando para renderizar la malla indexada enviada
void IndexMesh::draw() const {
	glDrawElements(mPrimitive, nNumIndices, GL_UNSIGNED_INT, vIndices);
}

IndexMesh* IndexMesh::generaAnilloCuadradoIndexado() {
	//Asignamos vIndices para que genere la forma
	IndexMesh* anilloMesh = new IndexMesh();
	anilloMesh->nNumIndices = 10;
	anilloMesh->vIndices = new GLuint[anilloMesh->nNumIndices]{ 0, 1, 2, 3, 4, 5, 6, 7, 0, 1 };

	//Definimos los vertices del poligono
	anilloMesh->mNumVertices = 8;								//Definimos el numero de vertices
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

	anilloMesh->vNormals.reserve(anilloMesh->mNumVertices);
	for (int i = 0; i < anilloMesh->mNumVertices; i++) {
		anilloMesh->vNormals.emplace_back(0.0, 0.0, 1.0);
	}
	//Saco normales
	/*anilloMesh->vNormals.reserve(anilloMesh->mNumVertices);
	for (int i = 0; i < anilloMesh->mNumVertices - 2; i++) {
		GLuint* auxArr = new GLuint[anilloMesh->nNumIndices]{ (GLuint)i,(GLuint)i + 1,(GLuint)i + 2 };
		glm::dvec3 aux = anilloMesh->CalculoVectorNormalPorNewell(auxArr);
		anilloMesh->vNormals.emplace_back(aux);
	}*/

	//Saco normales como dice el motherfucker del Aaron que es un maquina
	//anilloMesh->sacaNormales();


	return anilloMesh;
}

//SACA NORMALES POR NEWELL
glm::dvec3 IndexMesh::CalculoVectorNormalPorNewell(GLuint* face) {
	dvec3 n = { 0, 0, 0 };
	dvec3 vertActual;
	dvec3 vertSiguiente;

	for (int i = 0; i < 2; i++) {
		vertActual = vVertices[face[i]];	//Accedemos al vertice que se corresponde a cada índice
		vertSiguiente = vVertices[face[i + 1]];

		n.x += (vertActual.y - vertSiguiente.y) *
			(vertActual.z + vertSiguiente.z);
		n.y += (vertActual.z - vertSiguiente.z) *
			(vertActual.x + vertSiguiente.x);
		n.z += (vertActual.x - vertSiguiente.x) *
			(vertActual.y + vertSiguiente.y);
	}

	return glm::normalize(n);
}

void IndexMesh::sacaNormales() {
	vNormals.reserve(mNumVertices);

	for (int i = 0; i < mNumVertices; i++)
		vNormals.emplace_back(0, 0, 0);

	for (int i = 0; i < nNumIndices; i = i + 3) {
		dvec3 a, b, c;
		a = vVertices[vIndices[i]];
		b = vVertices[vIndices[i + 1]];
		c = vVertices[vIndices[i + 2]];

		dvec3 n = cross((b - a), (c - a));

		vNormals[vIndices[i]] += n;
		vNormals[vIndices[i + 1]] += n;
		vNormals[vIndices[i + 2]] += n;
	}

	for (int i = 0; i < mNumVertices; i++)
		vNormals[i] = glm::normalize(vNormals[i]);
}

IndexMesh* IndexMesh::generaCuboConTapasIndexado(GLdouble l) {
	IndexMesh* cuboMesh = new IndexMesh();
	cuboMesh->mPrimitive = GL_TRIANGLES;

	//Indices
	cuboMesh->nNumIndices = 36;
	cuboMesh->vIndices = new GLuint[cuboMesh->nNumIndices]{ 0,1,2,2,1,3,2,3,4,4,3,5,4,5,6,6,5,7,6,7,1,0,6,1,  6,0,2,4,6,2,  1,7,5, 1,5,3 };			//Javi y aaron

	//Vertices
	cuboMesh->mNumVertices = 8;
	cuboMesh->vVertices.reserve(cuboMesh->mNumVertices);
	cuboMesh->vVertices.emplace_back(-l, l, l);
	cuboMesh->vVertices.emplace_back(-l, -l, l);
	cuboMesh->vVertices.emplace_back(l, l, l);
	cuboMesh->vVertices.emplace_back(l, -l, l);

	cuboMesh->vVertices.emplace_back(l, l, -l);
	cuboMesh->vVertices.emplace_back(l, -l, -l);
	cuboMesh->vVertices.emplace_back(-l, l, -l);
	cuboMesh->vVertices.emplace_back(-l, -l, -l);

	cuboMesh->vColors.reserve(cuboMesh->nNumIndices);
	cuboMesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	cuboMesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	cuboMesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	cuboMesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	cuboMesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	cuboMesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	cuboMesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	cuboMesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);

	/*cuboMesh->vNormals.reserve(cuboMesh->mNumVertices);
	for (int i = 0; i < cuboMesh->mNumVertices - 2; i++) {
		GLuint* auxArr = new GLuint[cuboMesh->nNumIndices]{ (GLuint)i,(GLuint)i + 1,(GLuint)i + 2 };
		glm::dvec3 aux = cuboMesh->CalculoVectorNormalPorNewell(auxArr);
		cuboMesh->vNormals.emplace_back(aux);
	}*/

	//Saco normales
	cuboMesh->sacaNormales();

	return cuboMesh;
}
//-------------------------------------------------------------------------
//Clase Practica 2.1
MbR::MbR(int mm, int nn, glm::dvec3* perfill) {
}

MbR* MbR::generaMallaIndexadaPorRevolucion(int mm, int nn, glm::dvec3* perfill) {
	MbR* mesh = new MbR(mm, nn, perfill);
	// Definir la primitiva como GL_TRIANGLES
	mesh->mPrimitive = GL_TRIANGLES;
	// Definir el número de vértices como nn*mm
	mesh->mNumVertices = nn * mm;
	mesh->vVertices.reserve(mesh->mNumVertices);
	//mesh->vColors.reserve(mesh->mNumVertices);

	// Usar un vector auxiliar de vértices
	glm::dvec3* vertices = new dvec3[mesh->mNumVertices];
	for (int i = 0; i < nn; i++) {
		// Generar la muestra i-ésima de vértices
		GLdouble theta = i * 360 / nn;
		GLdouble c = cos(radians(theta));
		GLdouble s = sin(radians(theta));
		// R_y(theta) es la matriz de rotación alrededor del eje Y
		for (int j = 0; j < mm; j++) {
			int indice = i * mm + j; // 
			GLdouble x = c * perfill[j].x + s * perfill[j].z;
			GLdouble z = -s * perfill[j].x + c * perfill[j].z;
			vertices[indice] = glm::dvec3(x, perfill[j].y, z);

			mesh->vVertices.emplace_back(vertices[indice].x, vertices[indice].y, vertices[indice].z);			//Vamos metiendo los vertices
			//mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
		}
	}


	//Determinar los indices de las caras cuadrangulares
	int indiceMayor = 0;
	mesh->nNumIndices = mesh->nNumIndices = nn * (mm - 1) * 6;
	mesh->vIndices = new GLuint[mesh->nNumIndices];
	// El contador i recorre las muestras alrededor del eje Y
	for (int i = 0; i < nn; i++) {
		// El contador j recorre los vértices del perfil,
		// de abajo arriba. Las caras cuadrangulares resultan
		// al unir la muestra i-ésima con la (i+1)%nn-ésima
		for (int j = 0; j < mm - 1; j++) {
			// El contador indice sirve para llevar cuenta
			// de los índices generados hasta ahora. Se recorre
			// la cara desde la esquina inferior izquierda
			int indice = i * mm + j;

			//Añadimos los indices
			//Primer triangulo
			mesh->vIndices[indiceMayor] = indice + 1;
			indiceMayor++;
			mesh->vIndices[indiceMayor] = indice;
			indiceMayor++;
			mesh->vIndices[indiceMayor] = (indice + mm + 1) % (nn * mm);
			indiceMayor++;
			//Segundo triangulo
			mesh->vIndices[indiceMayor] = indice;
			indiceMayor++;
			mesh->vIndices[indiceMayor] = (indice + mm) % (nn * mm);
			indiceMayor++;
			mesh->vIndices[indiceMayor] = (indice + mm + 1) % (nn * mm);
			indiceMayor++;
		}
	}
	// Los cuatro índices son entonces:
	//indice, (indice + mm) % (nn * mm), (indice + mm + 1) % (nn * mm), indice + 1



	//Sacamos las normales y returneamos la mesh
	mesh->sacaNormales();
	return mesh;
}

Grid::Grid(GLuint lado, int divisiones)
{
	lado_ = lado;
	divisiones_ = divisiones;
}

Grid* Grid::generateGrid(GLdouble lado, GLuint nDiv)
{
	Grid* mesh = new Grid(lado, nDiv);

	//primitiva
	mesh->mPrimitive = GL_TRIANGLES;

	GLdouble incr = lado / nDiv; // incremento para la coordenada x, y la c. z
	GLuint numFC = nDiv + 1; // número de vértices por filas y columnas

	// Generación de vértices
	mesh->mNumVertices = numFC * numFC;
	//mesh->vertices = new dvec3[mesh->mNumVertices];
	mesh->vVertices.reserve(mesh->mNumVertices);

	GLdouble z = -lado / 2,
		x = -lado / 2;
	for (int f = 0; f < numFC; f++) {
		for (int c = 0; c < numFC; c++) {
			mesh->vVertices.emplace_back(x + c * incr,
				0,
				z + f * incr);

			/*mesh->vVertices[f * numFC + c] = dvec3(x + c * incr,
													0,
													z + f * incr);*/
		}
	}

	// Generación de índices
	mesh->nNumIndices = nDiv * nDiv * 6;
	mesh->vIndices = new GLuint[mesh->nNumIndices];


	int i = 0; // array de índices

	for (int f = 0; f < nDiv; f++) {
		for (int c = 0; c < nDiv; c++) {
			GLuint iv = f * numFC + c;
			mesh->vIndices[i++] = iv;
			mesh->vIndices[i++] = iv + numFC;
			mesh->vIndices[i++] = iv + 1;

			mesh->vIndices[i++] = iv + 1;
			mesh->vIndices[i++] = iv + numFC;
			mesh->vIndices[i++] = iv + numFC + 1;
		}
	}

	//Sacamos las normales y returneamos la mesh
	mesh->sacaNormales();
	return mesh;
}
