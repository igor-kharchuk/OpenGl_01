#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>

typedef struct {
    vec3 position;
    vec3 rotation;
    float fov;       // Кут огляду в радіанах
    float aspect;    // Співвідношення сторін екрану
    float near;      // Близька площина відсічки
    float far;       // Далека площина відсічки
} Camera;

#endif