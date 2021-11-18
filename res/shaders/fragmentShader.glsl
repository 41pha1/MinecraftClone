#version 330 core
in vec3 position;
in vec2 UV;
in vec3 normal;

out vec3 color;

uniform sampler2D textureSampler;
uniform vec3 skyColor;
uniform vec3 camPos;
uniform float viewDistance;

void main()
{
  vec4 texCol = texture(textureSampler, UV);
  if(texCol.a < 0.5) 
  	discard;
  vec3 lightDir = normalize(vec3(0.5, 1, 0.7));
  float diffuse = (0.5+ max(0., dot(normal, lightDir)))/1.5;
  color = texCol.rgb * diffuse;
  float fogLinear =  min(1.0, length(position-camPos) / (viewDistance));
  color = mix (color, skyColor, exp((viewDistance * 0.1) * (fogLinear - 1)));
}
