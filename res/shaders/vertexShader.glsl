#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;

out vec3 position;
out vec2 UV;
out vec3 normal;

uniform mat4 MODEL;
uniform mat4 VP;

void main()
{
	position = (MODEL * vec4(vertexPosition_modelspace, 1.)).xyz;
	UV = vertexUV;
	normal = vertexNormal;
	gl_Position = VP * MODEL * vec4(vertexPosition_modelspace, 1.);
}