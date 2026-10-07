#include "Renderer.h"

namespace PAG {
    //Inicialización de la instancia única a nulo
    PAG::Renderer *PAG::Renderer::instancia = nullptr;

    Renderer::Renderer() {
        //Inicizalización de variables
        shader_program = ShaderProgram();
        _camara = new Camara((float) anchoVentana, (float) altoVentana);
    }

    Renderer::~Renderer() {
        //Liberamos punteros
        delete _camara;
        _camara = nullptr;

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
     * Función OpenGL para refrescar la ventana (encapsula la parte de OpenGL)
     */
    void Renderer::refrescar() {
        glClearColor(_colorFondo[0], _colorFondo[1], _colorFondo[2], _colorFondo[3]);
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
        const std::string &nombre = shader_program.nombre_shader();

        if (nombre == "pag03") {
            //No tiene uniforms
        } else if (nombre == "pag05") {
            std::string nombreUniform = "matrizMVP";
            GLint posicion = glGetUniformLocation(idSP, nombreUniform.c_str());
            if (posicion != -1) {
                // El uniform existe y se ha podido localizar correctamente
                glm::mat4 matrizVP_camara = _camara->getMatVP();
                glUniformMatrix4fv(posicion, 1, GL_FALSE, &matrizVP_camara[0][0]);
            }
        } else {
            //Shader desconocido: no se envían uniforms
        }
    }

    /**
     * Función OpenGL para redimensionar ventana
     */
    void Renderer::redimensionar(int width, int height) {
        //Seteamos las variables globales del namespace
        anchoVentana = width;
        altoVentana = height;

        _camara->redimensionar((float) anchoVentana, (float) altoVentana);

        //Aplicamos el cambio a la ventana
        glViewport(0, 0, anchoVentana, altoVentana);
    }


    /**
     * Función OpenGL que devuelve el color del fondo
     */
    GLfloat *PAG::Renderer::getColorFondo() {
        return &_colorFondo[0];
    }

    /**
     * Función OpenGL que devuelve el color del fondo
     */
    void PAG::Renderer::setColorFondo(GLfloat colorFondo[4]) {
        _colorFondo[0] = colorFondo[0];
        _colorFondo[1] = colorFondo[1];
        _colorFondo[2] = colorFondo[2];
        _colorFondo[3] = colorFondo[3];
    }

    /**
    * Getter del ancho de ventana
    */
    int PAG::Renderer::ancho_ventana() const {
        return anchoVentana;
    }

    /**
     * Getter del alto de ventana
     */
    int PAG::Renderer::alto_ventana() const {
        return altoVentana;
    }


    /**
     * Getter de cámara del Renderer
     * @return Camara
     */
    Camara *PAG::Renderer::getCamara() const {
        return _camara;
    }


    /**
     * Se ejecuta cuando lo hace el callback de ratón (en un futuro se pasará por parámetro el movimiento relativo del ratón)
     */
    void PAG::Renderer::hacerMovimientoRaton() {
        if (_tipoMovimientoSeleccionado == TipoMovimiento::Zoom) {
            //Cogemos el ángulo de visión
            GLfloat anguloVision = _camara->getAnguloVision();
            anguloVision = anguloVision + 2.0f;

            //Actualizo la cámara
            _camara->mover(TipoMovimiento::Zoom, &anguloVision);
        }
    }


    /**
     * --------------------------------------
     *          PATRÓN OBSERVADOR
     * --------------------------------------
     */


    /**
     * Función para reaccionar ante peticiones de datos de las ventanas o cambios en las ventanas
     * @param t
     * @param ...
     */
    void Renderer::wakeUp(TipoVentana t, bool ventana_pidiendo, ...) {
        /**
         * ----------------------------------------------
         *          RENDERER ----> VENTANAS
         * ----------------------------------------------
         */

        if (ventana_pidiendo) {
            //Si la ventana está pidiendo atributos, según la que sea, se le trasfieren los datos necesarios
            //La ventana dará por parámetro los punteros que han de ser actualizados

            //En caso contrario, la ventana solo está avisando al Renderer de que algo cambió

            switch (t) {
                case TipoVentana::V_Selecc_Color_Fondo: {
                    //Pasa el color que tendrá el fondo
                    std::va_list args;
                    va_start(args, ventana_pidiendo);

                    GLfloat *colorFondoVentana = va_arg(args, GLfloat*);

                    colorFondoVentana[0] = _colorFondo[0];
                    colorFondoVentana[1] = _colorFondo[1];
                    colorFondoVentana[2] = _colorFondo[2];
                    colorFondoVentana[3] = _colorFondo[3];

                    va_end(args);

                    break;
                }
                case TipoVentana::V_Manejo_Camara: {
                    std::va_list args;
                    va_start(args, ventana_pidiendo);

                    GLfloat *angulo = va_arg(args, GLfloat*);

                    *angulo = _camara->getAnguloVision();

                    va_end(args);
                    break;
                }
                default: ;
            }

        /**
         * ----------------------------------------------
         *          VENTANAS ----> RENDERER
         * ----------------------------------------------
         */
        } else {
            switch (t) {
                case TipoVentana::V_Selecc_Color_Fondo: {
                    //Pasa el color que tendrá el fondo
                    std::va_list args;
                    va_start(args, ventana_pidiendo);

                    GLfloat *colorFondoVentana = va_arg(args, GLfloat*);

                    _colorFondo[0] = colorFondoVentana[0];
                    _colorFondo[1] = colorFondoVentana[1];
                    _colorFondo[2] = colorFondoVentana[2];
                    _colorFondo[3] = colorFondoVentana[3];

                    va_end(args);

                    break;
                }
                case TipoVentana::V_Texto_Shaders: {
                    //Pasará el nombre de los shaders
                    std::va_list args;
                    va_start(args, ventana_pidiendo);

                    std::string nombreShader(va_arg(args, char*));

                    //En el guión aparece vec3 de GLM. De momento lo dejo así para que no haya leak de memoria
                    if (!nombreShader.empty()) {
                        try {
                            shader_program.creaShaderProgram(nombreShader);
                        } catch (std::exception &e) {
                            std::cout << "\n--------------------\n" << "EXCEPCIÓN: " << e.what() <<
                                    "\n--------------------\n" << std::endl;
                        }
                    }

                    va_end(args);
                    break;
                }
                case TipoVentana::V_Manejo_Camara: {
                    std::va_list args;
                    va_start(args, ventana_pidiendo);

                    //Se setearía el tipo de movimiento seleccionado
                    _tipoMovimientoSeleccionado = *va_arg(args, TipoMovimiento*);

                    //Se actualizarían los parámetros de la cámara según el tipo
                    if (_tipoMovimientoSeleccionado == TipoMovimiento::Zoom) {
                        GLfloat *angulo = va_arg(args, GLfloat*);
                        _camara->mover(_tipoMovimientoSeleccionado, angulo);
                    }

                    va_end(args);
                    break;
                }
                default: ;
            }
        }
    }
} // PAG
