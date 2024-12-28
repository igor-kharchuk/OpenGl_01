#version 330 core

float saturate(float x) {
    return clamp(x, 0.0, 1.0);
}

vec2 saturate(vec2 x) {
    return clamp(x, 0.0, 1.0);
}

vec3 saturate(vec3 x) {
    return clamp(x, 0.0, 1.0);
}

vec4 saturate(vec4 x) {
    return clamp(x, 0.0, 1.0);
}

in vec3 normal;

vec3 lightDir = vec3(0.3, 1.0, 0.2);
out vec4 FragColor;

void main() {
	float lambert = saturate(dot(lightDir, normal));
   // FragColor = vec4(smoothstep(-0.35, 1, lambert));
	//FragColor *= vec4(0.2, 0.3, 0.4, 0.0) * 2;
	FragColor = vec4(normal, 1.0);
	//FragColor = vec4(1.0);
}