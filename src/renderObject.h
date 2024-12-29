#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include <GL/glew.h>
#include "mesh.h"
#include "camera.h"

typedef struct 
{
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint indexCount;
} RenderObject;

typedef struct
{
    GLuint modelLoc;
    GLuint normalLoc;
    GLuint viewLoc;
    GLuint projectionLoc;
} ROShaderLocations;

typedef struct
{
    GLfloat* modelMatrix;
    GLfloat* normalMatrix;
    GLfloat* viewMatrix;
    GLfloat* projectionMatrix;
} ROShaderMatrices;

RenderObject createRenderObject(Mesh mesh);
ROShaderLocations createShaderLocations(GLuint shaderProgram, char* modelLocName, char* normalLocName, char* viewLocName, char* projectionLocName);
void drawRenderObject(RenderObject* renderObject, GLuint shaderProgram, ROShaderMatrices shaderMatrices, ROShaderLocations shaderLocations);
void destroyRenderObject(RenderObject* renderObject);

#endif