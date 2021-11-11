#ifndef BOUNDINGBOXSHADER_H_
#define BOUNDINGBOXSHADER_H_

#include <string>

#include "ShaderProgram.h"

class BoundingBoxShader : public ShaderProgram
{
public:
	BoundingBoxShader();
	const std::string vertexShaderFile = "res/shaders/boundingboxVertex.glsl";
	const std::string fragmentShaderFile = "res/shaders/boundingboxFragment.glsl";
	virtual ~BoundingBoxShader();
};

#endif /* BOUNDINGBOXSHADER_H_ */
