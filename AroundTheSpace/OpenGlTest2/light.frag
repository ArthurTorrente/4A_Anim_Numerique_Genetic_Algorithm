#version 330 core

struct Material {
    sampler2D diffuse;
};

in vec2 TexCoord;

out vec4 color;

uniform Material material;

void main()
{
	vec3 text = vec3(texture(material.diffuse, TexCoord));
	//color = vec4(text, 1.0f);
	color = vec4(1.0f, 1.0f, 0.0f, 0.1f);
}