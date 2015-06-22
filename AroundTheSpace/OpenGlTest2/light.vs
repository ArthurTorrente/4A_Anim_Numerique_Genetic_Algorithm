#version 330 core
in vec3 aPositionVertex;
in vec2 aTextCoords;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

out vec2 TexCoord;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPositionVertex, 1.0f); 
	TexCoord = vec2(aTextCoords.x, 1.0 - aTextCoords.y);
}