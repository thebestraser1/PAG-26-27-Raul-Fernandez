#include "Renderer.h"

namespace PAG {
    //Inicialización de la instancia única a nulo
    PAG::Renderer *PAG::Renderer::instancia = nullptr;

    Renderer::Renderer() {
        //Inicizalización de variables
        _colorFondo = new GLfloat[4]{0.6f, 0.6f, 0.6f, 1.0f};
    }

    Renderer::~Renderer() {
        //Liberamos punteros
        delete[] _colorFondo;
        _colorFondo = nullptr;
    }


    //Métodos

    /**
     * Función para consultar el objeto único de la clase
     * @return La dirección de memoria del objeto
     */
    PAG::Renderer &PAG::Renderer::getInstancia() {
        if (!instancia) {
            // Lazy initialization: si aún no existe, lo crea
            instancia = new Renderer();
        }
        return *instancia;
    }

    /**
     * Función para establecer una referencia a las funciones OpenGL del driver gráfico
     *
     * @var ubicacionFunciones es un puntero a una función genérica. Después se castea a GLADloadproc
     */
    bool Renderer::inicializarGLAD(void *ubicacionFunciones) {
        return gladLoadGLLoader((GLADloadproc) ubicacionFunciones); //Casteo dentro
    }


    /**
     * Mostrar propiedades del contexto gráfico
     */
    void Renderer::mostrarPropiedadesContextoGrafico() {
        std::cout << "Grafica en uso: " << glGetString(GL_RENDERER) << std::endl
                << "Fabricante: " << glGetString(GL_VENDOR) << std::endl
                << "Version de OpenGL: " << glGetString(GL_VERSION) << std::endl
                << "OpenGL Shading Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    }


    /**
     * Función para activar la prueba de profundidad (Z-buffering). Esto
     * determina qué superficies son visibles y cuáles están ocultas
     */
    void Renderer::activarPruebaProfundidad() {
        glEnable(GL_DEPTH_TEST);
    }


    /**
     * Función OpenGL que devuelve el color del fondo
     */
    GLfloat *Renderer::getColorFondo() {
        return _colorFondo;
    }


    /**
     * Función OpenGL para refrescar la ventana (encapsula la parte de OpenGL)
     */
    void Renderer::refrescar() {
        glClearColor(_colorFondo[0], _colorFondo[1], _colorFondo[2], _colorFondo[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     //Pinta el Buffer trasero
    }

    /**
     * Función OpenGL para redimensionar ventana
     */
    void Renderer::redimensionar(int width, int height) {
        glViewport(0, 0, width, height);
    }


    void Renderer::wakeUp(TipoVentana t, ...) {
        switch (t) {
            case TipoVentana::V_Selecc_Color: {
                std::va_list args;
                va_start(args, t);
                GLfloat* nuevoColor = va_arg(args, GLfloat*);
                //En el guión aparece vec3 de GLM. De momento lo dejo así para que no haya leak de memoria
                if (nuevoColor) {
                    _colorFondo[0] = nuevoColor[0];
                    _colorFondo[1] = nuevoColor[1];
                    _colorFondo[2] = nuevoColor[2];
                    _colorFondo[3] = nuevoColor[3];
                    refrescar();
                }
                va_end(args);
                break;
            }
            default: ;
                // Procesar el resto de tipos de ventana
        }
        // Terminar cualquier otro procesamiento que sea necesario
    }
} // PAG
