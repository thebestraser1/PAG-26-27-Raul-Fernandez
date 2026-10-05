//
// Created by rferr on 05/10/2026.
//

#include "Camara.h"

namespace PAG {

    /**
     * Función para obtener la matriz de transformación obtenida tras multiplicar la de visión y proyección
     */
    PAG::Camara::Camara(float anchoVentana, float altoVentana) {
        aspect = anchoVentana / altoVentana;
    }

    /**
     * Función para obtener la matriz de transformación obtenida tras multiplicar la de visión y proyección
     */
    glm::mat4 PAG::Camara::getMatVP () {

        glm::mat4 v, p;

        v = glm::lookAt(position, lookAt, up);
        p = glm::perspective(fovY, aspect, zNear, zFar);

        glm::mat4 devolver = p*v;
        return devolver;
    }

    /**
     * Función para actualizar el aspecto (ante una posible redimensión de ventana)
     */
    void PAG::Camara::redimensionar(float ancho, float alto) {
        //El aspecto podría cambiar con una redimensión
        aspect = ancho / alto;
    }


} // PAG