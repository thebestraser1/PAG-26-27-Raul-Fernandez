#include "Ventanas.h"


namespace PAG {

    /** ---------------------
     *  VENTANA DE MENSAJES
     *  ---------------------
     */

    /**
     * Constructor de ventana de salida de mensajes
     */
    VentanaMensajes::VentanaMensajes(std::stringstream &textoInicial, float x, float y, float escala): textoSalida(textoInicial) {
        this->x = x;
        this->y = y;
        this->escala = escala;
    }

    /**
     * Dibujar la ventana de salida de mensajes de consola
     */
    void VentanaMensajes::dibujar() {

        //Posición a dibujar
        ImGui::SetNextWindowPos ( ImVec2 (x, y), ImGuiCond_Once );

        {
            if ( ImGui::Begin ( "Mensajes" ) ){ // La ventana está desplegada

                ImGui::SetWindowFontScale ( escala ); // Escalamos el texto si fuera necesario

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
    VentanaSelectorColor::VentanaSelectorColor(ImVec4 colorInicial, float x, float y, float escala): colorSeleccionado(colorInicial){
        this->x = x;
        this->y = y;
        this->escala = escala;
    }

    /**
     * Dibujar la ventana selección de color
     */
    void VentanaSelectorColor::dibujar() {

        //Posición a dibujar
        ImGui::SetNextWindowPos ( ImVec2 (x, y), ImGuiCond_Once );
        ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_Once);

        if ( ImGui::Begin ( "Selector Color" ) ){ // La ventana está desplegada

            ImGui::Text("MouseDown: %d",ImGui::IsMouseDown(ImGuiMouseButton_Left));


            ImGui::Text("Mouse pos: %.1f %.1f",
                        ImGui::GetIO().MousePos.x,
                        ImGui::GetIO().MousePos.y);

            ImGui::SetWindowFontScale ( 1.0f ); // Escalamos el texto si fuera necesario

            ImGui::Text("Selecciona un color:");
            float w = (ImGui::GetContentRegionAvail().x - ImGui::GetStyle().ItemSpacing.y) * 0.40f;
            ImGui::ColorPicker3("##MyColor##6", (float*)&colorSeleccionado, ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoAlpha);
        }

        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End ();
    }



}
