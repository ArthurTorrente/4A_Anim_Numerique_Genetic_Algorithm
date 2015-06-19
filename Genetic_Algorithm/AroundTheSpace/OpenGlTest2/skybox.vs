#version 330 core 
layout (location = 0) in vec3 aPositionVertex;
out vec3 TexCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;


void main()
{
    //gl_Position =   projectionMatrix * viewMatrix * vec4(aPositionVertex, 1.0);  
    //TexCoords = aPositionVertex;
	
	vec4 pos = projectionMatrix * viewMatrix * vec4(aPositionVertex, 1.0);
    gl_Position = pos.xyww;
    TexCoords = aPositionVertex;
}  