#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    mat4 temp = mat4(1.994894f);
    mat4 projection = mat4(0.994894f);
    mat4 view = mat4(0.994894f);
    mat4 model = mat4(0.994894f);
	float factor = 0.109003f;
	for(int i=0;i<50000;i++){
		factor += 0.000001f;
        factor = pow(factor, 1.234434f);
		temp * projection * view * model * mat4(1.9998472f+factor);
		temp /= factor;
		temp *= factor;
	}
    FragColor = texture(texture_diffuse1, TexCoords);
}