//
// Created by rferr on 11/11/2026.
//

#include "Luz.h"

namespace PAG {

    PAG::Luz::Luz(const std::string &nombre, PropiedadesLuz p_iniciales, TipoLuz t_luz) :
        _nombre(nombre),
        _tipoLuz(t_luz),
        _encendida(true),
        _propiedades(p_iniciales){

        //Creamos el vector con los distintos "tipos de luces que se pueden invocar"
        //El tipo de luz determinará el índice que quiero invocar en este vector

        _aplicadores.push_back(std::make_unique<LuzAmbiental_Aplicador>());
        _aplicadores.push_back(std::make_unique<LuzPuntual_Aplicador>());
        _aplicadores.push_back(std::make_unique<LuzDireccional_Aplicador>());
        _aplicadores.push_back(std::make_unique<Foco_de_Luz_Aplicador>());

    }

    /**
     * Función para aplicar la luz independientemente de su tipo
     * @param sp
     */
    void PAG::Luz::aplicarLuz(ShaderProgram* sp, glm::mat4 matrizV) const {

        PropiedadesLuz propiedades_a_aplicar = PropiedadesLuz();
        if (!_encendida) {

            //Apagamos las intensidades
            propiedades_a_aplicar.Ia = glm::vec3(0.0f);
            propiedades_a_aplicar.Id = glm::vec3(0.0f);
            propiedades_a_aplicar.Is = glm::vec3(0.0f);

            //El resto queda igual (no importa vaya)
            propiedades_a_aplicar.p = _propiedades.p;
            propiedades_a_aplicar.d = _propiedades.d;
            propiedades_a_aplicar.gamma = _propiedades.gamma;
        }else {
            propiedades_a_aplicar = _propiedades;
        }

        int indice_tipo_luz = static_cast<int>(_tipoLuz);

        _aplicadores[indice_tipo_luz]->aplicarLuz(propiedades_a_aplicar, *sp, matrizV);
    }


    /**
     * Función para encender y apagar la luz
     */
    void PAG::Luz::encendida_apagada() {
        _encendida = !_encendida;
    }



    /**
     * Función auxiliar de este CPP para asignar los uniforms de la luz al shader. Tiene en cuenta si está
     * encendida o apagada
     *
     * (no quiero asignar solo los que sean necesarios porque puede dar fallo, asigno todas)
     */
    void asignarPropiedadesAShader(const PropiedadesLuz &p, ShaderProgram &sp, glm::mat4 matrizV) {

        GLint Ia_asignar = glGetUniformLocation(sp.id_sp(), "Ia");
        GLint Id_asignar = glGetUniformLocation(sp.id_sp(), "Id");
        GLint Is_asignar = glGetUniformLocation(sp.id_sp(), "Is");
        GLint lightPosition_asignar = glGetUniformLocation(sp.id_sp(), "lightPosition");
        GLint lightDirection_asignar = glGetUniformLocation(sp.id_sp(), "lightDirection");
        GLint spotAngle_asignar = glGetUniformLocation(sp.id_sp(), "spotAngle");

        if (Ia_asignar != -1)
            glUniform3fv(Ia_asignar, 1, &p.Ia[0]);

        if (Id_asignar != -1)
            glUniform3fv(Id_asignar, 1, &p.Id[0]);

        if (Is_asignar != -1)
            glUniform3fv(Is_asignar, 1, &p.Is[0]);

        if (lightPosition_asignar != -1)
            //Importante multiplicar por la matriz de visión (se hace aquí porque es un uniform --> una sola vez)
            glUniform3fv(lightPosition_asignar, 1, &glm::vec3(matrizV * glm::vec4(p.p, 1.0f))[0]);

        if (lightDirection_asignar != -1) {
            //Se normaliza la dirección de luz (para no hacerlo repetidamente en el shader --> más eficiente)
            glm::vec3 direccionLuz = glm::normalize(p.d);

            //Importante multiplicar por la matriz de visión (se hace aquí porque es un uniform --> una sola vez)
            glUniform3fv(lightDirection_asignar, 1, &glm::vec3(matrizV * glm::vec4(direccionLuz, 0.0f))[0]);
        }

        if (spotAngle_asignar != -1)
            glUniform1f(spotAngle_asignar, p.gamma);
    }


    /**
     * FUNCIONES DE APLICACIÓN DE LAS DISTINTAS LUCES
     */

