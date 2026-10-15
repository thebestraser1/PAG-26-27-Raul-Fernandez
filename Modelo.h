//
// Created by rferr on 14/10/2026.
//

#ifndef PRACTICA1_MODELO_H
#define PRACTICA1_MODELO_H

#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h>
#include "ShaderProgram.h"

namespace PAG {
    class Modelo {

    private:
        GLuint idVAO = 0; // Identificador del vertex array object
        GLuint idVBO_posiciones = 0; // Identificador del vertex buffer object de posiciones
        GLuint idVBO_colores = 0; // Identificador del vertex buffer object de colores
        GLuint idIBO = 0; // Identificador del index buffer object

        std::vector<GLfloat> vertices;
        std::vector<GLfloat> normales;
        std::vector<unsigned int> indices;

        std::string _directorio;

        ShaderProgram _shader_modelo;
    public:
        Modelo(const char *rutaOBJ, const std::string &rutaShader);
        ~Modelo();
        ShaderProgram* getShaderModelo();
        void Dibujar();

    private:
        void cargar_shader_program(std::string ruta);

        void cargar_modelo(std::string ruta);
        void procesarMalla(aiMesh *mesh);
        void creaVAO(std::vector<GLfloat> vertices, std::vector<GLfloat> color, std::vector<unsigned int> indices);

    private:



    };
} // PAG

#endif //PRACTICA1_MODELO_H
