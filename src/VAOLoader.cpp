#include "VAOLoader.h"

#include <iostream>
#include <iterator>

static GLfloat g_vertex_buffer_data[] =
{
    .0f,.0f,.0f, // triangle 1 : begin
    .0f,.0f, 1.0f,
    .0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,.0f, // triangle 2 : begin
    .0f,.0f,.0f,
    .0f, 1.0f,.0f, // triangle 2 : end
    1.0f,.0f, 1.0f,
    .0f,.0f,.0f,
    1.0f,.0f,.0f,
    1.0f, 1.0f,.0f,
    1.0f,.0f,.0f,
    .0f,.0f,.0f,
    .0f,.0f,.0f,
    .0f, 1.0f, 1.0f,
    .0f, 1.0f,.0f,
    1.0f,.0f, 1.0f,
    .0f,.0f, 1.0f,
    .0f,.0f,.0f,
    .0f, 1.0f, 1.0f,
    .0f,.0f, 1.0f,
    1.0f,.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,.0f,.0f,
    1.0f, 1.0f,.0f,
    1.0f,.0f,.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,.0f,
    .0f, 1.0f,.0f,
    1.0f, 1.0f, 1.0f,
    .0f, 1.0f,.0f,
    .0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    .0f, 1.0f, 1.0f,
    1.0f,.0f, 1.0f
};

static GLfloat g_uv_buffer_data[] =
{
	.0f,.0f, // triangle 1 : begin
	.0f, 1.0f,
	 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f, // triangle 2 : begin
	.0f,.0f,
	.0f, 1.0f, // triangle 2 : end
	1.0f, 1.0f,
	.0f,.0f,
	1.0f,.0f,
	1.0f, 1.0f,
	1.0f,.0f,
	.0f,.0f,
	.0f,.0f,
	 1.0f, 1.0f,
	 1.0f,.0f,
	1.0f, 1.0f,
	.0f, 1.0f,
	.0f,.0f,
	.0f, 1.0f,
	.0f,.0f,
	1.0f,.0f,
	 1.0f, 1.0f,
	.0f,.0f,
	 1.0f,.0f,
	.0f,.0f,
	 1.0f, 1.0f,
	.0f, 1.0f,
	1.0f, 1.0f,
	1.0f,.0f,
	.0f,.0f,
	1.0f,  1.0f,
	.0f, .0f,
	.0f,  1.0f,
	1.0f, 1.0f,
	.0f, 1.0f,
	1.0f,.0f
};

std::vector<GLfloat> VAOLoader::getVerts(int id)
{
	return std::vector<GLfloat>(g_vertex_buffer_data, std::end(g_vertex_buffer_data));
}

std::vector<GLfloat> VAOLoader::getUVs(int id)
{
	return std::vector<GLfloat>(g_uv_buffer_data, std::end(g_uv_buffer_data));
}


//Loads the vertex data to the graphics card, returning a vao object.
VAO* VAOLoader::loadToVAO(GLfloat*verts, GLfloat*normals, GLfloat*uvs, GLuint*indices, int v, int u, int in)
{
	int vertArraySize = v*sizeof(verts[0]);
	int uvArraySize = u*sizeof(uvs[0]);
	int indiceArraySize = in*sizeof(indices[0]);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint VertexBuffer;
	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertArraySize, verts, GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvArraySize, uvs, GL_STATIC_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertArraySize, normals, GL_STATIC_DRAW);

	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiceArraySize, indices, GL_STATIC_DRAW);

	VAO * vao = new VAO(VertexArrayID);
	vao->setUVBuffer(uvbuffer);
	vao->setVertexBuffer(VertexBuffer);
	vao->setNormalBuffer(normalbuffer);
	vao->setIndexBuffer(elementbuffer);
	vao->vertCount = v;
	vao->indexCount = in;

	return vao;
}
