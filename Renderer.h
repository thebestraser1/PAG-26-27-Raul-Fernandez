#ifndef PRACTICA1_RENDERER_H
#define PRACTICA1_RENDERER_H

#include <iostream>
#include <cstdarg>  //Para funciones con número variable de elementos

//Para aleatorios
#include <random>

#include "Camara.h"
#include "Modelo.h"
#include "Listener.h"
#include "Material.h"
#include "Ventanas.h"



/**
* Espacio de nombres para las prácticas de Programación de Aplicaciones
* Gráficas
*/
namespace PAG {


    /**
     * @brief Clase encargada de encapsular la gestión del área de dibujo
     * OpenGL
     *
     * Esta clase coordina el renderizado de las escenas OpenGL. Se implementa
     * aplicando el patrón de diseño Singleton. Está pensada para que las
     * funciones callback hagan llamadas a sus métodos
     */
    class Renderer : public Listener{
    private:
        static Renderer *instancia;

        GLfloat _colorFondo[4] = {0.6f, 0.6f, 0.6f, 1.0f};

        //Modelos
        std::vector<Modelo*> _modelos;
        std::vector<Material*> _materiales;
        ModoVisualizacion _modoVisualizacion = MODELOS_RELLENOS;    //RELLENOS = 1, ALAMBRE = 0
        std::mt19937 _genRandomPos;                                 //Generador aleatorios de posiciones

        //Aspecto de ventana
        int anchoVentana = 1280;
        int altoVentana = 720;

        //Camara
        Camara* _camara;

        //Ventanas que escuchan a lo que cambie en el Renderer
        std::vector<Listener*> _listeners;

        Renderer(); //Constructor privado (Singletone)

    public:
        ~Renderer() override;

        static Renderer &getInstancia();

        bool inicializarGLAD(void* ubicacionFunciones);

        void mostrarPropiedadesContextoGrafico();

        void inicializarOpenGL();

        void refrescar();

        void wakeUp(TipoVentana t, bool ventana_pidiendo ...) override;

        void redimensionar(int width, int height);

        GLfloat* getColorFondo();

        void setColorFondo(GLfloat colorFondo[4]);

        int ancho_ventana() const;

        int alto_ventana() const;

        Camara* getCamara() const;

        void hacerMovimientoRaton(double movimiento_relativo_x, double movimiento_relativo_y);

    private:
        void controlar_uniforms(Modelo *modelo_usado) const;
        bool comprobar_modelos_mismo_nombre(const std::string& nombreModelo);
        bool comprobar_materiales_mismo_nombre(const std::string &nombreModelo);
        void borrar_modelo_por_nombre(const std::string& nombreModelo);
        Modelo* obtener_modelo_por_nombre(const std::string &nombreModelo) const;
        void desasignarMaterialModelos(const std::string &nombreMaterial) const;
        void borrar_material_por_nombre(const std::string &nombreMaterial);
        Material* obtener_material_por_nombre(const std::string &nombreMaterial) const;

    };
} // PAG

#endif //PRACTICA1_RENDERER_H
