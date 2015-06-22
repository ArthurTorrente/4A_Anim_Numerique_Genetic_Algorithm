#version 330 core
in vec3 aPositionVertex;
//in vec2 aTextCoords;
//in vec3 aPositionNormal;
in mat4 instanceMatrix;

//out vec2 TexCoord;
//out vec3 PositionNormal;
//out vec3 FragPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    gl_Position = projection * view * instanceMatrix * vec4(aPositionVertex, 1.0f); 
    //TexCoord = vec2(aTextCoords.x, 1.0 - aTextCoords.y);
	//PositionNormal = mat3(transpose(inverse(instanceMatrix))) * aPositionNormal;
	//FragPos = vec3(instanceMatrix * vec4(aPositionVertex, 1.0f));
}