#include "Ventanas.h"


namespace PAG {
    /** --------------------------------------------------------
    *  INFO GLOBAL DE LAS VENTANAS (fruto de la clase abstracta)
    *  ---------------------------------------------------------
    */

    //Inicialización de la escala de todas las ventanas
    float PAG::Ventanas::_escalaTexto = 1.0;

    /**
     * Se añade un observador (renderer) para eventos de ventanas
     */
    void Ventanas::addListener(Listener *listener) {
        _renderer_listener = listener;
    }


    /** ---------------------
     *  VENTANA DE MENSAJES
     *  ---------------------
     */

    /**
     * Constructor de ventana de salida de mensajes
     */
    PAG::VentanaMensajes::VentanaMensajes(std::stringstream &textoInicial, float x, float y) : _textoSalida(
        textoInicial) {
        this->pos_x = x;
        this->pos_y = y;
    }

    /**
     * Dibujar la ventana de salida de mensajes de consola
     */
    void VentanaMensajes::dibujar() {
        //Posición a dibujar
        ImGui::SetNextWindowPos(ImVec2(pos_x, pos_y), ImGuiCond_Once);

        {
            if (ImGui::Begin("Mensajes")) {
                // La ventana está desplegada (aquí no he puesto redimensión automática)

                ImGui::SetWindowFontScale(_escalaTexto); // Escalamos el texto si fuera necesario

                //Pintamos el buffer de texto de salida
                ImGui::TextUnformatted(_textoSalida.str().c_str());
            }

            // Si la ventana no está desplegada, Begin devuelve false
            ImGui::End();
        }
    }


    /** -----------------------------
     *  VENTANA DE SELECCIÓN DE COLOR
     *  -----------------------------
     */

    /**
     * Constructor de ventana de selección de color
     */
    VentanaSelectorColorFondo::VentanaSelectorColorFondo(float x,float y)
    {
        this->pos_x = x;
        this->pos_y = y;
    }

