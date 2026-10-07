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
            }


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
        
        GLfloat angulo = 0.0;
        
        //Recuperamos las variables necesarias actualizadas de Renderer (si existe)
        if (_renderer_listener) {
            _renderer_listener->wakeUp(TipoVentana::V_Manejo_Camara, true, &angulo);
        }
        

        bool ha_cambiado_angulo = false;

        if (ImGui::Begin("Manejador de cámara", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            // La ventana está desplegada

            ImGui::SetWindowFontScale(_escalaTexto); // Escalamos el texto si fuera necesario

            ImGui::Text("Zoom");
            ha_cambiado_angulo = ImGui::SliderFloat("##SliderZoom", &angulo, 20, 120, "%2.2fº");
        }

        if (ha_cambiado_angulo) {
            warn_listeners(TipoMovimiento::Zoom, angulo);
        }


        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End();
    }


    /**
     * Avisar a los observadores de un cambio en la ventana de manejo de cámara
     */
    void VentanaCamara::warn_listeners(TipoMovimiento t_movimiento, GLfloat angulo) const {
        if (t_movimiento == TipoMovimiento::Zoom) {
            if (_renderer_listener) {
                _renderer_listener->wakeUp(TipoVentana::V_Manejo_Camara, false, &t_movimiento, &angulo);
            }
        }
    }
}
