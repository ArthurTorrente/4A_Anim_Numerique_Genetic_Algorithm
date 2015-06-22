#version 330 core
in vec3 aPositionVertex;
in vec2 aTextCoords;
in vec3 aPositionNormal;

out vec2 TexCoord;
out vec3 PositionNormal;
out vec3 FragPos;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

uniform vec3 viewPos;

uniform vec2 offsets[10];

void main()
{
	//vec2 offset = offsets[gl_InstanceID];
	//vec3 aOffset = aPositionVertex.xyz + offset;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(aPositionVertex, 1.0f);
	TexCoord = vec2(aTextCoords.x, 1.0 - aTextCoords.y);
	PositionNormal = mat3(transpose(inverse(modelMatrix))) * aPositionNormal;
	FragPos = vec3(modelMatrix * vec4(aPositionVertex, 1.0f));
}