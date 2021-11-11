#include "ChunkRenderer.h"

#include <glad/glad.h>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <iostream>

#include "BlockShader.h"
#include "Camera.h"
#include "Chunk.h"
#include "ImageLoader.h"
#include "VAO.h"

void ChunkRenderer :: render(ShaderProgram *shader, Chunk * chunk, Camera * cam)
{
	//if(chunk->empty)
	//	return;

	chunk->vao->enableVAO();

	GLuint ModelMatrix = glGetUniformLocation(shader->ProgramID, "MODEL");
	glUniformMatrix4fv(ModelMatrix, 1, GL_FALSE, &chunk->model[0][0]);

	//glDrawArrays(GL_TRIANGLES, 0, chunk->vao->indexCount);
	glDrawElements(GL_TRIANGLES, chunk->vao->indexCount, GL_UNSIGNED_INT, (void*)0);
	chunk->vao->disableVAO();
}