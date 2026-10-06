#include "Ventanas.h"


namespace PAG {
    /** --------------------------------------------------------
    *  INFO GLOBAL DE LAS VENTANAS (fruto de la clase abstracta)
    *  ---------------------------------------------------------
    */

    //Inicialización de la escala de todas las ventanas
    float PAG::Ventanas::_escalaTexto = 1.0;

    /**
     * Añadir observadores para eventos de ventanas (función con definición global)
     */
    void Ventanas::addListener(Listener *listener) {
        _listeners.push_back(listener);
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
    VentanaSelectorColorFondo::VentanaSelectorColorFondo(GLfloat colorInicial[4], float x,float y)
    : _colorFondoSeleccionado{colorInicial[0], colorInicial[1], colorInicial[2], colorInicial[3]}
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

            //Variable para comprobar si ha habido un cambio de color (para avisar a observadores)
            bool cambio_color = false;

            ImGui::Text("Selecciona un color:");
            float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.40f;
            if (ImGui::ColorPicker3("##Color de paleta", (float *) _colorFondoSeleccionado,
                                    ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview |
                                    ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha)) {
                cambio_color = true;
            }
            ImGui::SameLine(); //Esto hace que aparezcan en la misma línea
            ImGui::BeginGroup(); //Se crea un mismo grupo (para que esto aparezca en la misma línea)
            ImGui::Text("Color Actual");
            ImGui::ColorButton("##ActualColor", *(ImVec4 *) _colorFondoSeleccionado, ImGuiColorEditFlags_NoAlpha,
                               ImVec2(100, 50));
            ImGui::EndGroup();
            if (ImGui::ColorEdit4("HSV como RGB##1", (float *) _colorFondoSeleccionado,
                                  ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Float)) {
                cambio_color = true;
            }
            if (ImGui::ColorEdit4("HSV como HSV##1", (float *) _colorFondoSeleccionado,
                                  ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_InputHSV |
                                  ImGuiColorEditFlags_Float)) {
                cambio_color = true;
            }
            if (ImGui::ColorEdit4("Hexadecimal", (float *) _colorFondoSeleccionado,
                                  ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoSmallPreview)) {
                cambio_color = true;
            }

            if (cambio_color) {
                warn_listeners(); //Avisamos a observadores si el color cambió
            }
        }

        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End();
    }


    /**
     * Avisar a los observadores de un cambio en la ventana de selección de color
     */
    void VentanaSelectorColorFondo::warn_listeners() {
        for (Listener *listener: _listeners) {
            listener->wakeUp(TipoVentana::V_Selecc_Color_Fondo, true, _colorFondoSeleccionado);
        }
    }

    /**
     * Función para reaccionar ante cambios en el Renderer relativos al color de fondo
     * @param t
     * @param ...
     */
    void VentanaSelectorColorFondo::wakeUp(TipoVentana t, bool ventana_a_renderer, ...) {
        if (t == TipoVentana::V_Selecc_Color_Fondo && ventana_a_renderer == false) {
            std::va_list args;
            va_start(args, ventana_a_renderer);

            GLfloat* colorRenderer = va_arg(args, GLfloat*);

            _colorFondoSeleccionado[0] = colorRenderer[0];
            _colorFondoSeleccionado[1] = colorRenderer[1];
            _colorFondoSeleccionado[2] = colorRenderer[2];
            _colorFondoSeleccionado[3] = colorRenderer[3];

            va_end(args);
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
        for (Listener *listener: _listeners) {
            listener->wakeUp(TipoVentana::V_Texto_Shaders, true, _nombre.c_str());
        }
    }


    /** ------------------------------------------
     *  VENTANA DE MANEJO DE CÁMARA
     *  ------------------------------------------
     */

    /**
     * Constructor de ventana de manejo de cámara
     */
    VentanaCamara::VentanaCamara(float x, float y, Camara *cam) {
        this->pos_x = x;
        this->pos_y = y;
        this->_angulo = cam->getAnguloVision();
    }

    /**
     * Dibujar la ventana de manejo de cámara
     */
    void VentanaCamara::dibujar() {
        //Posición a dibujar
        ImGui::SetNextWindowPos(ImVec2(pos_x, pos_y), ImGuiCond_Once);

        bool ha_cambiado_angulo = false;

        if (ImGui::Begin("Manejador de cámara", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            // La ventana está desplegada

            ImGui::SetWindowFontScale(_escalaTexto); // Escalamos el texto si fuera necesario

            ImGui::Text("Zoom");
            ha_cambiado_angulo = ImGui::SliderFloat("##SliderZoom", &_angulo, 20, 120, "%2.2fº");
        }

        if (ha_cambiado_angulo) {
            warn_listeners(TipoMovimiento::Zoom);
        }


        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End();
    }


    /**
     * Avisar a los observadores de un cambio en la ventana de manejo de cámara
     */
    void VentanaCamara::warn_listeners(TipoMovimiento t_movimiento) const {
        if (t_movimiento == TipoMovimiento::Zoom) {
            for (Listener *listener: _listeners) {
                listener->wakeUp(TipoVentana::V_Manejo_Camara, true, &t_movimiento, &_angulo);
            }
        }
    }


    /**
     * Función para reaccionar ante cambios en el Renderer (relativos a la cámara)
     * @param t
     * @param ...
     */
    void VentanaCamara::wakeUp(TipoVentana t, bool ventana_a_renderer, ...) {
        if (t == TipoVentana::V_Manejo_Camara && ventana_a_renderer == false) {
            std::va_list args;
            va_start(args, ventana_a_renderer);

            TipoMovimiento tipo_movimiento_realizado = *va_arg(args, TipoMovimiento*);

            if (tipo_movimiento_realizado == TipoMovimiento::Zoom) {
                _angulo = *va_arg(args, GLfloat*);
            }
            va_end(args);
        }


        // Terminar cualquier otro procesamiento que sea necesario
    }
}
