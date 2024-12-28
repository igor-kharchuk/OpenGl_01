#include <stdio.h>
#include <stdlib.h>
#include "shader_utils.h"
#include <GL/glew.h>

char* loadShaderSource(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Не вдалося відкрити файл шейдера: %s\n", filename);
        return NULL;
    }

    // Отримуємо розмір файлу
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    // Виділяємо пам'ять для зчитування файлу
    char* source = (char*)malloc(length + 1);
    if (!source) {
        printf("Не вдалося виділити пам'ять для шейдера\n");
        fclose(file);
        return NULL;
    }

    // Зчитуємо вміст файлу
    size_t bytesRead = fread(source, 1, length, file);
    if (bytesRead != length) {
        printf("Помилка при зчитуванні файлу шейдера: зчитано %zu байт із %ld\n", bytesRead, length);
        free(source);
        fclose(file);
        return NULL;
    }

    source[length] = '\0'; // Завершаємо рядок
    fclose(file);

    return source;
}

GLuint compileGLSL(const char* shaderSource, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    // Перевірка на помилки компіляції
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Помилка компіляції шейдера: %s\n", infoLog);
    }

    return shader;
}

GLuint createShaderProgramFromSource(const char* vertexFileName, const char* fragmentFileName, ShaderLanguage shaderLanguage) {
    if(shaderLanguage == SHADER_LANG_GLSL) {
        char* vertexSource = loadShaderSource(vertexFileName);
        char* fragmentSource = loadShaderSource(fragmentFileName);

        GLuint vertexShader = compileGLSL(vertexSource, GL_VERTEX_SHADER);
        GLuint fragmentShader = compileGLSL(fragmentSource, GL_FRAGMENT_SHADER);

        free(vertexSource);
        free(fragmentSource);

        // Створення програми
        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // Перевірка на помилки лінкування
        GLint success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            printf("Помилка лінкування програми: %s\n", infoLog);
        }

        // Видалення шейдерів після лінкування (вони більше не потрібні)
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        return shaderProgram;
    } 
    else {
        printf ("Тип вихідного шейдерного файлу не підтримується\n");
        exit(-1);
    }
}