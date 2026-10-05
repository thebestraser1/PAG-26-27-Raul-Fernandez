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

    /**
     * Función para obtener el ángulo de visión (en horizontal) de la cámara (en grados)
     *
     * Es decir fovX en grados
     */
    GLfloat PAG::Camara::getAnguloVision() const {

        //Obtener fovX (con despejar en la fórmula sale esto):
        GLfloat fovX = 2.0f * atanf(tanf(fovY * 0.5f) * aspect);

        return glm::degrees(fovX);
    }


    /**
     * Función para mover la cámara según los diferentes movimientos establecidos
     */
    void Camara::mover(TipoMovimiento movimiento, ...) {
        switch (movimiento) {
            case (TipoMovimiento::Zoom): {
                std::va_list args;
                va_start(args, movimiento);

                GLfloat* angulo = va_arg(args, GLfloat*);
                fovY = FovX_a_FovY_grados(glm::radians(*angulo));

                va_end(args);
                break;
            }
            default:;
        }
    }

    /**
     * Devuelve FovY a partir de un ángulo de visión (horizontal) en radianes. Útil para el zoom.
     * @param angulo
     * @return FovY en radianes
     */
    GLfloat PAG::Camara::FovX_a_FovY_grados(GLfloat angulo) const {
        return 2.0f * atanf(tanf(angulo * 0.5f) / aspect);
    }
} // PAG