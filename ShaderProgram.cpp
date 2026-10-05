//
// Created by rferr on 05/10/2026.
//

#include "ShaderProgram.h"



namespace PAG {
    ShaderProgram::~ShaderProgram() {
        //Liberamos recursos del Shader Program y el modelo
        if (idVS != 0) {
            glDeleteShader(idVS);
        }
        if (idFS != 0) {
            glDeleteShader(idFS);
        }
        if (idSP != 0) {
            glDeleteProgram(idSP);
        }
    }

    /**
     * Función que lanza excepción en caso de que haya habido algún tipo de fallo con la compilación de shaders
     * @param id Id a revisar
     * @param tipoShader String identificativo para la excepción
     */
    void PAG::ShaderProgram::revisarFallosCompilacion (GLuint id, const std::string& tipoShader) {
        //Comprobamos errores
        GLint resultadoCompilacion;
        glGetShaderiv ( id, GL_COMPILE_STATUS, &resultadoCompilacion );

        if ( resultadoCompilacion == GL_FALSE )
        {  /* Ha habido un error en la compilación.
              Para saber qué ha pasado, tenemos que recuperar el mensaje de error de
              OpenGL */
            GLint tamMsj = 0;
            std::string mensaje = "";
            glGetShaderiv ( id, GL_INFO_LOG_LENGTH, &tamMsj );

            if ( tamMsj > 0 )
            {
                GLchar* mensajeFormatoC = new GLchar[tamMsj];
                GLint datosEscritos = 0;
                glGetShaderInfoLog ( id, tamMsj, &datosEscritos, mensajeFormatoC );
                mensaje.assign ( mensajeFormatoC );
                delete[] mensajeFormatoC;
                mensajeFormatoC = nullptr;

                throw std::runtime_error("Fallo de compilación de shader de " + tipoShader + "\nMotivo: " + mensaje);
            }
            throw std::runtime_error("Fallo de compilación del shader");
        }
    }


    /**
     * Función que lanza excepción en caso de que haya habido algún tipo de fallo con el enlazado del programa
     * @param idPrograma Id a revisar
     * @param tipoShader String identificativo para la excepción
     */
    void PAG::ShaderProgram::revisarFallosEnlazadoPrograma (GLuint idPrograma) {
        //Comprobamos errores
        GLint resultadoEnlazado = 0;
        glGetProgramiv ( idPrograma, GL_LINK_STATUS, &resultadoEnlazado );

        if ( resultadoEnlazado == GL_FALSE )
        {  /* Ha habido un error en la compilación.
              Para saber qué ha pasado, tenemos que recuperar el mensaje de error de
              OpenGL */
            GLint tamMsj = 0;
            std::string mensaje = "";
            glGetProgramiv ( idPrograma, GL_INFO_LOG_LENGTH, &tamMsj );

            if ( tamMsj > 0 )
            {  GLchar* mensajeFormatoC = new GLchar[tamMsj];
                GLint datosEscritos = 0;
                glGetProgramInfoLog ( idPrograma, tamMsj, &datosEscritos, mensajeFormatoC );
                mensaje.assign ( mensajeFormatoC );
                delete[] mensajeFormatoC;
                mensajeFormatoC = nullptr;

                throw std::runtime_error("Fallo de enlazado del Program Shader\nMotivo: " + mensaje);
            }
            throw std::runtime_error("Fallo de enlazado del Program Shader");
        }
    }

    /**
     * Cargar fichero de shaders
     */
    std::string PAG::ShaderProgram::cargarFichero(const std::string &ruta) {
        std::ifstream archivoShader;
        archivoShader.open(ruta);
        if (!archivoShader.is_open()) {
            throw std::invalid_argument("El fichero " + ruta + " no se pudo cargar. Revisa la ruta.");
        }

        std::stringstream streamShader;
        streamShader << archivoShader.rdbuf();
        std::string codigoFuenteShader = streamShader.str();
        archivoShader.close();

        return codigoFuenteShader;
    }

    /**
     * Función para crear, compilar y enlazar el shader program
     */
    void PAG::ShaderProgram::creaShaderProgram(const std::string &nombre_shaders) {
        try {

            //Cargamos ficheros de shaders (vértices y fragmento)
            std::string carpetaProyecto = "shaders/";

            std::cout << "Cargando fichero " << nombre_shaders << "-vs.glsl" << std::endl;
            std::string miVertexShader = cargarFichero(carpetaProyecto + nombre_shaders + "-vs.glsl");
            std::cout << "Cargando fichero " << nombre_shaders << "-fs.glsl" << std::endl;
            std::string miFragmentShader = cargarFichero(carpetaProyecto + nombre_shaders + "-fs.glsl");

            //Creamos y compilamos shaders de vértice
            idVS = glCreateShader(GL_VERTEX_SHADER);
            if (idVS == 0) {throw std::invalid_argument("Falló la sentencia glCreateShader para el shader de vertices");}
            const GLchar *fuenteVS = miVertexShader.c_str();
            glShaderSource(idVS, 1, &fuenteVS, nullptr);
            glCompileShader(idVS);
            revisarFallosCompilacion(idVS, "vertices");

            //Creamos y compilamos shaders de fragmento
            idFS = glCreateShader(GL_FRAGMENT_SHADER);
            if (idFS == 0) {throw std::invalid_argument("Falló la sentencia glCreateShader para el shader de fragmentos");}
            const GLchar *fuenteFS = miFragmentShader.c_str();
            glShaderSource(idFS, 1, &fuenteFS, nullptr);
            glCompileShader(idFS);
            revisarFallosCompilacion(idFS, "fragmentos");

            idSP = glCreateProgram();
            if (idSP == 0) {throw std::invalid_argument("Falló la sentencia glCreateProgram, por lo que no se pudo crear el Shader Program");}
            glAttachShader(idSP, idVS);
            glAttachShader(idSP, idFS);
            glLinkProgram(idSP);
            revisarFallosEnlazadoPrograma(idSP);

            //En este punto, la carga del shader ha ido bien. Ponemos el nombre del shader:
            _nombreShader = nombre_shaders;

        } catch (std::invalid_argument &e) {
            throw std::invalid_argument(
                std::string("No se pudo cargar el Shader Program\nRazon: ") + e.what());
        }
    }

    /**
     * Getter de Id del shader program
     * @return
     */
    GLuint PAG::ShaderProgram::id_sp() const {
        return idSP;
    }

    /**
     * Getter del nombre del shader. Esto permitirá cargar luego sus uniforms según el shader que sea
     */
    std::string PAG::ShaderProgram::nombre_shader() const {
        return _nombreShader;
    }
} // PAG