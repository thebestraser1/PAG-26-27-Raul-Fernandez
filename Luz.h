//
// Created by rferr on 11/11/2026.
//

#ifndef PRACTICA1_LUZ_H
#define PRACTICA1_LUZ_H

#include <bits/unique_ptr.h>      //Esto de los punteros inteligentes me ha servido para hacer el vector de aplicadores
#include <glm/vec3.hpp>
#include "ShaderProgram.h"
#include <glm/trigonometric.hpp>
#include <glm/glm.hpp>

namespace PAG {

    //Acciones sobre las luces
    enum TipoAccionLuz {
        Anadir_luz,
        Encender_apagar,
        Borrar_luz,
    };

    //Propiedades de las luces
    struct PropiedadesLuz {
        glm::vec3 Ia = glm::vec3(0.0f);
        glm::vec3 Id = glm::vec3(0.0f);
        glm::vec3 Is = glm::vec3(0.0f);
        glm::vec3 p = glm::vec3(0.0f);
        glm::vec3 d = glm::vec3(0.0f, 0.0f, 0.0f);
        float gamma = 0.0f;     //ESTO VA EN GRADOS
    };

    //Tipos de luz
    enum TipoLuz {
        Luz_Ambiental,
        Luz_Puntual,
        Luz_Direccional,
        Foco_de_Luz,
    };
    constexpr int NUM_TIPOS_LUZ = 4;


    //Aplicación distinta de las luces según su tipo (patrón estrategia)

    class AplicadorLuz {
    public:
        AplicadorLuz () = default;
        virtual ~AplicadorLuz() = default;
        virtual void aplicarLuz(const PropiedadesLuz& p,ShaderProgram& sp, glm::mat4 matrizV) = 0;
    };



    class LuzAmbiental_Aplicador : public AplicadorLuz {
    public:
        void aplicarLuz(const PropiedadesLuz& p,ShaderProgram& sp, glm::mat4 matrizV) override;
    };



    class LuzPuntual_Aplicador : public AplicadorLuz {
    public:
        void aplicarLuz(const PropiedadesLuz& p,ShaderProgram& sp, glm::mat4 matrizV) override;
    };



    class LuzDireccional_Aplicador : public AplicadorLuz {
    public:
        void aplicarLuz(const PropiedadesLuz& p,ShaderProgram& sp, glm::mat4 matrizV) override;
    };



    class Foco_de_Luz_Aplicador : public AplicadorLuz {
    public:
        void aplicarLuz(const PropiedadesLuz& p,ShaderProgram& sp, glm::mat4 matrizV) override;
    };



    // CLASE BASE DE LUZ

    class Luz {
    private:
        std::vector<std::unique_ptr<AplicadorLuz>> _aplicadores;

    public:
        std::string _nombre;
        TipoLuz _tipoLuz;
        PropiedadesLuz _propiedades;
        bool _encendida;

    public:
        Luz(const std::string &nombre, PropiedadesLuz p_iniciales, TipoLuz t_luz);

        void aplicarLuz(ShaderProgram *sp, glm::mat4 matrizV) const;
        void encendida_apagada();
    };









} // PAG

#endif //PRACTICA1_LUZ_H
