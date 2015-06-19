#version 330 core
in vec4 aPositionVertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * vec4(aPositionVertex.xy, 0.0, 1.0);
    TexCoords = aPositionVertex.zw;
}  