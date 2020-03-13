#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;
layout (location = 3) in mat4 instanceMatrix;

out vec4 vCol;
out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;
out vec4 DirectionalLightSpacePos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 directionalLightTransform;
uniform float deltaA;
uniform float radius;

const float PI = 3.1415926;

void main()
{
	float x = model[3].x;
	float z = model[3].z;
	mat4 newModel = mat4(model);
	vec3 newPos = vec3(1.0f);
	if(deltaA>0){
		float r = sqrt(x*x + z*z);
		float sinA =  z/r;
		float A = asin(sinA);
		A += deltaA;
		if(A>2*PI){
			A = A - 2*PI;
		}
		float newZ = r*sin(A);
		float newX = r*cos(A);
		if(A>=0 && A<0.5f*PI){
			newZ *= -1;
		}else if(A>=0.5f*PI && A<PI){
			newX *= -1;
			newZ *= -1;
		}else if(A>=PI && A<1.5f*PI){
			newX *= -1;
		}
		newPos = vec3(newX, 1.0f, newZ);
		newModel[3] = vec4(newPos.x,newPos.y,newPos.z,newModel[3].w);
	}


	gl_Position = projection * view * newModel * vec4(pos, 1.0) ;
	DirectionalLightSpacePos = directionalLightTransform * newModel * vec4(pos, 1.0);
	
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
	
	TexCoords = tex;
	
	Normal = mat3(transpose(inverse(newModel))) * norm;
	
	FragPos = (newModel * vec4(pos, 1.0)).xyz; 
}