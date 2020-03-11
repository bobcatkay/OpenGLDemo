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

uniform int instanceMode;
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 directionalLightTransform;

void main()
{
	mat4 model0 = model;
	if(instanceMode > 0){
		model0 = instanceMatrix;
	}

	gl_Position = projection * view * model0 * vec4(pos, 1.0);
	DirectionalLightSpacePos = directionalLightTransform * model0 * vec4(pos, 1.0);
	
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
	
	TexCoords = tex;
	
	Normal = mat3(transpose(inverse(model0))) * norm;
	
	FragPos = (model0 * vec4(pos, 1.0)).xyz; 
}