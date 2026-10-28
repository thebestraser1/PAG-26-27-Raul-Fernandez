#include "Renderer.h"


namespace PAG {
    //Inicialización de la instancia única a nulo
    PAG::Renderer *PAG::Renderer::instancia = nullptr;

    Renderer::Renderer() {
        //Para valores aleatorios:
        std::random_device rd;
        _genRandomPos = std::mt19937(rd());

        //Inicizalización de variables
        _camara = new Camara((float) anchoVentana, (float) altoVentana);
    }

    Renderer::~Renderer() {
        //Liberamos punteros
        delete _camara;
        _camara = nullptr;

        if (!_modelos.empty()) {
            for (Modelo *modelo: _modelos) {
                delete modelo;
                modelo = nullptr;
            }
            _modelos.clear();
        }

        if (!_materiales.empty()) {
            for (Material *material: _materiales) {
                delete material;
                material = nullptr;
            }
            _materiales.clear();
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
     * Función OpenGL para refrescar la ventana (encapsula la parte de OpenGL)
     */
    void Renderer::refrescar() {
        glClearColor(_colorFondo[0], _colorFondo[1], _colorFondo[2], _colorFondo[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Limpia el buffer actual

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        if (!_modelos.empty()) {
            for (Modelo *modelo: _modelos) {
                glUseProgram(modelo->getShaderModelo()->id_sp());
                controlar_uniforms(modelo);

                modelo->Dibujar();
            }
        }
    }

    /**
     * Función encargada de otorgar los uniforms que necesite el shader program de cada práctica en concreto
     */
    void Renderer::controlar_uniforms(Modelo* modelo_usado) const {
        if (modelo_usado->getShaderModelo()->nombre_shader() == "pag03") {
            //No tiene uniforms

        } else if (modelo_usado->getShaderModelo()->nombre_shader() == "pag05") {
            //Uniforms de cámara
            std::string nombreUniform = "matrizMVP";
            GLint posicion = glGetUniformLocation(modelo_usado->getShaderModelo()->id_sp(), nombreUniform.c_str());

            if (posicion != -1) {
                // El uniform existe y se ha podido localizar correctamente

                // En este caso, componemos la matriz de modelado, visión y proyección
                glm::mat4 matrizVP_camara = _camara->getMatVP();
                glm::mat4 matrizM_modelo = modelo_usado->getMatrizModelado();

                glm::mat4 matrizMVP = matrizVP_camara * matrizM_modelo;
                glUniformMatrix4fv(posicion, 1, GL_FALSE, &matrizMVP[0][0]);
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
    void PAG::Renderer::hacerMovimientoRaton(double movimiento_relativo_x, double movimiento_relativo_y) {
        switch (*_camara->getTipoMovimientoActual()) {
            //------------------------------
            case TipoMovimiento::Zoom: {
                //------------------------------

                //Cogemos el ángulo de visión
                GLfloat anguloVision = _camara->getAnguloVision();
                anguloVision = anguloVision + movimiento_relativo_y / 2;

                //Controlo que el ángulo no se escape de los límites (sabiendo los límites que tiene la ventana que lleva esto)
                float lim_sup = VentanaCamara::_lim_sup_zoom;
                float lim_inf = VentanaCamara::_lim_inf_zoom;

                //Si excede los límites deshacemos la transformación que se quiere hacer
                anguloVision = (anguloVision < lim_inf) ? lim_inf : anguloVision;
                anguloVision = (anguloVision > lim_sup) ? lim_sup : anguloVision;

                //Actualizo la cámara
                _camara->mover(TipoMovimiento::Zoom, &anguloVision);
                break;
            }

            //------------------------------
            case TipoMovimiento::Pan: {
                //------------------------------

                //Directamente se mueve la cámara con el movimiento relativo de X
                GLfloat variacion = movimiento_relativo_x / 2;
                _camara->mover(TipoMovimiento::Pan, &variacion);
                break;
            }

            //------------------------------
            case TipoMovimiento::Tilt: {
                //------------------------------

                //Directamente se mueve la cámara con el movimiento relativo de X
                GLfloat variacion = movimiento_relativo_y / 2;
                _camara->mover(TipoMovimiento::Tilt, &variacion);
                break;
            }

            //------------------------------
            case TipoMovimiento::Dolly: {
                //------------------------------

                //Directamente se mueve la cámara con el movimiento relativo de X o Z
                GLfloat variacion_x = -(movimiento_relativo_x / 50);
                GLfloat variacion_z = -(movimiento_relativo_y / 50);
                _camara->mover(TipoMovimiento::Dolly, &variacion_x, &variacion_z);
                break;
            }

            //------------------------------
            case TipoMovimiento::Crane: {
                //------------------------------

                //Directamente se mueve la cámara con el movimiento relativo de X o Z
                GLfloat variacion_y = -(movimiento_relativo_y / 50);
                _camara->mover(TipoMovimiento::Crane, &variacion_y);
                break;
            }

            //------------------------------
            case TipoMovimiento::Orbit: {
                //------------------------------

                //Directamente se mueve la cámara con el movimiento relativo de X o Z
                GLfloat variacion_y = movimiento_relativo_y / 2;
                GLfloat variacion_x = movimiento_relativo_x / 2;
                _camara->mover(TipoMovimiento::Orbit, &variacion_x, &variacion_y);
                break;
            }

            default: ;
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

                    //Ventana necesita una dirección de memoria por referencia, no un valor (porque luego podrá modificar
                    //el tipo de movimiento de la cámara mediante el desplegable)
                    TipoMovimiento **t_movimiento = va_arg(args, TipoMovimiento**);

                    *angulo = _camara->getAnguloVision();
                    *t_movimiento = _camara->getTipoMovimientoActual();

                    va_end(args);
                    break;
                }
                case TipoVentana::V_Trasformaciones_Modelos: {
                    std::va_list args;
                    va_start(args, ventana_pidiendo);

                    std::vector<const char*>* nombresModelos = va_arg(args, std::vector<const char*>*);

                    nombresModelos->clear();
                    for (Modelo* modelo : _modelos) {
                        nombresModelos->push_back(modelo->_nombreModelo.c_str());
                    }

                    va_end(args);
                    break;
                }

                case TipoVentana::V_Gestion_Materiales: {
                    std::va_list args;
                    va_start(args, ventana_pidiendo);

                    std::vector<Material*>* materiales = va_arg(args, std::vector<Material*>*);

                    for (Material* material : _materiales) {
                        materiales->push_back(material);
                    }

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
                case TipoVentana::V_Carga_Modelo_Shaders: {
                    //Pasará el nombre de los shaders
                    std::va_list args;
                    va_start(args, ventana_pidiendo);

                    std::string nombreModelo(va_arg(args, char*));
                    std::string rutaModelo(va_arg(args, char*));
                    std::string nombreShader(va_arg(args, char*));

                    if (!nombreShader.empty() && !rutaModelo.empty()) {
                        if (comprobar_modelos_mismo_nombre(nombreModelo)) {
                            try {
                                //Cargamos el modelo en una posición aleatoria (si no hay uno que tenga ya el mismo nombre)
                                // Distribución entre -2 y 2
                                std::uniform_real_distribution<float> distribucion(-1.0f, 1.0f);

                                // Generar coordenadas aleatorias
                                float x = distribucion(_genRandomPos);
                                float y = distribucion(_genRandomPos);
                                float z = distribucion(_genRandomPos);

                                Modelo *modelo = new Modelo(nombreModelo, rutaModelo.c_str(), nombreShader, {x,y,z});

                                //Una vez se ha cargado bien, lo añadimos al vector de modelos
                                _modelos.push_back(modelo);
                            } catch (std::exception &e) {
                                std::cout << "--------------------\n" << "EXCEPCIÓN: " << e.what() <<
                                        "\n--------------------\n" << std::endl;
                            }
                        } else {
                            std::cout << "--------------------\n" << "EXCEPCIÓN: " <<
                                    "Ya hay un modelo con el nombre: " << nombreModelo <<
                                    "\n--------------------\n" << std::endl;
                        }
                    } else {
                        std::cout << "--------------------\n" << "EXCEPCIÓN: " <<
                                "No se ha insertado nombre de shader"
                                "o nombre de modelo" <<
                                "\n--------------------\n" << std::endl;
                    }

                    va_end(args);
                    break;
                }
                case TipoVentana::V_Manejo_Camara: {
                    std::va_list args;
                    va_start(args, ventana_pidiendo);

                    //Se actualizarían los parámetros de la cámara según el tipo
                    switch (*_camara->getTipoMovimientoActual()) {
                        case(TipoMovimiento::Zoom): {
                            GLfloat *angulo = va_arg(args, GLfloat*);
                            _camara->mover(TipoMovimiento::Zoom, angulo);
                            break;
                        }
                        case(TipoMovimiento::Pan): {
                            GLfloat *variacion = va_arg(args, GLfloat*);
                            _camara->mover(TipoMovimiento::Pan, variacion);
                            break;
                        }
                        case(TipoMovimiento::Tilt): {
                            GLfloat *variacion = va_arg(args, GLfloat*);
                            _camara->mover(TipoMovimiento::Tilt, variacion);
                            break;
                        }
                        case(TipoMovimiento::Dolly): {
                            GLfloat *variacion_x = va_arg(args, GLfloat*);
                            GLfloat *variacion_z = va_arg(args, GLfloat*);
                            _camara->mover(TipoMovimiento::Dolly, variacion_x, variacion_z);
                            break;
                        }
                        case(TipoMovimiento::Crane): {
                            GLfloat *variacion_y = va_arg(args, GLfloat*);
                            _camara->mover(TipoMovimiento::Crane, variacion_y);
                            break;
                        }
                        case(TipoMovimiento::Orbit): {
                            GLfloat *variacion_latitud = va_arg(args, GLfloat*);
                            GLfloat *variacion_longitud = va_arg(args, GLfloat*);
                            _camara->mover(TipoMovimiento::Orbit, variacion_latitud, variacion_longitud);
                            break;
                        }
                    }

                    va_end(args);
                    break;
                }
                case TipoVentana::V_Trasformaciones_Modelos: {
                    std::va_list args;
                    va_start(args, ventana_pidiendo);

                    /**
                     *  La ventana pasa (TipoAccion, nombreModelo, y params necesarios)
                     */

                    PAG::TipoAccionModelo* tipo_accion = va_arg(args, PAG::TipoAccionModelo*);
                    std::string * nombreModelo = va_arg(args, std::string*);

                    //Se actualizarían los parámetros de la cámara según el tipo
                    if (*tipo_accion == TipoAccionModelo::Borrar_modelo) {
                        borrar_modelo_por_nombre(*nombreModelo);
                    }else {
                        //Es una transformación (basta con multiplicar la matriz)
                        glm::mat4* transformacion = va_arg(args, glm::mat4*);
                        transformar_modelo_por_nombre(*nombreModelo, *tipo_accion, *transformacion);
                    }
                    break;
                }
                case TipoVentana::V_Gestion_Materiales: {
                    std::va_list args;
                    va_start(args, ventana_pidiendo);

                    PAG::TipoAccionMaterial* tipo_accion = va_arg(args, PAG::TipoAccionMaterial*);

                    switch (*tipo_accion) {
                        case (TipoAccionMaterial::Anadir_material): {
                            //Atributos de material
                            std::string nombreMaterial(va_arg(args, char*));
                            glm::vec3* colorAmbiental = va_arg(args, glm::vec3*);
                            glm::vec3* colorDifuso = va_arg(args, glm::vec3*);
                            glm::vec3* colorEspecular = va_arg(args, glm::vec3*);
                            GLfloat* brillo = va_arg(args, GLfloat*);


                            //Añadimos el material (si el nombre no existe)
                            if (comprobar_materiales_mismo_nombre(nombreMaterial)) {
                                _materiales.push_back(new Material(nombreMaterial, *colorAmbiental, *colorDifuso, *colorEspecular, *brillo));
                            }else {
                                std::cout << "--------------------\n" << "EXCEPCIÓN: " <<
                                        "Ya hay un material con el nombre: " << nombreMaterial <<
                                        "\n--------------------\n" << std::endl;
                            }
                            break;
                        }
                        case TipoAccionMaterial::Borrar_material:
                            {}
                            break;
                        case TipoAccionMaterial::Asignar_material:
                            {}
                            break;
                    }


                    va_end(args);
                    break;
                }
                default: ;
            }
        }
    }


    /**
     * Función para comprobar si ya hay un modelo con un nombre determinado
     */
    bool Renderer::comprobar_modelos_mismo_nombre(const std::string& nombreModelo) {
        for (Modelo *modelo: _modelos) {
            if (modelo->_nombreModelo == nombreModelo) {
                return false;
            }
        }
        return true;
    }

    /**
     * Función para comprobar si ya hay un modelo con un nombre determinado
     */
    bool Renderer::comprobar_materiales_mismo_nombre(const std::string& nombreModelo) {
        for (Material *material: _materiales) {
            if (material->nombre == nombreModelo) {
                return false;
            }
        }
        return true;
    }

    /**
     * Función para borrar un modelo por nombre
     */
    void Renderer::borrar_modelo_por_nombre(const std::string& nombreModelo) {
        for (int i=0; i<_modelos.size(); i++) {
            if (_modelos[i]->_nombreModelo == nombreModelo) {

                std::cout << "Se ha borrado " << _modelos[i]->_nombreModelo << std::endl;

                delete _modelos[i];
                _modelos[i] = nullptr;
                _modelos.erase(_modelos.begin() + i);
                return;
            }
        }
    }


    /**
     * Función para transformar un modelo por nombre
     */
    void Renderer::transformar_modelo_por_nombre(const std::string &nombreModelo, TipoAccionModelo tipoTransformacion, const glm::mat4 &transformacion) {
        for (int i=0; i<_modelos.size(); i++) {
            if (_modelos[i]->_nombreModelo == nombreModelo) {

                std::cout << "Se ha modificado " << _modelos[i]->_nombreModelo << std::endl;

                _modelos[i]->aplicarTransformacion(tipoTransformacion, transformacion);
            }
        }
    }


    /**
     * Función para obtener un material por nombre
     */
    Material* Renderer::obtener_material_por_nombre(const std::string &nombreMaterial) {
        for (Material* material : _materiales) {
            if (material->nombre == nombreMaterial) {

                return material;
            }
        }
        return nullptr;
    }
} // PAG
