#include <string.h>
#include "matrix_utils.h"

void createModelMatrix (mat4* matrix, vec3 position, vec3 rotation, vec3 scale) {
    glm_mat4_identity(*matrix);

    glm_scale(*matrix, scale);

    mat4 rotationMatrix;
    glm_rotate_x(*matrix, glm_rad(rotation[0]), rotationMatrix);
    glm_mat4_copy(rotationMatrix, *matrix);
    glm_rotate_y(*matrix, glm_rad(rotation[1]), rotationMatrix);
    glm_mat4_copy(rotationMatrix, *matrix);
    glm_rotate_z(*matrix, glm_rad(rotation[2]), rotationMatrix);
    glm_mat4_copy(rotationMatrix, *matrix);

    glm_translate(*matrix, position);
}

void createViewMatrix(mat4* matrix, Camera* camera) {
    glm_mat4_identity(*matrix);

    mat4 rotationMatrix;
    glm_rotate_x(*matrix, -camera->rotation[0], rotationMatrix);
    glm_mat4_copy(rotationMatrix, *matrix);
    glm_rotate_y(*matrix, -camera->rotation[1], rotationMatrix);
    glm_mat4_copy(rotationMatrix, *matrix);
    glm_rotate_z(*matrix, -camera->rotation[2], rotationMatrix);
    glm_mat4_copy(rotationMatrix, *matrix);

    glm_translate(*matrix, (vec3){-camera->position[0], -camera->position[1], -camera->position[2]});
}

void createProjectionMatrix(mat4* matrix, Camera* camera) {
    glm_perspective(camera->fov, camera->aspect, camera->near, camera->far, *matrix);
}

void createNormalMatrix(mat4* modelMatrix, mat4* normalMatrix) {
    glm_mat4_inv(*modelMatrix, *normalMatrix);
    glm_mat4_transpose(*normalMatrix);
}