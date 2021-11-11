#include "BoundingBoxShader.h"

BoundingBoxShader::BoundingBoxShader()
{
	ProgramID = LoadShaders(vertexShaderFile, fragmentShaderFile);

}

BoundingBoxShader::~BoundingBoxShader()
{

}

