#ifndef BLOCKSHADER_H_
#define BLOCKSHADER_H_

#include "ShaderProgram.h"

class BlockShader : public ShaderProgram
{
public:
	BlockShader();
	const std::string vertexShaderFile = "res/shaders/vertexShader.glsl";
	const std::string fragmentShaderFile = "res/shaders/fragmentShader.glsl";
	virtual ~BlockShader();
};

#endif
