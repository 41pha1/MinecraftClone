#include "BoundingBoxRenderer.h"

#include <glad/glad.h>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/detail/type_vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>

#include "BoundingBoxShader.h"
#include "Camera.h"
#include "Game.h"

BoundingBoxRenderer::BoundingBoxRenderer()
{
	shader = new BoundingBoxShader();
}

void BoundingBoxRenderer::render(Game * game)
{
	GLfloat vertices[] = {
		-0.0, -0.0, -0.0, 1.0,
		 1.0, -0.0, -0.0, 1.0,
		 1.0,  1.0, -0.0, 1.0,
		-0.0,  1.0, -0.0, 1.0,
		-0.0, -0.0,  1.0, 1.0,
		 1.0, -0.0,  1.0, 1.0,
		 1.0,  1.0,  1.0, 1.0,
		-0.0,  1.0,  1.0, 1.0,
	};
	GLuint vbo_vertices;
	glGenBuffers(1, &vbo_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLushort elements[] = {
	0, 1, 2, 3,
	4, 5, 6, 7,
	0, 4, 1, 5, 2, 6, 3, 7
	};
	GLuint ibo_elements;
	glGenBuffers(1, &ibo_elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glm::vec3 pos = game->camera->lookingAt;
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(pos.x,pos.y,pos.z));
	model = glm::scale(model, glm::vec3(1.0001f, 1.0001f, 1.0001f));

	glm::mat4 mvp = game->camera->Projection * game->camera->View * model;

	glUseProgram(shader->ProgramID);

	GLuint MatrixID = glGetUniformLocation(shader->ProgramID, "MVP");

	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
	  0,                  // attribute
	  4,                  // number of elements per vertex, here (x,y,z,w)
	  GL_FLOAT,           // the type of each element
	  GL_FALSE,           // take our values as-is
	  0,                  // no extra data between each position
	  0                   // offset of first element
	);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, 0);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_SHORT, (GLvoid*)(4*sizeof(GLushort)));
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_SHORT, (GLvoid*)(8*sizeof(GLushort)));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &vbo_vertices);
	glDeleteBuffers(1, &ibo_elements);
}

BoundingBoxRenderer::~BoundingBoxRenderer()
{

}

