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

            case (TipoMovimiento::Dolly): {
                std::va_list args;
                va_start(args, movimiento);

                GLfloat* variacion_x = va_arg(args, GLfloat*);
                GLfloat* variacion_z = va_arg(args, GLfloat*);

                //Dolly puede ser en X o en Z
                traslacionX(*variacion_x);
                traslacionZ(*variacion_z);

                va_end(args);
                break;
            }

            case (TipoMovimiento::Crane): {
                std::va_list args;
                va_start(args, movimiento);

                GLfloat* variacion_y = va_arg(args, GLfloat*);

                //Crane es en Y
                traslacionY(*variacion_y);

                va_end(args);
                break;
            }

            case (TipoMovimiento::Orbit): {
                std::va_list args;
                va_start(args, movimiento);

                GLfloat* variacion_longitud = va_arg(args, GLfloat*);
                GLfloat* variacion_latitud = va_arg(args, GLfloat*);

                //Dolly puede ser en X o en Z
                hacer_orbit_longitud(glm::radians(*variacion_longitud));
                hacer_orbit_latitud(glm::radians(*variacion_latitud));

                va_end(args);
                break;
            }
            default:;
        }
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
     *
     * Solo permitirá rotar 180º. Es decir, mirar hacia arriba o hacia abajo (no va más allá --> bug)
     *
     * @param angulo en radianes
     */
    void PAG::Camara::hacerTilt(GLfloat angulo) {

        glm::vec3 v_actual = obtener_vector_v();

        glm::mat4 m = glm::translate(position)
            * glm::rotate(angulo, obtener_vector_u())
            * glm::translate(-position);

        glm::vec3 lookAt_nuevo = glm::vec3(m * glm::vec4(lookAt, 1.0f));

        //Antes de efectuar comprobamos la EXCEPCIÓN de la vertical de la cámara
        //Para ello, voy a ver si el vector v es radicalmente distinto con la trasformación que se plantea
        //(es decir, si se ha pasado al "otro lado")
        glm::vec3 nuevo_n = glm::normalize(lookAt_nuevo - position);
        glm::vec3 nueva_u = glm::normalize(glm::cross(up, nuevo_n));
        glm::vec3 nueva_v = glm::cross(nuevo_n, nueva_u);

        //Si las v son muy distintas (una mira casi al lado contrario de la anterior) el coseno es negativo
        if (glm::dot(v_actual, nueva_v) >= 0.0f) {
            lookAt = glm::vec3(lookAt_nuevo);
        }
    }

    /**
     * Modifica la posición de la cámara según el eje X (de la cámara) --> U
     *
     * @param variacion
     */
    void PAG::Camara::traslacionX(GLfloat variacion) {
        position = glm::vec3(position + (obtener_vector_u() * variacion));
        lookAt = glm::vec3(lookAt + (obtener_vector_u() * variacion));
    }

    /**
    * Modifica la posición de la cámara según el eje Y (de la cámara) --> V
     *
     * @param variacion
     */
    void PAG::Camara::traslacionY(GLfloat variacion) {
        position = glm::vec3(position + (obtener_vector_v() * variacion));
        lookAt = glm::vec3(lookAt + (obtener_vector_v() * variacion));
    }

    /**
     * Modifica la posición de la cámara en el eje Z (de la cámara) --> N
     *
     * @param variacion
     */
    void PAG::Camara::traslacionZ(GLfloat variacion) {
        position = glm::vec3(position + (obtener_vector_n() * variacion));
        lookAt = glm::vec3(lookAt + (obtener_vector_n() * variacion));
    }


    /**
     * Hace el movimiento Orbit a nivel de longitud
     *
     * @param angulo
     */
    void PAG::Camara::hacer_orbit_longitud(GLfloat angulo) {
        glm::mat4 m = glm::translate(lookAt)
            * glm::rotate(angulo, obtener_vector_v())
            * glm::translate(-lookAt);

        //Aquí no hay problema con las verticales
        position = glm::vec3(m * glm::vec4(position, 1.0));

    }


    /**
     * Hace el movimiento Orbit a nivel de latitud (esta genera el problema de la vertical)
     *
     * @param angulo
     */
    void PAG::Camara::hacer_orbit_latitud(GLfloat angulo) {

        glm::mat4 m = glm::translate(lookAt)
            * glm::rotate(angulo, obtener_vector_u())
            * glm::translate(-lookAt);

        //Aquí hay que gestionar las verticales
        glm::vec3 nueva_position = glm::vec3(m * glm::vec4(position, 1.0));

        glm::vec3 nuevo_n = glm::normalize(lookAt - nueva_position);
        glm::vec3 nueva_u = glm::normalize(glm::cross(up, nuevo_n));
        glm::vec3 nueva_v = glm::cross(nuevo_n, nueva_u);

        glm::bvec3 son_colineales = glm::epsilonEqual(nueva_v, up, glm::epsilon<float>());

        if (glm::all(son_colineales)) {
            up = glm::vec3(0,0,1);

        }else {
            up = glm::vec3(0,1,0);
        }
        position = nueva_position;
    }


} // PAG