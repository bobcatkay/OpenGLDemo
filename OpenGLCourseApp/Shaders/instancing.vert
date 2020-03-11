#version 330

layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoords;
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

void main()
{
	gl_Position = projection * view * instanceMatrix * vec4(position, 1.0f);
	DirectionalLightSpacePos = directionalLightTransform * model * vec4(pos, 1.0);
	
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);
	
	TexCoords = texCoords;
	
	Normal = mat3(transpose(inverse(model))) * norm;
	
	FragPos = (model * vec4(pos, 1.0)).xyz; 
}