#ifndef PRACTICA1_VENTANA_H
#define PRACTICA1_VENTANA_H

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <iostream>
#include <imgui_stdlib.h>
#include <sstream>
#include <vector>
#include "Renderer.h"

namespace PAG {

    /**
     * Clase abstracta para establecer una jerarquía entre el tipo de ventanas
     */
    class Ventanas{
    protected:
        float pos_x = 10;                       //Posiciones x,y de las ventanas
        float pos_y = 10;
        static float _escalaTexto;                    //Compartida por todas las ventanas (para mantener consistencia)
        Listener* _renderer_listener = nullptr;       //Observadores que se suscriben a los cambios producidos en las ventanas
    public:
        virtual ~Ventanas() = default;
        void addListener ( Listener *listener );
        virtual void dibujar() = 0;     //Indico que es un virtual puro (se ha de sobre-escribir esta función)
    };


    /**
     * Ventana que muestra los mensajes de LOG por la ventana
     */
    class VentanaMensajes : public Ventanas{
    private:
        std::stringstream &_textoSalida;     //Importante por referencia para que se vaya actualizando (viene de main.cpp)
    public:
        VentanaMensajes(std::stringstream &textoInicial, float x, float y);
        void dibujar() override;
    };


    /**
     * Ventana que muestra un selector de color para cambiar el fondo de la aplicación
     */
    class VentanaSelectorColorFondo : public Ventanas{
    public:
        VentanaSelectorColorFondo(float x, float y);
        void dibujar() override;
        void warn_listeners(GLfloat* colorFondoSeleccionado);
    };


    /**
     * Ventana que muestra un selector de escala para el tamaño de fuente de las ventanas
     */
    class VentanaSelectorEscala : public Ventanas{
    public:
        VentanaSelectorEscala(float x, float y);
        void dibujar() override;
    };


    /**
     * Ventana en la que se inserta texto para cargar el shader correspondiented¡
     */
    class VentanaTextoShader : public Ventanas{
    private:
        std::string _nombre;
    public:
        VentanaTextoShader(float x, float y);
        void dibujar() override;

        void warn_listeners() const;
    };


    /**
     * Ventana de manejo de la cámara
     */
    class VentanaCamara : public Ventanas{
    public:
        VentanaCamara(float x, float y);
        void dibujar() override;
        void warn_listeners(TipoMovimiento t_movimiento, GLfloat angulo) const;
    };
} // PAG

#endif //PRACTICA1_VENTANA_H
