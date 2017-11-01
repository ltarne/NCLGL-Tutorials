#version 330 core

uniform sampler2D tex;

in Vertex	{
	vec4 colour;
	vec2 texCoord;
} IN;

out vec4 fragColor;


void main(void)	{
	//fragColor = vec4(1.0, 0.41, 0.71, 1.0);
	fragColor = texture(tex, IN.texCoord);
}
