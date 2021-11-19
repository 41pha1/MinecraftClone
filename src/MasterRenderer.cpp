#include "MasterRenderer.h"

#include <glad/glad.h>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "BlockShader.h"
#include "BoundingBoxRenderer.h"
#include "Camera.h"
#include "Chunk.h"
#include "ChunkRenderer.h"
#include "ImageLoader.h"
#include <iostream>

class BoundingBoxRenderer;
ShaderProgram * blockShader;
ChunkRenderer *chunkRenderer;
BoundingBoxRenderer * boundingBoxRenderer;

MasterRenderer::MasterRenderer(int width, int height)
{
	boundingBoxRenderer = new BoundingBoxRenderer();
	blockShader = new BlockShader();
	camera = new Camera(70, width, height, 0.1, 1000);
}

MasterRenderer::~MasterRenderer()
{

}

void MasterRenderer::render(Game* game)
{
	glClearColor(SKY_COLOR.x, SKY_COLOR.y, SKY_COLOR.z, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
//	glEnable(GL_MULTISAMPLE);

	if(game->camera->renderWireframe)
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	else
		glPolygonMode( GL_FRONT, GL_FILL );

	glUseProgram(blockShader->ProgramID);
	GLuint SkyColor = glGetUniformLocation(blockShader->ProgramID, "skyColor");
	glUniform3f(SkyColor, SKY_COLOR.x, SKY_COLOR.y, SKY_COLOR.z);

	GLuint CamPos = glGetUniformLocation(blockShader->ProgramID, "camPos");
	glUniform3f(CamPos, game->camera->pos.x, game->camera->pos.y, game->camera->pos.z);

	GLuint viewDistance = glGetUniformLocation(blockShader->ProgramID, "viewDistance");
	glUniform1f(viewDistance, (game->RENDER_DISTANCE-1) * Chunk::SIZE);

	GLuint TextureID = glGetUniformLocation(blockShader->ProgramID, "textureSampler");
	glUniform1i(TextureID, 0);
	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, ImageLoader::getTextureAtlas());

	glm::mat4 vp = game->camera->Projection * game->camera->View;
	GLuint ViewProjection = glGetUniformLocation(blockShader->ProgramID, "VP");
	glUniformMatrix4fv(ViewProjection, 1, GL_FALSE, &vp[0][0]);

	for(Chunk * chunk : game->renderQueue)
	{
		chunkRenderer->render(blockShader, chunk, camera);
	}

	if(game->camera->blockInReach)
		boundingBoxRenderer->render(game);
}

