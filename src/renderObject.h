#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include <GL/glew.h>
#include "mesh.h"

typedef struct 
{
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint indexCount;
} RenderObject;

RenderObject createRenderObject(Mesh mesh);
void drawRenderObject(RenderObject* renderObject, GLuint shaderProgram);
void destroyRenderObject(RenderObject* renderObject);

#endif