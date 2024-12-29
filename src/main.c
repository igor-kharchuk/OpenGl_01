#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "mesh.h"
#include "cube.h"
#include "renderObject.h"
#include "shader_utils.h"
#include "camera.h"
#include "matrix_utils.h"
#include "texture_utils.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    printf("Window resized: width=%d, height=%d\n", width, height);
}

GLFWwindow* window;

void init() {
    if(!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        exit(-1);
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    window = glfwCreateWindow(800, 600, "OpenGL 01", NULL, NULL);
    if(!window) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        exit(-1);
    }
}

void loop() {
    // create shader program
    GLuint shaderProgram = createShaderProgramFromSource("src/shaders/vertex_shader.glsl", "src/shaders/fragment_shader.glsl", SHADER_LANG_GLSL);
    GLuint shaderProgramNorm = createShaderProgramFromSource("src/shaders/vertex_shader.glsl", "src/shaders/fragment_normal.glsl", SHADER_LANG_GLSL);
    GLuint shaderProgramUnlit = createShaderProgramFromSource("src/shaders/vertex_shader.glsl", "src/shaders/fragment_unlit.glsl", SHADER_LANG_GLSL);

    ROShaderLocations roShaderLocations = createShaderLocations(shaderProgram, "model", "normalMat", "view", "projection");
    ROShaderLocations roShaderLocationsNorm = createShaderLocations(shaderProgramNorm, "model", "normalMat", "view", "projection");
    ROShaderLocations roShaderLocationsUnlit = createShaderLocations(shaderProgramUnlit, "model", "normalMat", "view", "projection");

    // load texture
    GLuint texture = loadTextureFreeImage("src/textures/mushroom.png");
    glActiveTexture(GL_TEXTURE0); // Використовуємо текстурний юніт 0
    glBindTexture(GL_TEXTURE_2D, texture);
    GLint textureLocation = glGetUniformLocation(shaderProgram, "uTexture");
    glUniform1i(textureLocation, 0); // Текстурний юніт 0

    // setup camera
    Camera camera = {
	.position = {0.0f, 1.0f, 5.0f},
    	.rotation = {-0.2f, 0.0f, 0.0},
    	.fov = M_PI / 4.0f,           // 45 градусів у радіанах
    	.aspect = 800.0f / 600.0f,    // Співвідношення сторін екрану
    	.near = 0.1f,                 // Близька площина відсічки
    	.far = 100.0f 
      };

    // load mesh from obj
    Mesh mesh;
    
    if(!loadObj("src/3d_models/mushroom.obj", &mesh)){
        exit(-1); }

    // create render object
    RenderObject cubeRenderObject = createRenderObject(mesh);
    free(mesh.vertices);
    free(mesh.indices);

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        
        mat4 model1, model2, model3;
        mat4 norm1, norm2, norm3;

        float angle = glfwGetTime();
        vec3 modelPos = {-2.0f, -0.7f, 0.0f};
        vec3 modelRot = {0.0f, angle * 70, 0.0f};
        vec3 modelScale = {0.5f, 0.5f, 0.5f};

        createModelMatrix(&model1, modelPos, modelRot, modelScale);
        createNormalMatrix(&model1, &norm1);

        vec3 modelPos1 = {0.0f, -0.7f, 0.0f};
        createModelMatrix(&model2, modelPos1, modelRot, modelScale);
        createNormalMatrix(&model2, &norm2);

        vec3 modelPos2 = {2.0f, -0.7f, 0.0f};
        createModelMatrix(&model3, modelPos2, modelRot, modelScale);
        createNormalMatrix(&model3, &norm3);

        mat4 view, proj;
        createViewMatrix(&view, &camera);
        createProjectionMatrix(&proj, &camera);

        ROShaderMatrices roShadMatrices1 = {
            (GLfloat*) model1,
            (GLfloat*) norm1,
            (GLfloat*) view,
            (GLfloat*) proj
        };

        ROShaderMatrices roShadMatrices2 = {
            (GLfloat*) model2,
            (GLfloat*) norm2,
            (GLfloat*) view,
            (GLfloat*) proj
        };

        ROShaderMatrices roShadMatrices3 = {
            (GLfloat*) model3,
            (GLfloat*) norm3,
            (GLfloat*) view,
            (GLfloat*) proj
        };

        // Очищення буферів і рендер
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawRenderObject(&cubeRenderObject, shaderProgram, roShadMatrices1, roShaderLocations);
        drawRenderObject(&cubeRenderObject, shaderProgramNorm, roShadMatrices2, roShaderLocationsNorm);
        drawRenderObject(&cubeRenderObject, shaderProgramUnlit, roShadMatrices3, roShaderLocationsUnlit);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    destroyRenderObject(&cubeRenderObject);
}

void destroy() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

int main() {
    init();
    loop();
    destroy();

    return 0;
}