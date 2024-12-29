#ifndef MESH_DATA_H
#define MESH_DATA_H

#include <cglm/cglm.h>
#include <GL/glew.h>

typedef struct {
    vec3 position; 
    vec3 color;     
    vec3 normal;
    vec2 uv;    
} Vertex;

typedef struct {
    Vertex* vertices;   // Масив для вершин
    GLuint* indices;    // Масив для індексів
    size_t verticesSize; 
    size_t indicesSize; 
    GLuint vertexCount;
    GLuint indexCount;
} Mesh;

int loadObj(const char* filename, Mesh* mesh);
void calculateNormals(Mesh* mesh);

#endif