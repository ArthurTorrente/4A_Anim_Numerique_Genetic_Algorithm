#version 330 core

layout (location = 0) in vec3 aPositionVertex;
layout (location = 1) in vec2 aTextCoords;
layout (location = 2) in vec3 aPositionNormal;
layout (location = 3) in mat4 instanceMatrix;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform float time;
uniform vec3 viewPos;

out vec3 PositionNormal;
out vec3 FragPos;
//out vec2 TextCoords;

//generate a random value from four points
vec4 rand(vec2 A,vec2 B,vec2 C,vec2 D){ 

	vec2 s=vec2(12.9898,78.233); 
	vec4 tmp=vec4(dot(A,s),dot(B,s),dot(C,s),dot(D,s)); 

	return fract(sin(tmp) * 43758.5453)* 2.0 - 1.0; 
} 

//this is similar to a perlin noise function
float noise(vec2 coord,float d){ 

	vec2 C[4]; 

	float d1 = 1.0/d;

	C[0]=floor(coord*d)*d1; 

	C[1]=C[0]+vec2(d1,0.0); 

	C[2]=C[0]+vec2(d1,d1); 

	C[3]=C[0]+vec2(0.0,d1);


	vec2 p=fract(coord*d); 

	vec2 q=1.0-p; 

	vec4 w=vec4(q.x*q.y,p.x*q.y,p.x*p.y,q.x*p.y); 

	return dot(vec4(rand(C[0],C[1],C[2],C[3])),w); 
} 

void main()
{
	vec4 otherPosition = instanceMatrix * vec4(1.0f);
	vec3 pos = vec3(normalize(otherPosition));
	pos = -time * pos;
	
	vec4 finalPos = projectionMatrix * viewMatrix * modelMatrix * instanceMatrix  * vec4((aPositionVertex + pos), 1.0f);
	
	//vec4 finalPos = projectionMatrix * viewMatrix * modelMatrix * instanceMatrix  * vec4((aPositionVertex), 1.0f);
	
   //generate some noise values based on vertex position and the time value which comes in from our OF app
	float noiseAmntX = noise( vec2(-(time*0.1) + finalPos.x * 0.05f, 100.0f), 2.5 );
	float noiseAmntY = noise( vec2((time*0.1) + finalPos.y * 0.05f, finalPos.x * 0.01f), 2.5 );
	float  noiseAmntZ = noise( vec2((time*0.1) + finalPos.z * 0.05f, finalPos.z * 0.01f), 2.5 );
	mat4 transformationMatrix; 
	transformationMatrix[0] = vec4(1.0f,0.0f,0.0f,(noiseAmntX * 0.2));
	transformationMatrix[1] = vec4(0.0f,1.0f,0.0f,(noiseAmntY * 0.2));
	transformationMatrix[2] = vec4(0.0f,0.0f,1.0f,(noiseAmntZ * 0.1));
	transformationMatrix[3] = vec4(0.0f,0.0f,0.0f,1.0f);
	
	/*mat4 transformationMatrix; 
	transformationMatrix[0] = vec4(1.0f,0.0f,0.0f,0.0f);
	transformationMatrix[1] = vec4(0.0f,1.0f,0.0f,0.0f);
	transformationMatrix[2] = vec4(0.0f,0.0f,1.0f,0.0f);
	transformationMatrix[3] = vec4(0.0f,0.0f,0.0f,1.0f);*/
	
	finalPos = transformationMatrix*finalPos;
	
	gl_Position = vec4(finalPos);
	vec3 d2 = (vec3(finalPos) - vec3(viewPos));
	PositionNormal = mat3(transpose(inverse(modelMatrix * instanceMatrix * transformationMatrix))) * aPositionNormal;
	FragPos = ((viewPos + d2 / dot(d2, normalize(vec3(0.0f,0.0f, 1.0f))))*d2);
	//FragPos = vec3(finalPos);
}