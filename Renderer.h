#ifndef PRACTICA1_RENDERER_H
#define PRACTICA1_RENDERER_H

#include <iostream>
#include <cstdarg>  //Para funciones con número variable de elementos

#include "Camara.h"
#include "Listener.h"
#include "ShaderProgram.h"



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

        ShaderProgram shader_program;

        //Modelo
        GLuint idVAO = 0; // Identificador del vertex array object
        GLuint idVBO = 0; // Identificador del vertex buffer object
        GLuint idIBO = 0; // Identificador del index buffer object

        //Aspecto de ventana
        int anchoVentana = 1024;
        int altoVentana = 576;

        //Camara
        Camara _camara;

        //Ventanas que escuchan a lo que cambie en el Renderer
        std::vector<Listener*> _listeners;

        Renderer(); //Constructor privado (Singletone)

    public:
        ~Renderer() override;

        static Renderer &getInstancia();

        bool inicializarGLAD(void* ubicacionFunciones);

        void mostrarPropiedadesContextoGrafico();

        void inicializarOpenGL();

        void creaModelo();

        void refrescar();

        void wakeUp(TipoVentana t, bool ventana_a_renderer ...) override;

        void redimensionar(int width, int height);

        GLfloat* getColorFondo() const;

        int ancho_ventana() const;

        int alto_ventana() const;

        Camara getCamara() const;

        void hacerZoomRaton();

        void addListener ( Listener *listener );

    private:
        void controlarUniforms(int idSP);
        void warn_listeners_camara();
    };
} // PAG

#endif //PRACTICA1_RENDERER_H
