//
// Created by rferr on 05/10/2026.
//

#ifndef PRACTICA1_SHADERPROGRAM_H
#define PRACTICA1_SHADERPROGRAM_H

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

#include "glad/glad.h"



namespace PAG {
    class ShaderProgram {
    private:
        GLuint idVS = 0; // Identificador del vertex shader
        GLuint idFS = 0; // Identificador del fragment shader
        GLuint idSP = 0; // Identificador del shader program

        static std::string cargarFichero(const std::string &ruta);
        static void revisarFallosCompilacion(GLuint id, const std::string& tipoShader);
        static void revisarFallosEnlazadoPrograma(GLuint idPrograma);
    public:
        ~ShaderProgram();
        void creaShaderProgram(const std::string &nombre_shaders);
        GLuint id_sp() const;
    };
} // PAG

#endif //PRACTICA1_SHADERPROGRAM_H
