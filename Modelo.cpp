//
// Created by rferr on 14/10/2026.
//

#include "Modelo.h"


namespace PAG {

    /**
     * Constructor desde el directorio indicado del modelo
     */
    PAG::Modelo::Modelo(const char *rutaOBJ, const std::string &rutaShader) {
        cargar_shader_program(rutaShader);
        cargar_modelo(rutaOBJ);
    }

    Modelo::~Modelo() {
        //Liberamos recursos del modelo
        if (idVBO_posiciones != 0) {
            glDeleteBuffers(1, &idVBO_posiciones);
        }
        if (idVBO_colores != 0) {
            glDeleteBuffers(1, &idVBO_colores);
        }
        if (idIBO != 0) {
            glDeleteBuffers(1, &idIBO);
        }
        if (idVAO != 0) {
            glDeleteVertexArrays(1, &idVAO);
        }
    }

    /**
     * Cargar shaderProgram desde un directorio (lo usará el constructor)
     */
    void PAG::Modelo::cargar_shader_program(std::string ruta) {
        try {
            _shader_modelo = ShaderProgram();
            _shader_modelo.creaShaderProgram(ruta);
        } catch (std::exception &e) {
            std::cout << "\n--------------------\n" << "ERROR::SHADER:: " << e.what() <<
                    "\n--------------------\n" << std::endl;
        }
    }


    /**
     * Cargar modelo desde un directorio (lo usará el constructor)
     */
    void PAG::Modelo::cargar_modelo(std::string ruta)
    {
        //Cargamos ficheros de shaders (vértices y fragmento)
        Assimp::Importer importador;
        const aiScene *escena = importador.ReadFile(ruta, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenSmoothNormals);

        if(!escena || escena->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !escena->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << importador.GetErrorString() << std::endl;
            return;
        }
        _directorio = ruta.substr(0, ruta.find_last_of('/'));

        procesarMalla(*escena->mMeshes);
    }


    void PAG::Modelo::procesarMalla(aiMesh *mesh)
    {

        //Procesar los vértices y normales
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            vertices.push_back(mesh->mVertices[i].x);
            vertices.push_back(mesh->mVertices[i].y);
            vertices.push_back(mesh->mVertices[i].z);

            normales.push_back(mesh->mNormals[i].x);
            normales.push_back(mesh->mNormals[i].y);
            normales.push_back(mesh->mNormals[i].z);
        }

        //Procesamos los índices
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            for(unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
            {
                indices.push_back(mesh->mFaces[i].mIndices[j]);
            }
        }

        //OJO: usaré las normales como color del objeto...
        creaVAO(vertices, normales, indices);
    }


    /**
     * Función para crear el VAO para el modelo a renderizar. Lo hace con VBO NO ENTRELAZADO
     */
    void PAG::Modelo::creaVAO(std::vector<GLfloat> vertices, std::vector<GLfloat> color, std::vector<unsigned int> indices) {

        //Generación y activación del VAO
        glGenVertexArrays(1, &idVAO);
        glBindVertexArray(idVAO);

        //Datos de posiciones (location = 0)
        glGenBuffers(1, &idVBO_posiciones);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO_posiciones);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(0); //Colocamos posición de vértices como atributo 0

        //Datos de color (location = 1)
        glGenBuffers(1, &idVBO_colores);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO_colores);
        glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(GLfloat), color.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(1); //Colocamos color de vértices como atributo 1

        //Procesamiento de índices (IBOs)
        glGenBuffers(1, &idIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    }


    ShaderProgram *Modelo::getShaderModelo() {
        return &_shader_modelo;
    }


    /**
     * Función para dibujar el VAO del modelo (cuando el Renderer lo necesite)
     */
    void Modelo::Dibujar() {
        glBindVertexArray(idVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }
} // PAG