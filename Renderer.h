#ifndef PRACTICA1_RENDERER_H
#define PRACTICA1_RENDERER_H

#include <iostream>
#include <cstdarg>

#include "Listener.h"
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
    class Renderer : public Listener{
    private:
        static Renderer *instancia;
        GLfloat *_colorFondo;

        Renderer(); //Constructor privado (Singletone)

    public:
        ~Renderer() override;

        static Renderer &getInstancia();

        bool inicializarGLAD(void* ubicacionFunciones);

        void mostrarPropiedadesContextoGrafico();

        void activarPruebaProfundidad();

        void refrescar();

        void redimensionar(int width, int height);

        GLfloat* getColorFondo();

        void wakeUp(TipoVentana t, ...) override;


    };
} // PAG

#endif //PRACTICA1_RENDERER_H
