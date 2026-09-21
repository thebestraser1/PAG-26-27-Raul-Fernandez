#include "Ventanas.h"


namespace PAG {

    /** ---------------------
     *  VENTANA DE MENSAJES
     *  ---------------------
     */

    /**
     * Constructor de ventana de salida de mensajes
     */
    VentanaMensajes::VentanaMensajes(std::stringstream &textoInicial, float x, float y, float *escalaTexto): textoSalida(textoInicial) {
        this->x = x;
        this->y = y;
        this->escalaTexto = escalaTexto;
    }

    /**
     * Dibujar la ventana de salida de mensajes de consola
     */
    void VentanaMensajes::dibujar() {

        //Posición a dibujar
        ImGui::SetNextWindowPos ( ImVec2 (x, y), ImGuiCond_Once );

        {
            if ( ImGui::Begin ( "Mensajes" ) ){ // La ventana está desplegada

                ImGui::SetWindowFontScale ( *escalaTexto ); // Escalamos el texto si fuera necesario

                //Pintamos el buffer de texto de salida
                ImGui::TextUnformatted(textoSalida.str().c_str());
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
    VentanaSelectorColor::VentanaSelectorColor(GLfloat *colorInicial, float x, float y, float *escalaTexto): colorSeleccionado(colorInicial){
        this->x = x;
        this->y = y;
        this->escalaTexto = escalaTexto;
    }

    /**
     * Dibujar la ventana selección de color
     */
    void VentanaSelectorColor::dibujar() {

        //Posición a dibujar
        ImGui::SetNextWindowPos ( ImVec2 (x, y), ImGuiCond_Once );
        ImGui::SetNextWindowSize(ImVec2(400, 400), ImGuiCond_Once);

        if ( ImGui::Begin ( "Selector de Color" ) ){ // La ventana está desplegada

            ImGui::SetWindowFontScale ( *escalaTexto ); // Escalamos el texto si fuera necesario

            ImGui::Text("Selecciona un color:");
            float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.40f;
            ImGui::ColorPicker3("##Color de paleta", (float*)colorSeleccionado, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
            ImGui::SameLine();      //Esto hace que aparezcan en la misma línea
            ImGui::BeginGroup();    //Se crea un mismo grupo (para que esto aparezca en la misma línea)
            ImGui::Text("Color Actual");
            ImGui::ColorButton("##ActualColor", *(ImVec4*)colorSeleccionado, ImGuiColorEditFlags_NoAlpha, ImVec2(100, 50));
            ImGui::EndGroup();
            ImGui::ColorEdit4("HSV como RGB##1", (float*)colorSeleccionado, ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Float);
            ImGui::ColorEdit4("HSV como HSV##1", (float*)colorSeleccionado, ImGuiColorEditFlags_DisplayHSV | ImGuiColorEditFlags_InputHSV | ImGuiColorEditFlags_Float);
            ImGui::ColorEdit4("Hexadecimal", (float*)colorSeleccionado, ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_NoSmallPreview);

        }

        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End ();
    }


    /** -----------------------------
    *  VENTANA DE SELECCIÓN DE ESCALA
    *  -----------------------------
    */

    /**
     * Constructor de ventana de selección de escala
     */
    VentanaSelectorEscala::VentanaSelectorEscala(float *escalaSliderInicial, float x, float y, float *escalaTexto): escalaSlider(escalaSliderInicial){
        this->x = x;
        this->y = y;
        this->escalaTexto = escalaTexto;
    }

    /**
     * Dibujar la ventana selección de Escala
     */
    void VentanaSelectorEscala::dibujar() {

        //Posición a dibujar
        ImGui::SetNextWindowPos ( ImVec2 (x, y), ImGuiCond_Once );

        if ( ImGui::Begin ( "Selector de Escala" ) ){ // La ventana está desplegada

            ImGui::SetWindowFontScale ( *escalaTexto ); // Escalamos el texto si fuera necesario

            ImGui::DragFloat("Escala de fuente (0-4)", escalaSlider, 0.005f, 0.0f, 4.0f, "%.3f");

        }

        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End ();
    }



}
