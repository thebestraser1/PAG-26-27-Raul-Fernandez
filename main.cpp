#include <iostream>
#include <sstream>

// IMPORTANTE: El include de GLAD (que ahora está en renderer) debe estar siempre ANTES de el de GLFW.
// Así todas las funciones (de nuestra versión de OpenGL) podrán ser usadas
#include "Renderer.h"
#include <GLFW/glfw3.h>             // Gestión de ventana y eventos con OpenGL

//Inclusión de ImGui (con ventanas de GLFW y dibujo de OpenGL)
#include "GUI.h"

//Inclusión de ventanas
#include <vector>

#include "Ventanas.h"


// -----------------------------------------------------
// -------------------- CALLBACKS ----------------------
// -----------------------------------------------------

/**
 * Esta función callback será llamada cuando GLFW produzca algún error
 */
void error_callback(int errno, const char *desc) {
    std::string aux(desc);
    std::cout << "Error de GLFW numero " << errno << ": " << aux << std::endl;
}


/**
 * Esta función callback será llamada cada vez que se cambie el tamaño del área de dibujo OpenGL.
 */
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    PAG::Renderer::getInstancia().redimensionar(width, height);
    std::cout << "Callback de redimension llamado" << std::endl;
}

/**
 * Esta función callback será llamada cada vez que se pulse una tecla dirigida al área de dibujo OpenGL.
 */
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    //Cierre de ventana
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    std::cout << "Callback de tecla llamado" << std::endl;
}

/**
 * Esta función callback será llamada cada vez que se pulse algún botón del ratón sobre el área de dibujo OpenGL.
 */
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        std::cout << "Pulsado el boton: " << button << std::endl;

        //Tras procesarlo con GLFW, se pasa el callback a ImGui
        ImGuiIO& io = ImGui::GetIO ();
        io.AddMouseButtonEvent ( button, true );

    } else if (action == GLFW_RELEASE) {
        std::cout << "Soltado el boton: " << button << std::endl;

        //Tras procesarlo con GLFW, se pasa el callback a ImGui
        ImGuiIO& io = ImGui::GetIO ();
        io.AddMouseButtonEvent ( button, false );
    }
}

/**
 * Esta función callback será llamada cada vez que se mueva la rueda del ratón sobre el área de dibujo OpenGL.
 */
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    std::cout << "Movida la rueda del raton " << xoffset
            << " Unidades en horizontal y " << yoffset
            << " unidades en vertical" << std::endl;
}


/**
 * Callback de cambio de color con rueda del ratón. Va cambiando entre tonalidades de grises

void scroll_color_callback(GLFWwindow *window, double xoffset, double yoffset) {
    GLfloat VARIACION = (GLfloat) yoffset / 10;     //Calculo la variación. En este caso (-0.1 o 0.1)

    GLfloat color_actual[4]; //Creamos un vector de 4 para el color actual de la ventana
    PAG::Renderer::getInstancia().getColorFondo(color_actual);

    if (yoffset > 0) {
        std::cout << "Moviste la rueda del raton hacia arriba" << std::endl;
    } else if (yoffset < 0) {
        std::cout << "Moviste la rueda del raton hacia abajo" << std::endl;
    }
    //En caso de == 0 no se hace nada

    GLfloat rojo = color_actual[0] + VARIACION;
    GLfloat verde = color_actual[1] + VARIACION;
    GLfloat azul = color_actual[2] + VARIACION;

    if (rojo > 1) rojo = 0;       if (rojo < 0) rojo = 1;
    if (verde > 1) verde = 0;     if (verde < 0) verde = 1;
    if (azul > 1) azul = 0;       if (azul < 0) azul = 1;

    //NOTA: Los 6 ifs es porque quiero un bucle de escala de grises. Si simplemente quisiera controlar los
    //colores, bastaría con la función "clamp" vista en teoría.

    PAG::Renderer::getInstancia().cambiarColorFondo(rojo, verde, azul, 1.0);
}
*/

// -----------------------------------------------------
// --------------------   MAIN    ----------------------
// -----------------------------------------------------

