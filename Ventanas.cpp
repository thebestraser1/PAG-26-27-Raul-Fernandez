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
    void Ventanas::addListener ( Listener *listener )
    {
        _listeners.push_back ( listener );
    }



    /** ---------------------
     *  VENTANA DE MENSAJES
     *  ---------------------
     */

    /**
     * Constructor de ventana de salida de mensajes
     */
    PAG::VentanaMensajes::VentanaMensajes(std::stringstream &textoInicial, float x, float y): _textoSalida(textoInicial) {
        this->x = x;
        this->y = y;
    }

    /**
     * Dibujar la ventana de salida de mensajes de consola
     */
    void VentanaMensajes::dibujar() {

        //Posición a dibujar
        ImGui::SetNextWindowPos ( ImVec2 (x, y), ImGuiCond_Once );

        {
            if ( ImGui::Begin ( "Mensajes" ) ){ // La ventana está desplegada

                ImGui::SetWindowFontScale ( _escalaTexto ); // Escalamos el texto si fuera necesario

                //Pintamos el buffer de texto de salida
                ImGui::TextUnformatted(_textoSalida.str().c_str());
            }

            // Si la ventana no está desplegada, Begin devuelve false
            ImGui::End ();
        }
    }



    /** -----------------------------
     *  VENTANA DE SELECCIÓN DE COLOR
     *  -----------------------------
     */

    /**
     * Constructor de ventana de selección de color
     */
    VentanaSelectorColor::VentanaSelectorColor(GLfloat *colorInicial, float x, float y): _colorSeleccionado(colorInicial){
        this->x = x;
        this->y = y;
    }

    /**
     * Dibujar la ventana selección de color
     */
    void VentanaSelectorColor::dibujar() {

        //Posición a dibujar
        ImGui::SetNextWindowPos ( ImVec2 (x, y), ImGuiCond_Once );
        ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Once);

        if ( ImGui::Begin ( "Selector de Color" ) ){ // La ventana está desplegada

            ImGui::SetWindowFontScale ( _escalaTexto ); // Escalamos el texto si fuera necesario

            //Variable para comprobar si ha habido un cambio de color (para avisar a observadores)
            bool cambio_color = false;

            ImGui::Text("Selecciona un color:");
            float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.40f;
            if (ImGui::ColorPicker3("##Color de paleta", (float*)_colorSeleccionado, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha)) {
                cambio_color = true;
            }
            ImGui::SameLine();      //Esto hace que aparezcan en la misma línea
            ImGui::BeginGroup();    //Se crea un mismo grupo (para que esto aparezca en la misma línea)
            ImGui::Text("Color Actual");
            ImGui::ColorButton("##ActualColor", *(ImVec4*)_colorSeleccionado, ImGuiColorEditFlags_NoAlpha, ImVec2(100, 50));
            ImGui::EndGroup();
            if (ImGui::ColorEdit4("HSV como RGB##1", (float*)_colorSeleccionado, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Float)) {
                cambio_color=true;
            }
            if (ImGui::ColorEdit4("HSV como HSV##1", (float*)_colorSeleccionado, ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_InputHSV | ImGuiColorEditFlags_Float)) {
                cambio_color=true;
            }
            if (ImGui::ColorEdit4("Hexadecimal", (float*)_colorSeleccionado, ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoSmallPreview)) {
                cambio_color=true;
            }

            if (cambio_color) {
                warn_listeners();   //Avisamos a observadores si el color cambió
            }

        }

        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End ();
    }


    /**
     * Avisar a los observadores de un cambio en la ventana de selección de color
     */
    void VentanaSelectorColor::warn_listeners()
    {
        for (Listener* listener : _listeners) {
            listener->wakeUp(TipoVentana::V_Selecc_Color, _colorSeleccionado);
        }
    }





    /** -----------------------------
    *  VENTANA DE SELECCIÓN DE ESCALA
    *  -----------------------------
    */

    /**
     * Constructor de ventana de selección de escala
     */
    VentanaSelectorEscala::VentanaSelectorEscala(float x, float y){
        this->x = x;
        this->y = y;
    }

    /**
     * Dibujar la ventana selección de Escala
     */
    void VentanaSelectorEscala::dibujar() {

        //Posición a dibujar
        ImGui::SetNextWindowPos ( ImVec2 (x, y), ImGuiCond_Once );

        if ( ImGui::Begin ( "Selector de Escala" ) ){ // La ventana está desplegada

            ImGui::SetWindowFontScale ( _escalaTexto ); // Escalamos el texto si fuera necesario

            ImGui::DragFloat("Escala de fuente (0-4)", &_escalaTexto, 0.005f, 0.0f, 4.0f, "%.3f");

        }

        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End ();
    }



}
