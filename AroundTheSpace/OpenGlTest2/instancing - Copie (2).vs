#version 330 core
layout (location = 0) in vec3 aPositionVertex;
layout (location = 1) in vec2 aTextCoords;
layout (location = 2) in vec3 aPositionNormal;
layout (location = 3) in mat4 instanceMatrix;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

out vec3 Normal;
out vec3 Position;

void main()
{
    gl_Position = projectionMatrix * viewMatrix * instanceMatrix * vec4(aPositionVertex.x, aPositionVertex.y, aPositionVertex.z, 1.0f);
	//TextCoords = aTextCoords;
	//PositionNormal = mat3(transpose(inverse(instanceMatrix))) * aPositionNormal;
	//FragPos = vec3(instanceMatrix * vec4(aPositionVertex, 1.0f));
	Normal = mat3(transpose(inverse(instanceMatrix))) * aPositionNormal;
    Position = vec3(instanceMatrix * vec4(aPositionVertex, 1.0f));
}