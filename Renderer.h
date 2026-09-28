#ifndef PRACTICA1_RENDERER_H
#define PRACTICA1_RENDERER_H

#include <iostream>
#include <cstdarg>  //Para funciones con número variable de elementos

#include "Listener.h"
#include "glad/glad.h"

#include <fstream>
#include <sstream>



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

        //Práctica 3
        GLuint idVS = 0; // Identificador del vertex shader
        GLuint idFS = 0; // Identificador del fragment shader
        GLuint idSP = 0; // Identificador del shader program
        GLuint idVAO = 0; // Identificador del vertex array object
        GLuint idVBO = 0; // Identificador del vertex buffer object
        GLuint idIBO = 0; // Identificador del index buffer object

        Renderer(); //Constructor privado (Singletone)

        static std::string cargarFichero(const std::string& ruta);

        static void revisarFallosCompilacion(GLuint id, const std::string& tipoShader);

        static void revisarFallosEnlazadoPrograma(GLuint idPrograma);

    public:
        ~Renderer() override;

        static Renderer &getInstancia();

        bool inicializarGLAD(void* ubicacionFunciones);

        void mostrarPropiedadesContextoGrafico();

        void inicializarOpenGL();

        void creaShaderProgram();

        void creaModelo();

        void refrescar();

        void redimensionar(int width, int height);

        GLfloat* getColorFondo() const;

        void wakeUp(TipoVentana t, ...) override;


    };
} // PAG

#endif //PRACTICA1_RENDERER_H