    /**
     * Luz ambiental
     */
    void LuzAmbiental_Aplicador::aplicarLuz(const PropiedadesLuz &p, ShaderProgram &sp, glm::mat4 matrizV) {
        std::string nombreSubrutina = "luzAmbiental";
        asignarPropiedadesAShader(p, sp, matrizV);

        //---------------------------------------------
        //   PAG 08 (subrutinas de luz)
        //---------------------------------------------
        if (sp.nombre_shader() == "pag08") {
            GLint subrutinaLuz  = glGetSubroutineUniformLocation ( sp.id_sp(), GL_FRAGMENT_SHADER, "colorMallaOLuz" );
            GLuint id_subrutinaAmbiental = glGetSubroutineIndex ( sp.id_sp(), GL_FRAGMENT_SHADER, nombreSubrutina.c_str());

            //Ponemos en el hueco de la subrutina el índice de la función que se va a usar
            GLuint indices[1];
            indices[subrutinaLuz]  = id_subrutinaAmbiental;
            glUniformSubroutinesuiv ( GL_FRAGMENT_SHADER, 1, indices );
        }
    }



    /**
     * Luz puntual
     */
    void LuzPuntual_Aplicador::aplicarLuz(const PropiedadesLuz &p, ShaderProgram &sp, glm::mat4 matrizV) {
        std::string nombreSubrutina = "luzPuntual";
        asignarPropiedadesAShader(p, sp, matrizV);

        //---------------------------------------------
        //   PAG 08 (subrutinas de luz)
        //---------------------------------------------
        if (sp.nombre_shader() == "pag08") {
            GLint subrutinaLuz  = glGetSubroutineUniformLocation ( sp.id_sp(), GL_FRAGMENT_SHADER, "colorMallaOLuz" );
            GLuint id_subrutinaLuzPuntual = glGetSubroutineIndex ( sp.id_sp(), GL_FRAGMENT_SHADER, nombreSubrutina.c_str());

            //Ponemos en el hueco de la subrutina el índice de la función que se va a usar
            GLuint indices[1];
            indices[subrutinaLuz]  = id_subrutinaLuzPuntual;
            glUniformSubroutinesuiv ( GL_FRAGMENT_SHADER, 1, indices );
        }
    }


    /**
     * Luz direccional
     */
    void LuzDireccional_Aplicador::aplicarLuz(const PropiedadesLuz &p, ShaderProgram &sp, glm::mat4 matrizV) {
        std::string nombreSubrutina = "luzDireccional";
        asignarPropiedadesAShader(p, sp, matrizV);

        //---------------------------------------------
        //   PAG 08 (subrutinas de luz)
        //---------------------------------------------
        if (sp.nombre_shader() == "pag08") {
            GLint subrutinaLuz  = glGetSubroutineUniformLocation ( sp.id_sp(), GL_FRAGMENT_SHADER, "colorMallaOLuz" );
            GLuint id_subrutinaLuzDireccional = glGetSubroutineIndex ( sp.id_sp(), GL_FRAGMENT_SHADER, nombreSubrutina.c_str());

            //Ponemos en el hueco de la subrutina el índice de la función que se va a usar
            GLuint indices[1];
            indices[subrutinaLuz]  = id_subrutinaLuzDireccional;
            glUniformSubroutinesuiv ( GL_FRAGMENT_SHADER, 1, indices );
        }
    }


    /**
     * Foco de luz
     */
    void Foco_de_Luz_Aplicador::aplicarLuz(const PropiedadesLuz &p, ShaderProgram &sp, glm::mat4 matrizV) {
        std::string nombreSubrutina = "focoLuz";
        asignarPropiedadesAShader(p, sp, matrizV);

        //---------------------------------------------
        //   PAG 08 (subrutinas de luz)
        //---------------------------------------------
        if (sp.nombre_shader() == "pag08") {
            GLint subrutinaLuz  = glGetSubroutineUniformLocation ( sp.id_sp(), GL_FRAGMENT_SHADER, "colorMallaOLuz" );
            GLuint id_subrutinaFoco = glGetSubroutineIndex ( sp.id_sp(), GL_FRAGMENT_SHADER, nombreSubrutina.c_str());

            //Ponemos en el hueco de la subrutina el índice de la función que se va a usar
            GLuint indices[1];
            indices[subrutinaLuz]  = id_subrutinaFoco;
            glUniformSubroutinesuiv ( GL_FRAGMENT_SHADER, 1, indices );
        }
    }






} // PAG