
#include "Renderer.h"

namespace PAG {
    //Inicialización de los atributos
    PAG::Renderer* PAG::Renderer::instancia = nullptr;

    //Constructor por defecto
    Renderer::Renderer(){}

    //Destructor por defecto
    Renderer::~Renderer(){}


    //Métodos

    /**
     * Función para consultar el objeto único de la clase
     * @return La dirección de memoria del objeto
     */
    PAG::Renderer& PAG::Renderer::getInstancia ()
    {
        if ( !instancia ){ // Lazy initialization: si aún no existe, lo crea
            instancia = new Renderer ();
        }
        return *instancia;
    }

    /**
     * Función para establecer una referencia a las funciones OpenGL del driver gráfico
     *
     * @var ubicacionFunciones es un puntero a una función genérica. Después se castea a GLADloadproc
     */
    bool Renderer::inicializarGLAD(void* ubicacionFunciones)
    {
        return gladLoadGLLoader((GLADloadproc) ubicacionFunciones); //Casteo dentro
    }


    /**
     * Mostrar propiedades del contexto gráfico
     */
    void Renderer::mostrarPropiedadesContextoGrafico()
    {
        std::cout << "Grafica en uso: " << glGetString(GL_RENDERER) << std::endl
        << "Fabricante: " << glGetString(GL_VENDOR) << std::endl
        << "Version de OpenGL: " << glGetString(GL_VERSION) << std::endl
        << "OpenGL Shading Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    }


    /**
     * Función para activar la prueba de profundidad (Z-buffering). Esto
     * determina qué superficies son visibles y cuáles están ocultas
     */
    void Renderer::activarPruebaProfundidad()
    {
        glEnable(GL_DEPTH_TEST);
    }


    /**
     * Función OpenGL que devuelve el color del fondo
     */
    GLfloat* Renderer::getColorFondo ()
    {
        return colorFondo;
    }


    /**
     * Función OpenGL para refrescar la ventana (encapsula la parte de OpenGL)
     */
    void Renderer::refrescar ()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     //Limpieza del buffer back
    }

    /**
     * Función OpenGL para redimensionar ventana
     */
    void Renderer::redimensionar (int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    /**
     * Función OpenGL para cambiar el color del fondo
     */
    void Renderer::cambiarColorFondo (GLfloat r, GLfloat g, GLfloat b, GLfloat a)
    {
        glClearColor(r, g, b, a);
        colorFondo[0] = r;
        colorFondo[1] = g;
        colorFondo[2] = b;
        colorFondo[3] = a;
    }

} // PAG