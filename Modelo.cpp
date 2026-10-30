//
// Created by rferr on 14/10/2026.
//

#include "Modelo.h"


namespace PAG {

    /**
     * Constructor desde el directorio indicado del modelo
     */
    PAG::Modelo::Modelo(const std::string& nombreModelo, const char *rutaOBJ, const std::string &rutaShader, glm::vec3 posicion)
        : _posicion(posicion), _nombreModelo(nombreModelo){

        if (!cargar_shader_program(rutaShader)) {
            throw std::runtime_error("No se pudo cargar el shader: " + rutaShader);
        }
        if (!cargar_modelo(rutaOBJ)) {
            throw std::runtime_error(std::string("No se pudo cargar el modelo: ") + rutaOBJ);
        }
    }

    Modelo::~Modelo() {
        //Liberamos recursos del modelo
        if (idVBO_posiciones != 0) {
            glDeleteBuffers(1, &idVBO_posiciones);
        }
        if (idVBO_normales != 0) {
            glDeleteBuffers(1, &idVBO_normales);
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
    bool PAG::Modelo::cargar_shader_program(std::string ruta) {
        try {
            _shader_modelo = ShaderProgram();
            _shader_modelo.creaShaderProgram(ruta);
            return true;
        } catch (std::exception &e) {
            std::cout << "\n--------------------\n" << "ERROR::SHADER:: " << e.what() <<
                    "\n--------------------\n" << std::endl;
            return false;
        }
    }


    /**
     * Cargar modelo desde un directorio (lo usará el constructor)
     */
    bool PAG::Modelo::cargar_modelo(std::string ruta)
    {
        //Cargamos ficheros de shaders (vértices y fragmento)
        Assimp::Importer importador;
        const aiScene *escena = importador.ReadFile(ruta, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenSmoothNormals);

        if(!escena || escena->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !escena->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << importador.GetErrorString() << std::endl;
            return false;
        }
        _directorio = ruta.substr(0, ruta.find_last_of('/'));

        procesarMalla(*escena->mMeshes);
        return true;
    }

    /**
     * Función para rellenar vértices, normales e índices a partir de la malla de Assimp
     * @param mesh
     */
    void PAG::Modelo::procesarMalla(aiMesh *mesh)
    {

        //Procesar los vértices y normales
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            //Hay que tener en cuenta la posición inicial del modelo
            _vertices.emplace_back(mesh->mVertices[i].x + _posicion[0], mesh->mVertices[i].y + _posicion[1], mesh->mVertices[i].z + _posicion[2]);

            //Asignaré para esta práctica como color las normales del modelo
            _normales.emplace_back(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }

        //Procesamos los índices
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            for(unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
            {
                _indices.push_back(mesh->mFaces[i].mIndices[j]);
            }
        }

        creaVAO(_vertices, _normales, _indices);
    }


    /**
     * Función para crear el VAO para el modelo a renderizar. Lo hace con VBO NO ENTRELAZADO
     */
    void PAG::Modelo::creaVAO(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normales, std::vector<unsigned int> indices) {

        //Generación y activación del VAO
        glGenVertexArrays(1, &idVAO);
        glBindVertexArray(idVAO);

        //Datos de posiciones (location = 0)
        glGenBuffers(1, &idVBO_posiciones);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO_posiciones);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(0); //Colocamos posición de vértices como atributo 0

        //Datos de color (location = 1)
        glGenBuffers(1, &idVBO_normales);
        glBindBuffer(GL_ARRAY_BUFFER, idVBO_normales);
        glBufferData(GL_ARRAY_BUFFER, normales.size() * sizeof(glm::vec3), normales.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(1); //Colocamos color de vértices como atributo 1

        //Procesamiento de índices (IBOs)
        glGenBuffers(1, &idIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    }


    ShaderProgram *Modelo::getShaderModelo() {
        return &_shader_modelo;
    }


    glm::mat4 Modelo::getMatrizModelado() const {
        return matrizModelado;
    }


    /**
     * Función para dibujar el VAO del modelo (cuando el Renderer lo necesite)
     */
    void Modelo::Dibujar() {
        glBindVertexArray(idVAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idIBO);
        glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }


    /**
     * Función para aplicar matriz de transformación a los vértices y normales
     */
    void Modelo::aplicarTransformacion(TipoAccionModelo tipo_transformacion, const glm::mat4 &transformacion) {

        //TRANSFORMACIÓN GENERAL DEL MODELO

        // Se acumulan las transformaciones en la matriz de modelado del modelo (hago lo de la posición para que las
        // rotaciones se hagan sobre sí mismas respecto a la posición inicial del modelo
        matrizModelado = matrizModelado * glm::translate(glm::mat4(1.0f), _posicion) * transformacion * glm::translate(glm::mat4(1.0f), -_posicion);
    }


    /**
     * Función para asignar un material al modelo
     */
    void Modelo::asignarMaterial(Material* material) {
        _material = material;
    }

    /**
     * Función para eliminar el material del modelo
     */
    void Modelo::eliminarMaterial() {
        _material = nullptr;
    }


    /**
     * Función para obtener el material del modelo
     */
    Material* Modelo::obtenerMaterial() const {
        return _material;
    }

} // PAG