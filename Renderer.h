#ifndef PRACTICA1_RENDERER_H
#define PRACTICA1_RENDERER_H

#include <iostream>
#include "glad/glad.h"

/**
* Espacio de nombres para las prácticas de Programación de Aplicaciones
* Gráficas
*/
namespace PAG {


    /**
     * @brief Clase encargada de encapsular la gestión del área de dibujo
     * OpenGL
     *
     * Esta clase coordina el renderizado de las escenas OpenGL. Se implementa
     * aplicando el patrón de diseño Singleton. Está pensada para que las
     * funciones callback hagan llamadas a sus métodos
     */
    class Renderer {
    private:
        static Renderer *instancia;
        GLfloat colorFondo[4] = {0.6f, 0.6f, 0.6f, 1.0f}; // valor inicial por defecto

        Renderer(); //Constructor privado (Singletone)

    public:
        virtual ~Renderer();

        static Renderer &getInstancia();

        bool inicializarGLAD(void* ubicacionFunciones);

        void mostrarPropiedadesContextoGrafico();

        void activarPruebaProfundidad();

        void refrescar();

        void redimensionar(int width, int height);

        void cambiarColorFondo(GLfloat r, GLfloat g, GLfloat b, GLfloat a);

        GLfloat* getColorFondo();

        float *getEscalaTexto();


    };
} // PAG

#endif //PRACTICA1_RENDERER_H