int main() {

    //Almacenar el Cout en un buffer
    std::stringstream buffer;
    std::streambuf *original = std::cout.rdbuf(buffer.rdbuf());

    std::cout << "Comenzando aplicacion PAG - Prueba 01" << std::endl;

    // Este callback hay que registrarlo ANTES de llamar a glfwInit
    glfwSetErrorCallback((GLFWerrorfun) error_callback);

    // Inicialización de GLFW. Si no está no hay nada que hacer
    if (glfwInit() != GLFW_TRUE) {
        std::cout << "Fallo al inicializar GLFW" << std::endl;
        return -1;
    }

    // Definimos las características que queremos que tenga el contexto gráfico
    // OpenGL de la ventana que vamos a crear.
    glfwWindowHint(GLFW_SAMPLES, 4); // - Activa antialiasing x4.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Esta y las 2
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // siguientes activan un contexto
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // OpenGL Core Profile 4.3.

    // Definimos el puntero para guardar la dirección de la ventana de la aplicación y
    // la creamos
    GLFWwindow *window;

    // Tamaño, título de la ventana, en ventana y no en pantalla completa,
    // El primer nullptr --> Es el monitor donde quiero que se inicialice la ventana en pantalla completa
    // El segundo nullptr --> Sirve para compartir recursos OpenGL con otra ventana
    window = glfwCreateWindow(1024, 576, "PAG. Practica 1", nullptr, nullptr);

    // Comprobamos si la creación de la ventana ha tenido éxito.
    if (window == nullptr) {
        std::cout << "Fallo al abrir la ventana de GLFW" << std::endl;
        glfwTerminate(); // - Liberamos los recursos que ocupaba GLFW.
        return -2;
    }

    // Hace que el contexto OpenGL asociado a la ventana que acabamos de crear pase a
    // ser el contexto actual de OpenGL para las siguientes llamadas a la biblioteca
    glfwMakeContextCurrent(window);

    // Inicialización de GLAD.
    if (!PAG::Renderer::getInstancia().inicializarGLAD((void*) glfwGetProcAddress)) {
        std::cout << "Fallo en la inicializacion de GLAD" << std::endl;
        glfwDestroyWindow(window); // Liberamos los recursos que ocupaba GLFW.
        window = nullptr;
        glfwTerminate();
        return -3;
    }

    // Propiedades del contexto 3D construido
    PAG::Renderer::getInstancia().mostrarPropiedadesContextoGrafico();

    //Registramos los callbacks que responderán a los eventos principales
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);


    //Inicialización de ImGui
    PAG::GUI::getInstancia().inicializacionIMGUI();

    //Para el caso de GLFW y OpenGL, hay que completar la inicialización con las siguientes llamadas:
    ImGui_ImplGlfw_InitForOpenGL ( window, true );
    ImGui_ImplOpenGL3_Init ();
    //Nota: considero que las 2 sentencias de arriba van en main.cpp porque acoplarían GUI si las meto en la clase


    // Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar.
    PAG::Renderer::getInstancia().activarPruebaProfundidad();


    // Inicialización de escena
    // Establecemos un gris medio como color con el que se borrará el frame buffer.
    GLfloat *colorFondo = PAG::Renderer::getInstancia().getColorFondo();
    PAG::Renderer::getInstancia().cambiarColorFondo(colorFondo[0], colorFondo[1], colorFondo[2], colorFondo[3]);

    //Establecemos escala del texto
    GLfloat *escala = PAG::Renderer::getInstancia().getEscalaTexto();

    //Establecenmos una ventana de mensajes y una ventana de selección de color
    std::vector<PAG::Ventanas*> ventanas = {
        new PAG::VentanaMensajes(buffer, 10, 10, escala),
        new PAG::VentanaSelectorColor(colorFondo, 280,40, escala),
        new PAG::VentanaSelectorEscala(escala, 100, 400, escala)
    };


    // Ciclo de eventos de la aplicación. La condición de parada es que la ventana principal deba cerrarse.
    /**
     * Esta función antes era un callback a glfwSetWindowRefreshCallback. Sin embargo, esto actualiza
     * la ventana cuando GLFW lo considera necesario. Por el caracter interactivo de ImGui, lo suyo es
     * incorporar esta función al ciclo de eventos (si no, falla) (creo que luego con el patrón observador
     * podemos hacer que solo cuando haya un cambio se llame a esto...)
     */
    while (!glfwWindowShouldClose(window)) {
        PAG::Renderer::getInstancia().refrescar();      //Encapsulación de OpenGL

        // AQUÍ SE DIBUJARÍA LO QUE SE NECESITE
        //-------------------------------------

        PAG::GUI::getInstancia().dibujarVentanas(ventanas);

        //Si cambió el color de fondo, deberá reflejarse
        PAG::Renderer::getInstancia().cambiarColorFondo(colorFondo[0], colorFondo[1], colorFondo[2], colorFondo[3]);

        //-------------------------------------

        // GLFW usa un doble buffer para que no haya parpadeo. Esta orden
        // intercambia el buffer back (que se ha estado dibujando) por el
        // que se mostraba hasta ahora front. Debe ser la última orden de
        // este callback
        glfwSwapBuffers(window);

        // Obtiene y organiza los eventos pendientes, tales como pulsaciones de
        // teclas o de ratón, etc. Siempre al final de cada iteración del ciclo
        // de eventos y después de glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Una vez terminado el ciclo de eventos, liberar recursos, etc.
    std::cout << "Terminando aplicacion PAG de prueba" << std::endl;

    //Finalizamos ImGui (adaptado a GLFW y OpenGL)
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    //Nota: considero que las 2 sentencias de arriba van en main.cpp porque acoplarían GUI si las meto en la clase

    //Finalización ImGui
    PAG::GUI::getInstancia().finalizacionIMGUI();

    //Borramos todas las ventanas creadas
    for (PAG::Ventanas* ventana : ventanas) {
        delete ventana;
    }

    std::cout.rdbuf(original); //Devolvemos el read/write buffer original (para el cout)
    glfwDestroyWindow(window); // Cerramos y destruimos la ventana de la aplicación.
    window = nullptr;
    glfwTerminate(); // Liberamos los recursos que ocupaba GLFW.
}
