#version 330 core
layout(location = 0) in vec3 aPos; 
layout(location = 1) in vec3 aColor; 
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec2 aUV; 

uniform mat4 model;     
uniform mat4 view;        
uniform mat4 projection;  
uniform mat4 normalMat;	  

out vec3 normal;
out vec2 uv;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    normal = (normalMat * vec4(aNormal, 1)).rgb;
    uv = aUV;
    //normal = aNormal;
}