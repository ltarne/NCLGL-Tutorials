#version 330 core

uniform sampler2D waterTex;
uniform samplerCube cubeTex;

uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;

in Vertex {
  vec4 colour;
  vec2 texCoord;
  vec3 normal;
  vec3 tangent;
  vec3 binormal;
  vec3 worldPos;
} IN;

out vec4 fragColour;

void main(void) {
  vec4 diffuse = texture(waterTex, IN.texCoord);
  //vec4 cubeDiff = texture(cubeTex, IN.texCoord);
  vec3 incident = normalize(IN.worldPos - cameraPos);
  float dist = length(lightPos - IN.worldPos);
  float atten = 1.0 - clamp(dist / lightRadius, 0.2, 1.0);
  vec4 reflection = texture(cubeTex, reflect(incident, normalize(IN.normal)));

  fragColour = (lightColour * diffuse * atten) * (diffuse + reflection);
  //fragColour = cubeDiff;
}
