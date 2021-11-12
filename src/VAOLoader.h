#ifndef VAOLOADER_H_
#define VAOLOADER_H_

#include <glad/glad.h>
#include <vector>

#include "VAO.h"

class VAOLoader
{
public:
	static std::vector<GLfloat> getVerts(int id);
	static std::vector<GLfloat> getUVs(int id);
	static VAO * loadToVAO(GLfloat* verts, GLfloat* normals, GLfloat*uvs, GLuint* indices, int v, int u, int i);
};

#endif
