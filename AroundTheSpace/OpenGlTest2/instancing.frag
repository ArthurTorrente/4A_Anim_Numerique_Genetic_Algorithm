#version 400

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

//in vec2 TextCoords;
//in vec3 PositionNormal;
in vec3 FragPos;

//uniform vec3 cameraPos;
//uniform samplerCube skybox;
uniform Material material;

out vec4 color;

void main()
{
	color = textureProj(material.diffuse,  FragPos);
	
		//vec3 I = normalize(FragPos - cameraPos);
    //vec3 R = reflect(I, normalize(PositionNormal));
	
	//vec2 out = vec2(FragPos);
	
  // color = texture(skybox, R);
  
 //color = textureProj(material.diffuse,  TextCoords);
}