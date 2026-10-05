//
// Created by rferr on 05/10/2026.
//

#ifndef PRACTICA1_CAMARA_H
#define PRACTICA1_CAMARA_H

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include <cstdarg>  //Para funciones con número variable de elementos

namespace PAG {

        enum TipoMovimiento {
        Zoom,
    };

    class Camara {
    private:
        //Inicialización de la cámara por defecto. Todos los parámetros podrán tocarse con ventanas (salvo el aspect)

        //Parámetros de la cámara (visión)
        glm::vec3 position = glm::vec3(0, 0, 3);
        glm::vec3 lookAt = glm::vec3(0, 0, 0);
        glm::vec3 up = glm::vec3(0, 1, 0);

        //Parámetros de la cámara (proyección)
        GLfloat fovY = 0.785;       //Unos 45º verticales o 72º horizontales
        GLfloat aspect;             //Se inicializa en constructor (según dimensiones de pantalla)
        GLfloat zNear = 0.0001;
        GLfloat zFar = 500;

    public:
        Camara(float anchoVentana, float altoVentana);

        glm::mat4 getMatVP ();

        void redimensionar(float ancho, float alto);

        GLfloat getAnguloVision() const;

        void mover(TipoMovimiento movimiento, ...);

    private:
        GLfloat FovX_a_FovY_grados(GLfloat fovX) const;
    };
} // PAG

#endif //PRACTICA1_CAMARA_H
