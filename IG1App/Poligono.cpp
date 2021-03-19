#include "Poligono.h"

void Poligono::render(glm::dmat4 const& modelViewMat)const {
	if (mMesh != nullptr) {
		glm::dmat4 aMat = modelViewMat * mModelMat;  // glm matrix multiplication
		upload(aMat);
		glLineWidth(2);
		glColor3d(mColor.r, mColor.g, mColor.b);
		mMesh->render();
		glLineWidth(1);
		glColor3d(1, 1, 1);
	}
}