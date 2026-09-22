#ifndef PRACTICA1_VENTANA_H
#define PRACTICA1_VENTANA_H

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <sstream>
#include <vector>
#include "Listener.h"
#include <GL/gl.h>

namespace PAG {

    /**
     * Clase abstracta para establecer una jerarquía entre el tipo de ventanas
     */
    class Ventanas {
    protected:
        float x = 10;                       //Posiciones x,y de las ventanas
        float y = 10;
        static float _escalaTexto;          //Compartida por todas las ventanas (para mantener consistencia)
        std::vector<Listener*> _listeners;  //Observadores que se suscriben a los cambios producidos en las ventanas
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
        std::stringstream &_textoSalida;     //Importante por referencia para que se vaya actualizando
    public:
        VentanaMensajes(std::stringstream &textoInicial, float x, float y);
        void dibujar() override;
    };


    /**
     * Ventana que muestra un selector de color para cambiar el fondo de la aplicación
     */
    class VentanaSelectorColor : public Ventanas{
    private:
        GLfloat *_colorSeleccionado;
    public:
        VentanaSelectorColor(GLfloat *colorInicial, float x, float y);
        void dibujar() override;
        void warn_listeners();
    };


    /**
     * Ventana que muestra un selector de escala para el tamaño de fuente de las ventanas
     */
    class VentanaSelectorEscala : public Ventanas{
    private:
    public:
        VentanaSelectorEscala(float x, float y);
        void dibujar() override;
    };
} // PAG

#endif //PRACTICA1_VENTANA_H
