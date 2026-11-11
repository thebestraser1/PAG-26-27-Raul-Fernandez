//
// Created by rferr on 11/11/2026.
//

#ifndef PRACTICA1_LUZ_H
#define PRACTICA1_LUZ_H

#include <bits/unique_ptr.h>      //Esto de los punteros inteligentes me ha servido para hacer el vector de aplicadores
#include <glm/vec3.hpp>
#include "ShaderProgram.h"

namespace PAG {

    //Propiedades de las luces
    struct PropiedadesLuz {
        glm::vec3 Ia;
        glm::vec3 Id;
        glm::vec3 Is;
        glm::vec3 p;
        glm::vec3 d;
        float gamma;
        float s;
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
        virtual void aplicarLuz(const PropiedadesLuz& p,ShaderProgram& sp) = 0;
    };



    class LuzAmbiental_Aplicador : public AplicadorLuz {
    public:
        void aplicarLuz(const PropiedadesLuz& p,ShaderProgram& sp) override;
    };



    class LuzPuntual_Aplicador : public AplicadorLuz {
    public:
        void aplicarLuz(const PropiedadesLuz& p,ShaderProgram& sp) override;
    };



    class LuzDireccional_Aplicador : public AplicadorLuz {
    public:
        void aplicarLuz(const PropiedadesLuz& p,ShaderProgram& sp) override;
    };



    class Foco_de_Luz_Aplicador : public AplicadorLuz {
    public:
        void aplicarLuz(const PropiedadesLuz& p,ShaderProgram& sp) override;
    };



    // CLASE BASE DE LUZ

    class Luz {
    private:
        TipoLuz _tipoLuz;
        bool _encendida;
        std::vector<std::unique_ptr<AplicadorLuz>> _aplicadores;

    public:
        PropiedadesLuz _propiedades;

    public:
        Luz(PropiedadesLuz p_iniciales, TipoLuz t_luz);

        void aplicarLuz(ShaderProgram *sp) const;
        void setTipoLuz(TipoLuz _tipoLuz);
        void encendida_apagada();
    };









} // PAG

#endif //PRACTICA1_LUZ_H
