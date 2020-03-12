#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0f); 
    mat4 temp = mat4(1.994894f);
    mat4 projection = mat4(0.994894f);
    mat4 view = mat4(0.994894f);
    mat4 model = mat4(0.994894f);
	float factor = 0.109003f;
	for(int i=0;i<500000;i++){
		factor += 0.000001f;
        factor = pow(factor, 1.234434f);
		temp * projection * view * model * mat4(1.9998472f+factor);
		temp /= factor;
		temp *= factor;
	}
}