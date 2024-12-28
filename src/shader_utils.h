#ifndef SHADER_UTILS
#define SHADER_UTILS

#include <GL/glew.h>

typedef enum {
    SHADER_LANG_GLSL = 0,
    SHADER_LANG_HLSL = 1
} ShaderLanguage;

char* loadShaderSource(const char* filename);
GLuint compileGLSL(const char* shaderSource, GLenum shaderType);
GLuint createShaderProgramFromSource(const char* vertexFileName, const char* fragmentFileName, ShaderLanguage shaderLanguage);

#endif // SHADER_UTILS