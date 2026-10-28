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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Material.h"
#include "ShaderProgram.h"

namespace PAG {

    enum TipoAccionModelo {
        Traslacion_modelo,
        RotacionX_modelo,
        RotacionY_modelo,
        RotacionZ_modelo,
        Escalado_modelo,
        Borrar_modelo,
    };


    class Modelo {

    private:

        GLuint idVAO = 0; // Identificador del vertex array object
        GLuint idVBO_posiciones = 0; // Identificador del vertex buffer object de posiciones
        GLuint idVBO_colores = 0; // Identificador del vertex buffer object de colores
        GLuint idIBO = 0; // Identificador del index buffer object

        std::vector<glm::vec3> _vertices;
        std::vector<glm::vec3> _colorVertice;
        std::vector<unsigned int> _indices;

        glm::mat4 matrizModelado = glm::mat4(1.0f);     //Identidad

        std::string _directorio;
        ShaderProgram _shader_modelo;
        Material* _material = nullptr;

    public:
        std::string _nombreModelo;

        glm::vec3 _posicion;

        Modelo(const std::string& nombreModelo, const char *rutaOBJ, const std::string &rutaShader, glm::vec3 posicionInicial);
        ~Modelo();
        ShaderProgram* getShaderModelo();
        glm::mat4 getMatrizModelado() const;
        void Dibujar();
        void aplicarTransformacion(TipoAccionModelo tipo_transformacion, const glm::mat4 &transformacion);

    private:
        bool cargar_shader_program(std::string ruta);
        bool cargar_modelo(std::string ruta);
        void procesarMalla(aiMesh *mesh);
        void creaVAO(std::vector<glm::vec3> vertices, std::vector<glm::vec3> color, std::vector<unsigned int> indices);




    };
} // PAG

#endif //PRACTICA1_MODELO_H
