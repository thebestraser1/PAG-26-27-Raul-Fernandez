//
// Created by rferr on 11/11/2026.
//

#include "Luz.h"

namespace PAG {

    PAG::Luz::Luz(PropiedadesLuz p_iniciales) :
        _tipoLuz(TipoLuz::Luz_Ambiental),
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
    void PAG::Luz::aplicarLuz(ShaderProgram* sp) const {

        int indice_tipo_luz = static_cast<int>(_tipoLuz);

        _aplicadores[indice_tipo_luz]->aplicarLuz(_propiedades, *sp);
    }


    /**
     * Función para aplicar cambiar el tipo de luz
     */
    void PAG::Luz::setTipoLuz(TipoLuz tipo_luz) {
        _tipoLuz = tipo_luz;
    }



    /**
     * Función auxiliar de este CPP para asignar los uniforms de la luz al shader
     *
     * (no quiero asignar solo los que sean necesarios porque puede dar fallo, asigno todas)
     */
    void asignarPropiedadesAShader(const PropiedadesLuz &p, ShaderProgram &sp) {
        GLint Ia_asignar = glGetUniformLocation(sp.id_sp(), "Ia");
        GLint Id_asignar = glGetUniformLocation(sp.id_sp(), "Id");
        GLint Is_asignar = glGetUniformLocation(sp.id_sp(), "Is");
        GLint shininess_asignar = glGetUniformLocation(sp.id_sp(), "shininess");
        GLint lightPosition_asignar = glGetUniformLocation(sp.id_sp(), "lightPosition");
        GLint lightDirection_asignar = glGetUniformLocation(sp.id_sp(), "lightDirection");
        GLint spotAngle_asignar = glGetUniformLocation(sp.id_sp(), "spotAngle");

        if (Ia_asignar != -1)
            glUniform3fv(Ia_asignar, 1, &p.Ia[0]);

        if (Id_asignar != -1)
            glUniform3fv(Id_asignar, 1, &p.Id[0]);

        if (Is_asignar != -1)
            glUniform3fv(Is_asignar, 1, &p.Is[0]);

        if (shininess_asignar != -1)
            glUniform1f(shininess_asignar, p.s);

        if (lightPosition_asignar != -1)
            glUniform3fv(lightPosition_asignar, 1, &p.p[0]);

        if (lightDirection_asignar != -1)
            glUniform3fv(lightDirection_asignar, 1, &p.d[0]);

        if (spotAngle_asignar != -1)
            glUniform1f(spotAngle_asignar, p.gamma);
    }


    /**
     * FUNCIONES DE APLICACIÓN DE LAS DISTINTAS LUCES
     */

    /**
     * Luz ambiental
     */
    void LuzAmbiental_Aplicador::aplicarLuz(const PropiedadesLuz &p, ShaderProgram &sp) {
        std::string nombreSubrutina = "luzAmbiental";
        asignarPropiedadesAShader(p, sp);

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
    void LuzPuntual_Aplicador::aplicarLuz(const PropiedadesLuz &p, ShaderProgram &sp) {
        std::string nombreSubrutina = "luzPuntual";
        asignarPropiedadesAShader(p, sp);

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
    void LuzDireccional_Aplicador::aplicarLuz(const PropiedadesLuz &p, ShaderProgram &sp) {
        std::string nombreSubrutina = "luzDireccional";
        asignarPropiedadesAShader(p, sp);

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
    void Foco_de_Luz_Aplicador::aplicarLuz(const PropiedadesLuz &p, ShaderProgram &sp) {
        std::string nombreSubrutina = "focoLuz";
        asignarPropiedadesAShader(p, sp);

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