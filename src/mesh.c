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
    size_t uvCount = 0;
    size_t normalCount = 0;
    size_t indexCount = 0;

    // Перший цикл: підрахунок кількості вершин, текстурних координат, нормалей та індексів
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            vertexCount++;
        } else if (line[0] == 'v' && line[1] == 't') {
            uvCount++;
        } else if (line[0] == 'v' && line[1] == 'n') {
            normalCount++;
        } else if (line[0] == 'f') {
            indexCount += 3; // Для кожного трикутника 3 індекси
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

    vec3* normals = malloc(sizeof(vec3) * normalCount);
    vec2* uvs = malloc(sizeof(vec2) * uvCount);

    if (!normals || !uvs) {
        printf("Не вдалося виділити пам'ять для допоміжних масивів\n");
        free(mesh->vertices);
        free(mesh->indices);
        fclose(file);
        return 0;
    }

    // Повертаємо курсор файлу на початок
    fseek(file, 0, SEEK_SET);

    size_t vIndex = 0, uvIndex = 0, nIndex = 0, iIndex = 0;

    // Другий цикл: заповнення даних у відповідні масиви
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'v' && line[1] == ' ') {
            // Вершини
            vec3 vertex;
            sscanf(line, "v %f %f %f", &vertex[0], &vertex[1], &vertex[2]);
            glm_vec3_copy(vertex, mesh->vertices[vIndex].position);
            glm_vec3_copy((vec3){1.0f, 1.0f, 1.0f}, mesh->vertices[vIndex].color); // Припустимо, білий колір
            vIndex++;
        } else if (line[0] == 'v' && line[1] == 't') {
            // Текстурні координати
            vec2 uv;
            sscanf(line, "vt %f %f", &uv[0], &uv[1]);
            glm_vec2_copy(uv, uvs[uvIndex++]);
        } else if (line[0] == 'v' && line[1] == 'n') {
            // Нормалі
            vec3 normal;
            sscanf(line, "vn %f %f %f", &normal[0], &normal[1], &normal[2]);
            glm_vec3_copy(normal, normals[nIndex++]);
        } else if (line[0] == 'f') {
            // Індекси (трикутники)
            unsigned int vIdx[3], uvIdx[3], nIdx[3];
            int matched = sscanf(line, "f %u/%u/%u %u/%u/%u %u/%u/%u",
                                 &vIdx[0], &uvIdx[0], &nIdx[0],
                                 &vIdx[1], &uvIdx[1], &nIdx[1],
                                 &vIdx[2], &uvIdx[2], &nIdx[2]);

            if (matched == 9) {
                for (int k = 0; k < 3; k++) {
                    unsigned int vi = vIdx[k] - 1;
                    unsigned int uvi = uvIdx[k] - 1;
                    unsigned int ni = nIdx[k] - 1;

                    // Копіюємо UV та нормалі у відповідні вершини
                    glm_vec2_copy(uvs[uvi], mesh->vertices[vi].uv);
                    glm_vec3_copy(normals[ni], mesh->vertices[vi].normal);

                    mesh->indices[iIndex++] = vi; // Додаємо індекс вершини
                }
            }
        }
    }

    // Оновлення розмірів
    mesh->verticesSize = sizeof(Vertex) * vertexCount;
    mesh->indicesSize = sizeof(GLuint) * indexCount;

    mesh->vertexCount = vertexCount;
    mesh->indexCount = indexCount;

    // Звільняємо тимчасові масиви
    free(normals);
    free(uvs);

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