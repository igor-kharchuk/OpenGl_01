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

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void setupMatrices(mat4* modelMatrix, mat4* viewMatrix, mat4* projectionMatrix, mat4* normalMatrix) {

    // Обертання куба
    float angle = glfwGetTime();
    vec3 modelPos = {0.0f, 0.0f, 0.0f};
    vec3 modelRot = {0.0f, angle * 70, 0.0f};
    vec3 modelScale = {1.0f, 1.0f, 1.0f};

    createModelMatrix(modelMatrix, modelPos, modelRot, modelScale);

    Camera camera = {
	.position = {0.0f, 0.0f, 5.0f},
    	.rotation = {0.0f, 0.0f, 0.0},
    	.fov = M_PI / 4.0f,           // 45 градусів у радіанах
    	.aspect = 800.0f / 600.0f,    // Співвідношення сторін екрану
    	.near = 0.1f,                 // Близька площина відсічки
    	.far = 100.0f 
      };

    createViewMatrix(viewMatrix, &camera);
    createProjectionMatrix(projectionMatrix, &camera);
    createNormalMatrix(modelMatrix, normalMatrix);
}

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
    Mesh cubeMesh;
    //createCube(&cubeMesh);
    
    if(!loadObj("src/3d_models/monkey.obj", &cubeMesh)){
        exit(-1);
    }
    calculateNormals(&cubeMesh);

    RenderObject cubeRenderObject = createRenderObject(cubeMesh);

    free(cubeMesh.vertices);
    free(cubeMesh.indices);

    GLuint shaderProgram = createShaderProgramFromSource("src/shaders/vertex_shader.glsl", "src/shaders/fragment_shader.glsl", SHADER_LANG_GLSL);
    
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    GLuint normalLoc = glGetUniformLocation(shaderProgram, "normalMat");

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        // Oновлення і передача матриць
        mat4 modelMatrix, viewMatrix, projectionMatrix, normalMatrix;
        setupMatrices(&modelMatrix, &viewMatrix, &projectionMatrix, &normalMatrix);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (GLfloat*)modelMatrix);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (GLfloat*)viewMatrix);
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (GLfloat*)projectionMatrix);
        glUniformMatrix4fv(normalLoc, 1, GL_FALSE, (GLfloat*)normalMatrix);

        // Очищення буферів і рендер
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawRenderObject(&cubeRenderObject, shaderProgram);
        
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