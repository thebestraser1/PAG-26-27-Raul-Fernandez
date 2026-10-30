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
#include "imfilebrowser.h"

namespace PAG {

    /**
     * Clase abstracta para establecer una jerarquía entre el tipo de ventanas
     */
    class Ventanas{
    protected:
        float pos_x = 10;                             //Posiciones x,y de las ventanas
        float pos_y = 10;
        bool minimizada = true;                          //Ventana aparece minimizada??
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
        VentanaMensajes(std::stringstream &textoInicial, float x, float y, bool minimizada);
        void dibujar() override;
    };


    /**
     * Ventana que muestra un selector de color para cambiar el fondo de la aplicación
     */
    class VentanaSelectorColorFondo : public Ventanas{
    public:
        VentanaSelectorColorFondo(float x, float y, bool minimizada);
        void dibujar() override;
        void warn_listeners(GLfloat* colorFondoSeleccionado);
    };


    /**
     * Ventana que muestra un selector de escala para el tamaño de fuente de las ventanas
     */
    class VentanaSelectorEscala : public Ventanas{
    public:
        VentanaSelectorEscala(float x, float y, bool minimizada);
        void dibujar() override;
    };


    /**
     * Ventana en la que se inserta texto para cargar el shader correspondiented¡
     */
    class VentanaCargaModeloYShader : public Ventanas{
    private:
        ImGui::FileBrowser _file_browser;
        std::string _nombreModelo;
        std::string _rutaModelo;
        std::string _nombreShader;
    public:
        VentanaCargaModeloYShader(float x, float y, bool minimizada);
        void dibujar() override;

        void warn_listeners() const;
    };


    /**
     * Ventana de manejo de la cámara
     */
    class VentanaCamara : public Ventanas{
    public:
        static const float _lim_inf_zoom;
        static const float _lim_sup_zoom;

        VentanaCamara(float x, float y, bool minimizada);
        void dibujar() override;
    };


    /**
     * Ventana de manejo de transformaciones de modelos
     */
    class VentanaTrasformacionesModelos : public Ventanas{
        std::string _nombreModeloSeleccionado;
        int _indiceModeloSeleccionado = -1;   //Indice equivalente al vector de modelos de Renderer.cpp

        //--------------------------
        //Operaciones con el modelo
        //--------------------------
        PAG::TipoAccionModelo _operacion_a_realizar = PAG::TipoAccionModelo::Traslacion_modelo;

        //Traslación
        GLfloat trasl_x_relativa = 0.0;
        GLfloat trasl_y_relativa = 0.0;
        GLfloat trasl_z_relativa = 0.0;

        //Rotación
        GLfloat rot_x = 0.0;
        GLfloat rot_y = 0.0;
        GLfloat rot_z = 0.0;

        //Escalado
        GLfloat escala = 1.0;

    public:
        VentanaTrasformacionesModelos(float x, float y, bool minimizada);
        void dibujar() override;
    };



    /**
     * Ventana de gestión de materiales (tiene operaciones CRUD)
     */
    class VentanaGestionMateriales : public Ventanas{

        TipoAccionMaterial _accion_actual = TipoAccionMaterial::Anadir_material;

        //Atributos al añadir material nuevo
        std::string _nombre_material_anadir;

        glm::vec3 _colorAmbiental_anadir = glm::vec3(0.0f,0.0f, 0.0f);
        glm::vec3 _colorDifuso_anadir = glm::vec3(0.0f,0.0f, 0.0f);
        glm::vec3 _colorEspecular_anadir = glm::vec3(0.0f,0.0f, 0.0f);

        GLfloat _brillo_anadir = 1.0f;

        //Atributos para modificar material
        std::string _nombreMaterialSeleccionado;
        int _indiceMaterialSeleccionado = -1;   //Indice equivalente al vector de materiales de Renderer.cpp

        //Atributos para asignar a un modelo
        std::string _nombreModeloSeleccionado;
        int _indiceModeloSeleccionado = -1;   //Indice equivalente al vector de modelos de Renderer.cpp

        public:
            VentanaGestionMateriales(float x, float y, bool minimizada);
            void dibujar() override;
    };

    /**
     * Ventana para visualizar los modelos como sólidos o como malla de alambre con color fijo.
     */
    class VentanaVisualizModelos : public Ventanas{

    public:
        VentanaVisualizModelos(float x, float y, bool minimizada);
        void dibujar() override;
    };
} // PAG

#endif //PRACTICA1_VENTANA_H
