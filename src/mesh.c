#include "mesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cglm/cglm.h>
#include <GL/glew.h>

int loadObj(const char* filename, Mesh* mesh) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Не вдалося відкрити файл: %s\n", filename);
        return 0;
    }

    char line[128];
    size_t vertexCount = 0;
    size_t normalCount = 0;
    size_t indexCount = 0;

    // Перший цикл: підрахунок кількості вершин, нормалей і індексів
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            vertexCount++;
        } else if (line[0] == 'v' && line[1] == 'n') {
            normalCount++;
        } else if (line[0] == 'f') {
            indexCount += 3;  // Для кожного трикутника 3 індекси
        }
    }

    // Виділення пам'яті після підрахунку кількості елементів
    mesh->vertices = malloc(sizeof(Vertex) * vertexCount);
    mesh->indices = malloc(sizeof(GLuint) * indexCount);

    if (!mesh->vertices || !mesh->indices) {
        printf("Не вдалося виділити пам'ять для структури Mesh\n");
        fclose(file);
        return 0;
    }

    // Повертаємо курсор файлу на початок
    fseek(file, 0, SEEK_SET);

    size_t vIndex = 0, nIndex = 0, iIndex = 0;

    // Другий цикл: заповнення даних у відповідні масиви
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            // Вершини
            vec3 vertex;
            sscanf(line, "v %f %f %f", &vertex[0], &vertex[1], &vertex[2]);
            glm_vec3_copy(vertex, mesh->vertices[vIndex].position);
            glm_vec3_copy((vec3){1.0f, 1.0f, 1.0f}, mesh->vertices[vIndex].color); // Припустимо, білий колір
            vIndex++;
        } else if (line[0] == 'v' && line[1] == 'n') {
            // Нормалі
            vec3 normal;
            sscanf(line, "vn %f %f %f", &normal[0], &normal[1], &normal[2]);
            glm_vec3_copy(normal, mesh->vertices[nIndex].normal);
            nIndex++;
        } else if (line[0] == 'f') {
            // Індекси (трикутники)
            GLuint idx[3];
            int matched = sscanf(line, "f %u/%*u/%*u %u/%*u/%*u %u/%*u/%*u", &idx[0], &idx[1], &idx[2]);
            if (matched == 3) {
                mesh->indices[iIndex++] = idx[0] - 1; // Переводимо індекси з 1 в 0
                mesh->indices[iIndex++] = idx[1] - 1;
                mesh->indices[iIndex++] = idx[2] - 1;
            }
        }
    }

    // Оновлення розмірів
    mesh->verticesSize = sizeof(Vertex) * vertexCount;
    mesh->indicesSize = sizeof(GLuint) * indexCount;

    mesh->vertexCount = vertexCount;
    mesh->indexCount = indexCount;

    fclose(file);
    return 1;
}

void calculateNormals(Mesh* mesh) {
    for (size_t i = 0; i < mesh->indexCount; i += 3) {
        GLuint i0 = mesh->indices[i];
        GLuint i1 = mesh->indices[i + 1];
        GLuint i2 = mesh->indices[i + 2];

        vec3 v0, v1, v2, edge1, edge2, normal;
        glm_vec3_copy(mesh->vertices[i0].position, v0);
        glm_vec3_copy(mesh->vertices[i1].position, v1);
        glm_vec3_copy(mesh->vertices[i2].position, v2);

        // Обчислення векторів країв трикутника
        glm_vec3_sub(v1, v0, edge1);
        glm_vec3_sub(v2, v0, edge2);

        // Обчислення нормалі через векторний добуток
        glm_vec3_cross(edge1, edge2, normal);
        glm_vec3_normalize(normal);

        // Призначення нормалі всім вершинам трикутника
        glm_vec3_add(mesh->vertices[i0].normal, normal, mesh->vertices[i0].normal);
        glm_vec3_add(mesh->vertices[i1].normal, normal, mesh->vertices[i1].normal);
        glm_vec3_add(mesh->vertices[i2].normal, normal, mesh->vertices[i2].normal);
    }

    // Нормалізація всіх нормалей
    for (size_t i = 0; i < mesh->vertexCount; i++) {
        glm_vec3_normalize(mesh->vertices[i].normal);
    }
}