//#pragma once
#ifndef _H_Mesh_H_
#define _H_Mesh_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include <vector>

//-------------------------------------------------------------------------

class Mesh
{
public:

	static Mesh* createRGBAxes(GLdouble l); // creates a new 3D-RGB axes mesh

	Mesh() {};
	virtual ~Mesh() {};

	Mesh(const Mesh& m) = delete;  // no copy constructor
	Mesh& operator=(const Mesh& m) = delete;  // no copy assignment

	virtual void render() const;

	GLuint size() const { return mNumVertices; };   // number of elements
	std::vector<glm::dvec3> const& vertices() const { return vVertices; };
	std::vector<glm::dvec4> const& colors() const { return vColors; };

	static 	Mesh* generaPoligono(GLuint numL, GLdouble rd);
	static Mesh* generaSierpinski(GLdouble rd, GLuint numP);
	static Mesh* generaTrianguloRGB(GLdouble rd);
	static Mesh* generaRectangulo(GLdouble w, GLdouble h);
	static Mesh* generaRectanguloRGB(GLdouble w, GLdouble h);
	static Mesh* generaEstrella3D(GLdouble re, GLuint numL, GLdouble h);
	static Mesh* generaContCubo(GLdouble ld);
	static Mesh* generaRectanguloTexCor(GLdouble w, GLdouble h, GLuint rw, GLuint rh);
	static Mesh* generaEstrellaTexCor(GLdouble re, GLuint np, GLdouble h);

	static Mesh* generaContCuboTexCor(GLdouble nl);
protected:

	GLuint mPrimitive = GL_TRIANGLES;   // graphic primitive: GL_POINTS, GL_LINES, GL_TRIANGLES, ...
	GLuint mNumVertices = 0;			// number of elements ( = vVertices.size())
	std::vector<glm::dvec3> vVertices;  // vertex array
	std::vector<glm::dvec4> vColors;    // color array
	std::vector<glm::dvec2> vTexCoords; // TEXTURA GOES BRR BRR
	std::vector<glm::dvec3> vNormals;	// tabla de normales
	virtual void draw() const;

};
//-------------------------------------------------------------------------
class IndexMesh : public Mesh {
public:
	IndexMesh():Mesh() { mPrimitive = GL_TRIANGLE_STRIP; }
	~IndexMesh() { delete[] vIndices; }
	virtual void render() const;
	virtual void draw() const;

	static IndexMesh* generaAnilloCuadradoIndexado();
	static IndexMesh* generaCuboConTapasIndexado(GLdouble l);
	void sacaNormales();

	glm::dvec3 CalculoVectorNormalPorNewell(GLuint* face);
protected:
	GLuint* vIndices = nullptr; // tabla de índices
	GLuint nNumIndices = 0;
};

#endif //_H_Scene_H_
#ifndef _H_INDEXMESH_H_
#define _H_INDEXMESH_H_


//-------------------------------------------------------------------------
//Normales
/*
debe estar normalizado (módilo 1)
- Ojo con el comando glEnable(GL_NORMALIZE);

El comando glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
colorea caras traseras
*/
#endif