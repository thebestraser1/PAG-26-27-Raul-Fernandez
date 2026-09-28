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

        //Liberamos recursos del Shader Program y el modelo
        if (idVS != 0) {
            glDeleteShader(idVS);
        }
        if (idFS != 0) {
            glDeleteShader(idFS);
        }
        if (idSP != 0) {
            glDeleteProgram(idSP);
        }
        if (idVBO != 0) {
            glDeleteBuffers(1, &idVBO);
        }
        if (idIBO != 0) {
            glDeleteBuffers(1, &idIBO);
        }
        if (idVAO != 0) {
            glDeleteVertexArrays(1, &idVAO);
        }
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
     * Cargar fichero de shaders
     */
    std::string PAG::Renderer::cargarFichero(const std::string& ruta) {
        std::ifstream archivoShader;
        archivoShader.open(ruta);
        if (!archivoShader.is_open()) {
            /* Error abriendo el archivo.  TODO*/
        }

        std::stringstream streamShader;
        streamShader << archivoShader.rdbuf();
        std::string codigoFuenteShader = streamShader.str();
        archivoShader.close();

        return codigoFuenteShader;
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
    void Renderer::inicializarOpenGL() {
        glClearColor(_colorFondo[0], _colorFondo[1], _colorFondo[2], _colorFondo[3]);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
    }


    /**
     * Función para crear, compilar y enlazar el shader program
     * @note No se incluye ninguna comprobación de errores
     */
    void PAG::Renderer::creaShaderProgram() {
        std::string miVertexShader = cargarFichero("pag03-vs.glsl");

        std::string miFragmentShader = cargarFichero("pag03-fs.glsl");

        idVS = glCreateShader(GL_VERTEX_SHADER);
        const GLchar *fuenteVS = miVertexShader.c_str();
        glShaderSource(idVS, 1, &fuenteVS, nullptr);
        glCompileShader(idVS);

        idFS = glCreateShader(GL_FRAGMENT_SHADER);
        const GLchar *fuenteFS = miFragmentShader.c_str();
        glShaderSource(idFS, 1, &fuenteFS, nullptr);
        glCompileShader(idFS);

        idSP = glCreateProgram();
        glAttachShader(idSP, idVS);
        glAttachShader(idSP, idFS);
        glLinkProgram(idSP);
    }


    /**
     * Función para crear el VAO para el modelo a renderizar
     * @note No se incluye ninguna comprobación de errores
     */
    void PAG::Renderer::creaModelo() {
        GLfloat vertices[] = {
            -.5, -.5, 0,
            .5, -.5, 0,
            .0, .5, 0
        };
        GLuint indices[] = {0, 1, 2};

        //Generación y activación del VAO
        glGenVertexArrays(1, &idVAO);
        glBindVertexArray(idVAO);

        //Creación del VBO
        glGenBuffers(1, &idVBO);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(0); //Colocamos posición de vértices como atributo 0

        glGenBuffers(1, &idIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLuint), indices, GL_STATIC_DRAW);
    }


    /**
     * Función OpenGL que devuelve el color del fondo
     */
    GLfloat *Renderer::getColorFondo() const {
        return this->_colorFondo;
    }


    /**
     * Función OpenGL para refrescar la ventana (encapsula la parte de OpenGL)
     */
    void Renderer::refrescar() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Limpia el buffer actual

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUseProgram(idSP);
        glBindVertexArray(idVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    }

    /**
     * Función OpenGL para redimensionar ventana
     */
    void Renderer::redimensionar(int width, int height) {
        glViewport(0, 0, width, height);
    }


    void Renderer::wakeUp(TipoVentana t, ...) {
        switch (t) {
            case TipoVentana::V_Selecc_Color_Fondo: {
                //Podría pasar un color, pero en realidad ya está cambiando _colorFondo por puntero
                glClearColor(_colorFondo[0], _colorFondo[1], _colorFondo[2], _colorFondo[3]);
                break;
            }
            default: ;
        }
        // Terminar cualquier otro procesamiento que sea necesario
    }
} // PAG
