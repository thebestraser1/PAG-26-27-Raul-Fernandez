//
// Created by rferr on 05/10/2026.
//

#ifndef PRACTICA1_CAMARA_H
#define PRACTICA1_CAMARA_H

#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>

#include <cstdarg>  //Para funciones con número variable de elementos


#include <iostream>

namespace PAG {

    enum TipoMovimiento {
        Zoom,
        Pan,
    };


    class Camara {
    private:
        //Inicialización de la cámara por defecto. Todos los parámetros podrán tocarse con ventanas (salvo el aspect)

        //Parámetros de la cámara (visión)
        glm::vec3 position = glm::vec3(0, 0, 3);
        glm::vec3 lookAt = glm::vec3(0, 0, -1);
        glm::vec3 up = glm::vec3(0, 1, 0);

        //Parámetros de la cámara (proyección)
        GLfloat fovY = 0.785;       //Unos 45º verticales o 72º horizontales
        GLfloat aspect;             //Se inicializa en constructor (según dimensiones de pantalla)
        GLfloat zNear = 0.0001;
        GLfloat zFar = 500;

        //Tipo de movimiento de cámara seleccionado
        TipoMovimiento _tipoMovimientoSeleccionado = Zoom;

    public:
        Camara(float anchoVentana, float altoVentana);

        glm::mat4 getMatVP ();

        void redimensionar(float ancho, float alto);

        GLfloat getAnguloVision() const;

        TipoMovimiento* getTipoMovimientoActual ();

        void mover(TipoMovimiento tipo, ...);

    private:
        //Funciones auxiliares de cálculo
        glm::vec3 obtener_vector_n();
        glm::vec3 obtener_vector_u();
        glm::vec3 obtener_vector_v();

        //Funciones de movimiento auxiliares
        void hacerZoom(GLfloat fovX);
        void hacerPan(GLfloat angulo);
    };
} // PAG

#endif //PRACTICA1_CAMARA_H
