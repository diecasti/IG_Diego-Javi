//#pragma once
#ifndef _H_INDEXMESH_H_
#define _H_INDEXMESH_H_

#include <GL/freeglut.h>
#include <glm.hpp>

#include <vector>

#include "Mesh.h"

//-------------------------------------------------------------------------

class IndexMesh : public Mesh {
protected:
	GLuint* vIndices = nullptr; // tabla de índices
	GLuint nNumIndices = 0;
	//…
public:
	IndexMesh() { mPrimitive = GL_TRIANGLES; }
	~IndexMesh() { delete[] indices; }
	virtual void render() const;
	virtual void draw() const;
	//…
private:
	//puntero a GLUINT
	GLvoid* indices;
	//std::vector<glm::dvec3> indices; // tabla de normales
};

#endif