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
     * Función para obtener la matriz de transformación obtenida tras multiplicar la de visión y proyección
     */
    TipoMovimiento* PAG::Camara::getTipoMovimientoActual () {
        return &_tipoMovimientoSeleccionado;
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
                hacerZoom(glm::radians(*angulo));

                va_end(args);
                break;
            }
            case (TipoMovimiento::Pan): {
                std::va_list args;
                va_start(args, movimiento);

                GLfloat* variacion = va_arg(args, GLfloat*);
                hacerPan(glm::radians(*variacion));

                va_end(args);
                break;
            }
            case (TipoMovimiento::Tilt): {
                std::va_list args;
                va_start(args, movimiento);

                GLfloat* variacion = va_arg(args, GLfloat*);
                hacerTilt(glm::radians(*variacion));

                va_end(args);
                break;
            }
            default:;
        }

        /**

        //Tras mover, comprobamos la EXCEPCIÓN de la cámara --> Ver si n y up son colineales
        glm::vec3 n = obtener_vector_n();

        glm::bvec3 colineales = glm::epsilonEqual(n, up, glm::epsilon<float>());

        if (glm::all(colineales)) {

            //Cambiamos up (eje Y) por Z para calcular el producto vectorial con u. Cuando salgamos de esta situación, se restaura
            up = glm::vec3(0,0,1);
        }else {
            up = glm::vec3(0, 1, 0);
        }
        */

    }



    /**
     * Obtener el vector n de las coordenadas de la cámara
     */
    glm::vec3 PAG::Camara::obtener_vector_n() {
        return glm::normalize(position - lookAt);
    }

    /**
     * Obtener el vector u de las coordenadas de la cámara
     */
    glm::vec3 PAG::Camara::obtener_vector_u() {
        glm::vec3 n = obtener_vector_n();

        //Con el producto vectorial con el vector vertical (v) sale u
        return glm::normalize(glm::cross(up, n));
    }

    /**
     * Obtener el vector v de las coordenadas de la cámara
     */
    glm::vec3 PAG::Camara::obtener_vector_v() {
        glm::vec3 n = obtener_vector_n();
        glm::vec3 u = obtener_vector_u();   //Optimizable para no llamar a n 2 veces (pero mejor comprensión del proceso)

        return glm::normalize(glm::cross(n, u));
    }



    /**
     * Morifica FovY a partir de un ángulo de visión (horizontal) en radianes. Útil para el zoom.
     * @param angulo en radianes
     */
    void PAG::Camara::hacerZoom(GLfloat angulo) {
        fovY = 2.0f * atanf(tanf(angulo * 0.5f) / aspect);
    }



    /**
     * Modifica (rota) el punto LookAt a partir del vector V de la cámara.
     * @param angulo en radianes
     */
    void PAG::Camara::hacerPan(GLfloat angulo) {

        glm::mat4 m = glm::translate(position)
                    * glm::rotate(angulo, obtener_vector_v())
                    * glm::translate(-position);

        glm::vec4 lookAt_aux = m * glm::vec4(lookAt, 1.0f);              // w = 1: es un punto

        lookAt = glm::vec3(lookAt_aux);
    }


    /**
     * Modifica (rota) el punto LookAt a partir del vector U de la cámara.
     * @param angulo en radianes
     */
    void PAG::Camara::hacerTilt(GLfloat angulo) {

        glm::mat4 m = glm::translate(position)
                    * glm::rotate(angulo, obtener_vector_u())
                    * glm::translate(-position);

        glm::vec4 lookAt_aux = m * glm::vec4(lookAt, 1.0f);              // w = 1: es un punto

        lookAt = glm::vec3(lookAt_aux);
    }


} // PAG