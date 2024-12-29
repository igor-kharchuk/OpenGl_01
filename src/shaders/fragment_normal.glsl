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
in vec2 uv;

uniform sampler2D uTexture;

vec3 lightDir = vec3(0.3, 1.0, 0.2);
out vec4 FragColor;

void main() {
    vec3 ambientColor = vec3(0.2, 0.3, 0.7) * 0.5;
    vec3 lightColor = vec3(1.0, 1.0, 1.0);

    vec3 textureColor = texture(uTexture, uv).rgb;

	float lambert = saturate(dot(lightDir, normal));
    lambert = smoothstep(-1, 1, lambert);

    //FragColor = vec4(textureColor * lightColor * lambert + ambientColor, 1.0);
	//FragColor *= vec4(uv.x, uv.y, 0.0, 1.0);
	FragColor = vec4((normal + 1.0) / 2.0, 1.0);
	//FragColor = vec4(1.0);
}