    /**
     * Dibujar la ventana selección de color
     */
    void VentanaSelectorColorFondo::dibujar() {
        //Posición a dibujar
        ImGui::SetNextWindowPos(ImVec2(pos_x, pos_y), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Once);

        if (ImGui::Begin("Selector de Color", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            // La ventana está desplegada

            ImGui::SetWindowFontScale(_escalaTexto); // Escalamos el texto si fuera necesario

            //Se coge la variable de color del renderer
            GLfloat colorFondoSeleccionado[4] = {0,0,0,0};

            if (_renderer_listener) {
                _renderer_listener->wakeUp(TipoVentana::V_Selecc_Color_Fondo, true, colorFondoSeleccionado);

                //Variable para comprobar si ha habido un cambio de color (para avisar a observadores)
                bool cambio_color = false;

                ImGui::Text("Selecciona un color:");
                float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.40f;
                if (ImGui::ColorPicker3("##Color de paleta", (float *) colorFondoSeleccionado,
                                        ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview |
                                        ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha)) {
                    cambio_color = true;
                }
                ImGui::SameLine(); //Esto hace que aparezcan en la misma línea
                ImGui::BeginGroup(); //Se crea un mismo grupo (para que esto aparezca en la misma línea)
                ImGui::Text("Color Actual");
                ImGui::ColorButton("##ActualColor", *(ImVec4 *) colorFondoSeleccionado, ImGuiColorEditFlags_NoAlpha,
                                   ImVec2(100, 50));
                ImGui::EndGroup();
                if (ImGui::ColorEdit4("HSV como RGB##1", (float *) colorFondoSeleccionado,
                                      ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Float)) {
                    cambio_color = true;
                                      }
                if (ImGui::ColorEdit4("HSV como HSV##1", (float *) colorFondoSeleccionado,
                                      ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_InputHSV |
                                      ImGuiColorEditFlags_Float)) {
                    cambio_color = true;
                                      }
                if (ImGui::ColorEdit4("Hexadecimal", (float *) colorFondoSeleccionado,
                                      ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoSmallPreview)) {
                    cambio_color = true;
                                      }

                if (cambio_color) {
                    warn_listeners(colorFondoSeleccionado); //Avisamos a observadores si el color cambió
                }
            }

            // Si la ventana no está desplegada, Begin devuelve false
            ImGui::End();
        }
    }


    /**
     * Avisar a los observadores de un cambio en la ventana de selección de color
     */
    void VentanaSelectorColorFondo::warn_listeners(GLfloat* colorFondoSeleccionado) {
        if (_renderer_listener) {
            _renderer_listener->wakeUp(TipoVentana::V_Selecc_Color_Fondo, false, colorFondoSeleccionado);
        }
    }



    /** -----------------------------
    *  VENTANA DE SELECCIÓN DE ESCALA
    *  -----------------------------
    */

    /**
     * Constructor de ventana de selección de escala
     */
    VentanaSelectorEscala::VentanaSelectorEscala(float x, float y) {
        this->pos_x = x;
        this->pos_y = y;
    }

    /**
     * Dibujar la ventana selección de Escala
     */
    void VentanaSelectorEscala::dibujar() {
        //Posición a dibujar
        ImGui::SetNextWindowPos(ImVec2(pos_x, pos_y), ImGuiCond_Once);

        if (ImGui::Begin("Selector de Escala", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            // La ventana está desplegada

            ImVec2 posActual = ImGui::GetWindowPos();

            ImGui::SetWindowFontScale(_escalaTexto); // Escalamos el texto si fuera necesario

            ImGui::Text("Escala de fuente");
            ImGui::SliderFloat("##Escala de fuente", &_escalaTexto, 0.5f, 2.5f);
        }

        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End();
    }


    /** ------------------------------------------
     *  VENTANA DE SELECCIÓN DE SHADER POR TEXTO
     *  ------------------------------------------
     */

    /**
     * Constructor de ventana de carga de shader
     */
    VentanaCargaModeloYShader::VentanaCargaModeloYShader(float x, float y) {
        this->pos_x = x;
        this->pos_y = y;

        std::string carpetaModelos = "modelos";
        
        //Buscador de modelos
        _file_browser = ImGui::FileBrowser();
        _file_browser.SetPwd(std::filesystem::current_path() / carpetaModelos);
        _file_browser.SetTitle("Buscador de modelos .obj");
        _file_browser.SetTypeFilters({".obj"});
    }

    /**
     * Dibujar la ventana de carga de shader
     */
    void VentanaCargaModeloYShader::dibujar() {
        //Posición a dibujar
        ImGui::SetNextWindowPos(ImVec2(pos_x, pos_y), ImGuiCond_Once);


        bool _botonCargaPresionado = false;

        if (ImGui::Begin("Cargador de Modelo y Shader", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            // La ventana está desplegada

            ImVec2 posActual = ImGui::GetWindowPos();

            ImGui::SetWindowFontScale(_escalaTexto); // Escalamos el texto si fuera necesario

            if (ImGui::Button("Buscar Modelo")) {
                _file_browser.Open();
            }

            if (!_rutaModelo.empty()) {
                ImGui::Text("Ruta de fichero:");
                _nombreModelo = _rutaModelo.substr(_rutaModelo.find_last_of("\\/") + 1);
                ImGui::TextDisabled(_nombreModelo.c_str());
            }

            ImGui::Text("Nombre de fichero de shader:");
            ImGui::InputText("##TextoShader", &_nombreShader, ImGuiInputTextFlags_AutoSelectAll);
            _botonCargaPresionado = ImGui::Button("Cargar");
        }

        if (_botonCargaPresionado) {
            std::cout << "Modelo a cargar: " << _rutaModelo << std::endl;
            std::cout << "Shader a cargar: " << _nombreShader << std::endl;
            warn_listeners();
        }

        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End();

        _file_browser.Display();

        if(_file_browser.HasSelected())
        {
            _rutaModelo = _file_browser.GetSelected().string();
            _file_browser.ClearSelected();
            _file_browser.Close();
        }
    }


    /**
     * Avisar a los observadores de un cambio en la ventana de selección de shader
     */
    void VentanaCargaModeloYShader::warn_listeners() const {
        if (_renderer_listener) {
            _renderer_listener->wakeUp(TipoVentana::V_Texto_Shaders, false, _nombreModelo.c_str(), _rutaModelo.c_str(), _nombreShader.c_str());
        }
    }


    /** ------------------------------------------
     *  VENTANA DE MANEJO DE CÁMARA
     *  ------------------------------------------
     */

    const float VentanaCamara::_lim_inf_zoom = 20;
    const float VentanaCamara::_lim_sup_zoom = 120;

    /**
     * Constructor de ventana de manejo de cámara
     */
    VentanaCamara::VentanaCamara(float x, float y) {
        this->pos_x = x;
        this->pos_y = y;
    }

    /**
     * Dibujar la ventana de manejo de cámara
     */
    void VentanaCamara::dibujar() {
        //Posición a dibujar
        ImGui::SetNextWindowPos(ImVec2(pos_x, pos_y), ImGuiCond_Once);

        //Modificadores fijos
        const GLfloat variacion_Pan = 2.0;  //Angulo de variación
        const GLfloat variacion_Tilt = 2.0;  //Angulo de variación
        const GLfloat traslacion_X = 0.1;  //Variación de traslación (X)
        const GLfloat traslacion_Y = 0.1;  //Variación de traslación (Y)
        const GLfloat traslacion_Z = 0.1;  //Variación de traslación (Z)
        const GLfloat variacion_Orbit = 2.0; //Angulo de variacion


        if (_renderer_listener) {

            //Recuperamos las variables necesarias para las ventanas desde el Rende
            //rer (actualizadas)
            TipoMovimiento *tipo_movimiento_camara = nullptr;
            GLfloat angulo_Zoom = 0.0;

            _renderer_listener->wakeUp(TipoVentana::V_Manejo_Camara, true, &angulo_Zoom, &tipo_movimiento_camara);

            if (ImGui::Begin("Manejador de cámara", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
                // La ventana está desplegada

                ImGui::SetWindowFontScale(_escalaTexto); // Escalamos el texto si fuera necesario

                ImGui::Text("Movimiento");

                //Este vector funciona porque he puesto el mismo orden que en el enum de Camara.h
                //Es para que muestre el nombre de los enumerados
                const char* movimientos[] = { "Zoom", "Pan", "Tilt", "Dolly", "Crane", "Orbit"};

                //Sacamos el índice que ocupa el tipo de movimiento actual en el enumerado gracias a static_cast
                int movimientoActual = static_cast<int>(*tipo_movimiento_camara);

                //La ejecución entra aquí solo cuando se ha cambiado el tipo de movimiento
                if (ImGui::Combo("##Movimiento", &movimientoActual, movimientos, IM_ARRAYSIZE(movimientos))) {

                    //Se puede hacer la operación inversa a lo anterior (sacar un tipo de movimiento desde un índice (int))
                    *tipo_movimiento_camara = static_cast<TipoMovimiento>(movimientoActual);
                }

                switch (*tipo_movimiento_camara) {
                    case TipoMovimiento::Zoom: {
                        ImGui::Text("Ángulo");
                        bool ha_cambiado_angulo = false;
                        ha_cambiado_angulo = ImGui::SliderFloat("##SliderZoom", &angulo_Zoom, _lim_inf_zoom, _lim_sup_zoom, "%2.2fº");

                        if (ha_cambiado_angulo) {
                            _renderer_listener->wakeUp(TipoVentana::V_Manejo_Camara, false, &angulo_Zoom);
                        }
                        break;
                    }
                    case TipoMovimiento::Pan: {
                        ImGui::Text("Dirección");
                        if (ImGui::Button("<- Izquierda", ImVec2(100, 0))) {
                            // Acción al pulsar Izquierda
                            _renderer_listener->wakeUp(TipoVentana::V_Manejo_Camara, false, &variacion_Pan);
                        }

                        ImGui::SameLine();

                        if (ImGui::Button("Derecha ->", ImVec2(100, 0))) {
                            // Acción al pulsar Derecha
                            GLfloat variacion_Pan_der = -variacion_Pan;
                            _renderer_listener->wakeUp(TipoVentana::V_Manejo_Camara, false, &variacion_Pan_der);
                        }
                        break;
                    }
                    case TipoMovimiento::Tilt: {
                        ImGui::Text("Dirección");
                        if (ImGui::Button("^ Arriba ^", ImVec2(100, 0))) {
                            // Acción al pulsar Arriba
                            _renderer_listener->wakeUp(TipoVentana::V_Manejo_Camara, false, &variacion_Tilt);
                        }

                        ImGui::SameLine();

                        if (ImGui::Button("v Abajo v", ImVec2(100, 0))) {
                            // Acción al pulsar Abajo
                            GLfloat variacion_Tilt_der = -variacion_Tilt;
                            _renderer_listener->wakeUp(TipoVentana::V_Manejo_Camara, false, &variacion_Tilt_der);
                        }
                        break;
                    }

                    case TipoMovimiento::Dolly: {
                        ImGui::Text("Dirección");
                        if (ImGui::Button("^ Adelante ^", ImVec2(208, 0))) {
                            GLfloat variacion_X_adelante = 0;
                            GLfloat variacion_Z_adelante = -traslacion_Z;
                            _renderer_listener->wakeUp(TipoVentana::V_Manejo_Camara, false, &variacion_X_adelante, &variacion_Z_adelante);
                        }


                        if (ImGui::Button("<- Izquierda", ImVec2(100, 0))) {
                            GLfloat variacion_X_izquierda = -traslacion_X;
                            GLfloat variacion_Z_izquierda = 0;
                            _renderer_listener->wakeUp(TipoVentana::V_Manejo_Camara, false, &variacion_X_izquierda, &variacion_Z_izquierda);
                        }

                        ImGui::SameLine();

                        if (ImGui::Button("Derecha ->", ImVec2(100, 0))) {
                            GLfloat variacion_X_derecha = traslacion_X;
                            GLfloat variacion_Z_derecha = 0;
                            _renderer_listener->wakeUp(TipoVentana::V_Manejo_Camara, false, &variacion_X_derecha, &variacion_Z_derecha);
                        }

                        if (ImGui::Button("v Atrás v", ImVec2(208, 0))) {
                            GLfloat variacion_X_atras = 0;
                            GLfloat variacion_Z_atras = traslacion_Z;
                            _renderer_listener->wakeUp(TipoVentana::V_Manejo_Camara, false, &variacion_X_atras, &variacion_Z_atras);
                        }

                        break;
                    }

                    case TipoMovimiento::Crane: {
                        ImGui::Text("Dirección");
                        if (ImGui::Button("^ Arriba ^", ImVec2(100, 0))) {

                            GLfloat variacion_Y_arriba = traslacion_Y;
                            _renderer_listener->wakeUp(TipoVentana::V_Manejo_Camara, false, &variacion_Y_arriba);
                        }

                        ImGui::SameLine();

                        if (ImGui::Button("v Abajo v", ImVec2(100, 0))) {

                            GLfloat variacion_Y_abajo = -traslacion_Y;
                            _renderer_listener->wakeUp(TipoVentana::V_Manejo_Camara, false, &variacion_Y_abajo);
                        }
                        break;
                    }
                    case TipoMovimiento::Orbit: {
                        ImGui::Text("Latitud");
                        if (ImGui::Button("^ Norte ^", ImVec2(100, 0))) {
                            GLfloat variacion_Norte_longitud = 0;
                            GLfloat variacion_Norte_latitud = variacion_Orbit;
                            _renderer_listener->wakeUp(TipoVentana::V_Manejo_Camara, false, &variacion_Norte_longitud, &variacion_Norte_latitud);
                        }

                        ImGui::SameLine();

                        if (ImGui::Button("v Sur v", ImVec2(100, 0))) {
                            GLfloat variacion_Sur_longitud = 0;
                            GLfloat variacion_Sur_latitud = -variacion_Orbit;
                            _renderer_listener->wakeUp(TipoVentana::V_Manejo_Camara, false, &variacion_Sur_longitud, &variacion_Sur_latitud);
                        }

                        ImGui::Text("Longitud");
                        if (ImGui::Button("^ Oeste ^", ImVec2(100, 0))) {
                            GLfloat variacion_Oeste_longitud = -variacion_Orbit;
                            GLfloat variacion_Oeste_latitud = 0;
                            _renderer_listener->wakeUp(TipoVentana::V_Manejo_Camara, false, &variacion_Oeste_longitud, &variacion_Oeste_latitud);
                        }

                        ImGui::SameLine();

                        if (ImGui::Button("v Este v", ImVec2(100, 0))) {
                            GLfloat variacion_Este_longitud = variacion_Orbit;
                            GLfloat variacion_Este_latitud = 0;
                            _renderer_listener->wakeUp(TipoVentana::V_Manejo_Camara, false, &variacion_Este_longitud, &variacion_Este_latitud);
                        }
                        break;
                    }

                    default: ;
                }

            }


            // Si la ventana no está desplegada, Begin devuelve false
            ImGui::End();
        }
    }



    /** ------------------------------------------
     *  VENTANA DE TRASFORMACIONES DE MODELOS
     *  ------------------------------------------
     */


    /**
     * Constructor de ventana de manejo de cámara
     */
    VentanaTrasformacionesModelos::VentanaTrasformacionesModelos(float x, float y) {
        this->pos_x = x;
        this->pos_y = y;
    }

    /**
     * Dibujar la ventana de manejo de cámara
     */
    void VentanaTrasformacionesModelos::dibujar() {
        //Posición a dibujar
        ImGui::SetNextWindowPos(ImVec2(pos_x, pos_y), ImGuiCond_Once);

        if (_renderer_listener) {

            //Recuperamos los modelos que tenga el Renderer (solo sus nombres, ya que no van repes)
            std::vector<const char*> nombreModelos;

            _renderer_listener->wakeUp(TipoVentana::V_Trasformaciones_Modelos, true, &nombreModelos);

            //Buscamos para el nombre seleccionado qué índice en el vector tiene
            for (int i=0; i<nombreModelos.size(); i++) {
                if (nombreModelos[i] == _nombreModeloSeleccionado) {
                    _indiceModeloSeleccionado = i;
                }
            }

            if (ImGui::Begin("Transformaciones de modelos", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
                // La ventana está desplegada

                ImGui::SetWindowFontScale(_escalaTexto); // Escalamos el texto si fuera necesario

                if (!nombreModelos.empty()) {
                    ImGui::Text("Modelo seleccionado:               ");

                    //La ejecución entra aquí solo cuando se ha cambiado el tipo de movimiento
                    if (ImGui::Combo("##Modelo seleccionado", &_indiceModeloSeleccionado, nombreModelos.data(), (int)nombreModelos.size())) {

                        //Al hacer click en una de las opciones, su índice hace referencia al nombre de modelo seleccionado
                        _nombreModeloSeleccionado = nombreModelos[_indiceModeloSeleccionado];
                    }

                    PAG::TipoAccion operacion_a_realizar;

                    if (ImGui::Button("Borrar Modelo")) {
                        operacion_a_realizar = PAG::TipoAccion::Borrar;
                        _renderer_listener->wakeUp(TipoVentana::V_Trasformaciones_Modelos, false, &operacion_a_realizar, &_nombreModeloSeleccionado);
                    }
                }else {
                    ImGui::Text("          Carga algún modelo           ");
                }




            }

            // Si la ventana no está desplegada, Begin devuelve false
            ImGui::End();
        }



    }
}
