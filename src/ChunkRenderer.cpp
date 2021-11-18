#include "ChunkRenderer.h"

#include <glad/glad.h>
#include <iostream>

#include "Chunk.h"
#include "ShaderProgram.h"
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
