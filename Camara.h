//
// Created by rferr on 05/10/2026.
//

#ifndef PRACTICA1_CAMARA_H
#define PRACTICA1_CAMARA_H

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace PAG {

    class Camara {
    private:

        //Parámetros de la cámara (visión)
        glm::vec3 position = glm::vec3(0, 0, 3);
        glm::vec3 lookAt = glm::vec3(0, 0, 0);
        glm::vec3 up = glm::vec3(0, 1, 0);

        //Parámetros de la cámara (proyección)
        GLfloat fovY = 0.785;       //Unos 45º
        GLfloat aspect = 1.778;     //16:9
        GLfloat zNear = 0.0001;
        GLfloat zFar = 500;

    public:
        Camara() = default;

        glm::mat4 getMatVP () const;

        
    };
} // PAG

#endif //PRACTICA1_CAMARA_H
