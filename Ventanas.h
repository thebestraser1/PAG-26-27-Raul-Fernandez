//
// Created by rferr on 21/09/2026.
//

#ifndef PRACTICA1_VENTANA_H
#define PRACTICA1_VENTANA_H

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <sstream>

namespace PAG {
    /**------------------------------------------------------------------------
     * Clase abstracta para establecer una jerarquía entre el tipo de ventanas
     */
    class Ventanas {
    public:
        virtual ~Ventanas() = default;

        virtual void dibujar() = 0;     //Indico que es un virtual puro (se ha de sobre-escribir esta función)
    };


    /** -----------------------------------------------------
     * Ventana que muestra los mensajes de LOG por la ventana
     */
    class VentanaMensajes : public Ventanas{
    private:
        std::stringstream &textoSalida;     //Importante por referencia para que se vaya actualizando
    public:
        VentanaMensajes(std::stringstream &textoInicial): textoSalida(textoInicial){}

        void dibujar() override{
            if ( ImGui::Begin ( "Mensajes" ) ){ // La ventana está desplegada

                ImGui::SetWindowFontScale ( 1.0f ); // Escalamos el texto si fuera necesario

                //Pintamos el buffer de texto de salida
                ImGui::TextUnformatted(textoSalida.str().c_str());
            }

            // Si la ventana no está desplegada, Begin devuelve false
            ImGui::End ();
        }
    };











} // PAG

#endif //PRACTICA1_VENTANA_H
