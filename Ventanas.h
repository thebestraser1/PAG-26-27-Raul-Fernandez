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

    /**
     * Clase abstracta para establecer una jerarquía entre el tipo de ventanas
     */
    class Ventanas {
    protected:
        float x = 10;
        float y = 10;
        float escala = 1.0;
    public:
        virtual ~Ventanas() = default;
        virtual void dibujar() = 0;     //Indico que es un virtual puro (se ha de sobre-escribir esta función)
    };


    /**
     * Ventana que muestra los mensajes de LOG por la ventana
     */
    class VentanaMensajes : public Ventanas{
    private:
        std::stringstream &textoSalida;     //Importante por referencia para que se vaya actualizando
    public:
        VentanaMensajes(std::stringstream &textoInicial, float x, float y, float escala=1.0);
        void dibujar() override;
    };


    /**
     * Ventana que muestra un selector de color para el fondo de la aplicación
     */
    class VentanaSelectorColor : public Ventanas{
    private:
        ImVec4 *colorSeleccionado;
    public:
        VentanaSelectorColor(ImVec4 *colorInicial, float x, float y, float escala = 1.0);
        void dibujar() override;
    };
} // PAG

#endif //PRACTICA1_VENTANA_H
