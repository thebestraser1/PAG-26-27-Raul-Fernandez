#include "Renderer.h"

namespace PAG {
    //Inicialización de la instancia única a nulo
    PAG::Renderer *PAG::Renderer::instancia = nullptr;

    Renderer::Renderer() {
        //Inicizalización de variables
        _colorFondo = new GLfloat[4]{0.6f, 0.6f, 0.6f, 1.0f};
        shader_program = ShaderProgram();
        _camara = Camara();
    }

    Renderer::~Renderer() {
        //Liberamos punteros
        delete[] _colorFondo;
        _colorFondo = nullptr;

        //Liberamos recursos del modelo
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
     * Función para crear el VAO para el modelo a renderizar
     * @note No se incluye ninguna comprobación de errores
     */
    void PAG::Renderer::creaModelo() {
        GLfloat vertices[] = {
            //Creación del VBO de manera ENTRELAZADA (posición, color)
            -.5, -.5, 0, 1.0, 0.4, 0.2,
            .5, -.5, 0, 0.2, 1.0, 0.4,
            .0, .5, 0, 0.4, 0.2, 1.0
        };
        GLuint indices[] = {0, 1, 2};

        //Generación y activación del VAO
        glGenVertexArrays(1, &idVAO);
        glBindVertexArray(idVAO);

        //Creación de un ÚNICO VBO de posiciones y color de los vértices
        glGenBuffers(1, &idVBO);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        //Datos de posiciones (location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(0); //Colocamos posición de vértices como atributo 0

        //Datos de color (location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), ((GLfloat *) NULL + (3)));
        glEnableVertexAttribArray(1); //Colocamos color de vértices como atributo 1

        //Creación del VBO de colores de los vértices de manera NO ENTRELAZADA
        //--------------------------------------------------------------------
        /*
        GLfloat colores[] = {
            1.0, 0.4, 0.2,
            0.2, 1.0, 0.4,
            0.4, 0.2, 1.0
        };
        glGenBuffers(1, &idVBO);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO);
        glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(colores), colores, GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(1); //Colocamos color de vértices como atributo 1
        */

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

        //Si no hay shader aún cargado, no cargar ninguno
        int idSP = shader_program.id_sp();

        if (idSP != 0) {
            glUseProgram(shader_program.id_sp());
            controlarUniforms(idSP);
            glBindVertexArray(idVAO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        }
    }

    /**
     * Función encargada de otorgar los uniforms que necesite el shader program de cada práctica en concreto
     */
    void Renderer::controlarUniforms(int idSP) {
        const std::string& nombre = shader_program.nombre_shader();

        if (nombre == "pag03") {
            //No tiene uniforms
        }
        else if (nombre == "pag05") {
            std::string nombreUniform = "matrizMVP";
            GLint posicion = glGetUniformLocation ( idSP, nombreUniform.c_str () );
            if ( posicion != -1 ){  // El uniform existe y se ha podido localizar correctamente
                glm::mat4 matrizVP_camara = _camara.getMatVP();
                glUniformMatrix4fv ( posicion, 1, GL_FALSE, &matrizVP_camara[0][0]);}
        }
        else {
            //Shader desconocido: no se envían uniforms
        }
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
            case TipoVentana::V_Texto_Shaders: {
                //Pasará el nombre de los shaders
                std::va_list args;
                va_start(args, t);
                std::string nombreShader(va_arg(args, char*));

                //En el guión aparece vec3 de GLM. De momento lo dejo así para que no haya leak de memoria
                if (!nombreShader.empty()) {
                    try {
                        shader_program.creaShaderProgram(nombreShader);
                    } catch (std::exception &e) {
                        std::cout << "\n--------------------\n" << "EXCEPCIÓN: " << e.what() << "\n--------------------\n" << std::endl;
                    }
                }
                va_end(args);
            }
            default: ;
        }


            // Terminar cualquier otro procesamiento que sea necesario
        }
    } // PAG
