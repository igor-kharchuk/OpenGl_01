#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <GL/glew.h>
#include "camera.h"
#include <cglm/cglm.h>

void createModelMatrix (mat4* matrix, vec3 position, vec3 rotation, vec3 scale);
void createViewMatrix (mat4* matrix, Camera* camera);
void createProjectionMatrix (mat4* matrix, Camera* camera);
void createNormalMatrix(mat4* modelMatrix, mat4* normalMatrix);

#endif