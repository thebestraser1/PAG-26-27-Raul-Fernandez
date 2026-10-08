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
    VentanaTextoShader::VentanaTextoShader(float x, float y) {
        this->pos_x = x;
        this->pos_y = y;
    }

    /**
     * Dibujar la ventana de carga de shader
     */
    void VentanaTextoShader::dibujar() {
        //Posición a dibujar
        ImGui::SetNextWindowPos(ImVec2(pos_x, pos_y), ImGuiCond_Once);

        bool _buttonPressed = false;

        if (ImGui::Begin("Selector de Shader", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            // La ventana está desplegada

            ImVec2 posActual = ImGui::GetWindowPos();

            ImGui::SetWindowFontScale(_escalaTexto); // Escalamos el texto si fuera necesario

            ImGui::Text("Nombre de fichero:");
            ImGui::InputText("##", &_nombre, ImGuiInputTextFlags_AutoSelectAll);
            _buttonPressed = ImGui::Button("Cargar");
        }

        if (_buttonPressed) {
            std::cout << "Shader a cargar: " << _nombre << std::endl;
            warn_listeners();
        }

        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End();
    }


    /**
     * Avisar a los observadores de un cambio en la ventana de selección de shader
     */
    void VentanaTextoShader::warn_listeners() const {
        if (_renderer_listener) {
            _renderer_listener->wakeUp(TipoVentana::V_Texto_Shaders, false, _nombre.c_str());
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


        if (_renderer_listener) {

            //Recuperamos las variables necesarias para las ventanas desde el Renderer (actualizadas)
            TipoMovimiento *tipo_movimiento_camara = nullptr;
            GLfloat angulo_Zoom = 0.0;

            _renderer_listener->wakeUp(TipoVentana::V_Manejo_Camara, true, &angulo_Zoom, &tipo_movimiento_camara);

            bool ha_cambiado_angulo = false;

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
                    }
                    default: ;
                }

            }




            // Si la ventana no está desplegada, Begin devuelve false
            ImGui::End();
        }
    }
}
