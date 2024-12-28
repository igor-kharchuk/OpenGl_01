#include "renderObject.h"

RenderObject createRenderObject(Mesh mesh) {
    RenderObject renderObj;
    
    renderObj.indexCount = mesh.indexCount;

    // Генерація VAO, VBO та EBO
    glGenVertexArrays(1, &renderObj.VAO);
    glGenBuffers(1, &renderObj.VBO);
    glGenBuffers(1, &renderObj.EBO);

    // Прив'язка VAO
    glBindVertexArray(renderObj.VAO);
    
    // Прив'язка та налаштування VBO
    glBindBuffer(GL_ARRAY_BUFFER, renderObj.VBO);
    glBufferData(GL_ARRAY_BUFFER, mesh.verticesSize, mesh.vertices, GL_STATIC_DRAW);

    // Прив'язка та налаштування EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderObj.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indicesSize, mesh.indices, GL_STATIC_DRAW);

    // Налаштування атрибутів для позиції, кольору та нормалі
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position)); // Позиції
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color)); // Кольори
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal)); // Нормалі
    glEnableVertexAttribArray(2);

    // Розв'язка VAO
    glBindVertexArray(0);

    return renderObj;
}

void drawRenderObject(RenderObject* renderObject, GLuint shaderProgram) {
    glUseProgram(shaderProgram);
    glBindVertexArray(renderObject->VAO);
    glDrawElements(GL_TRIANGLES, renderObject->indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void destroyRenderObject(RenderObject* renderObject) {
    // Видалити VBO, EBO та VAO
    glDeleteBuffers(1, &renderObject->VBO);
    glDeleteBuffers(1, &renderObject->EBO);
    glDeleteVertexArrays(1, &renderObject->VAO);
}
