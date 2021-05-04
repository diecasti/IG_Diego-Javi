#include "IndexMesh.h"
#include "CheckML.h"
#include <fstream>
using namespace std;
using namespace glm;

//-------------------------------------------------------------------------
void IndexMesh::render() const {
	//… // Comandos OpenGL para enviar datos de arrays a GPU
	// Nuevos comandos para la tabla de índices
		if (vIndices != nullptr) {
			glEnableClientState(GL_INDEX_ARRAY);
			glIndexPointer(GL_UNSIGNED_INT, 0, indices);
		}
	//… // Comandos OpenGL para deshabilitar datos enviados
	// Nuevo comando para la tabla de índices:
		glDisableClientState(GL_INDEX_ARRAY);
}
//-------------------------------------------------------------------------

// Comando para renderizar la malla indexada enviada
void IndexMesh::draw() const {
	glDrawElements(mPrimitive, nNumIndices,
		GL_UNSIGNED_INT, vIndices);
}