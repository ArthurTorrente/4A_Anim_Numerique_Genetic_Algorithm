#version 120

attribute vec3 aPositionVertex;
attribute vec3 aPositionNormal;

uniform vec4 uColor;
uniform vec3 uLightPosition;

varying vec4 vFinalVertexColor;

void main()
{
	vec3 normal = normalize(gl_NormalMatrix * aPositionNormal);

	vec3 lightPosition = normalize(uLightPosition);

	float docProduct = max(dot(normal, lightPosition), 0.0f);

	vFinalVertexColor = docProduct * uColor;

	gl_Position = gl_ModelViewProjectionMatrix * vec4(aPositionVertex, 1.0f);
}