#include <iostream>
// IMPORTANTE: El include de GLAD debe estar siempre ANTES de el de GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
    std::cout << "Comenzando aplicacion PAG - Prueba 01" << std::endl;

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

    // - Hace que el contexto OpenGL asociado a la ventana que acabamos de crear pase a
    // ser el contexto actual de OpenGL para las siguientes llamadas a la biblioteca
    glfwMakeContextCurrent(window);

    // Ahora inicializamos GLAD.
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Fallo en la inicializacion de GLAD" << std::endl;
        glfwDestroyWindow(window); // - Liberamos los recursos que ocupaba GLFW.
        window = nullptr;
        glfwTerminate();
        return -3;
    }

    // Propiedades del contexto 3D construido
    std::cout << "Grafica en uso: " << glGetString(GL_RENDERER) << std::endl
            << "Fabricante: " << glGetString(GL_VENDOR) << std::endl
            << "Version de OpenGL: " << glGetString(GL_VERSION) << std::endl
            << "OpenGL Shading Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


    // Estas 2 siguientes sentencias no tienen por qué llamarse cada vez en el ciclo de eventos

    // Establecemos un gris medio como color con el que se borrará el frame buffer.
    glClearColor(0.6, 0.6, 0.6, 1.0);

    // Le decimos a OpenGL que tenga en cuenta la profundidad a la hora de dibujar.
    glEnable(GL_DEPTH_TEST);

    // Ciclo de eventos de la aplicación. La condición de parada es que la ventana principal deba cerrarse.
    while (!glfwWindowShouldClose(window)) {

        // Borra los buffers (color y profundidad)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // GLFW usa un doble buffer para que no haya parpadeo. Esta orden
        // intercambia el buffer back (en el que se ha estado dibujando) por el
        // que se mostraba hasta ahora (front).
        glfwSwapBuffers(window);

        // Obtiene y organiza los eventos pendientes, tales como pulsaciones de
        // teclas o de ratón, etc. Siempre al final de cada iteración del ciclo
        // de eventos y después de glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Una vez terminado el ciclo de eventos, liberar recursos, etc.
    std::cout << "Terminando aplicacion PAG de prueba" << std::endl;
    glfwDestroyWindow(window); // Cerramos y destruimos la ventana de la aplicación.
    window = nullptr;
    glfwTerminate(); // Liberamos los recursos que ocupaba GLFW.
}
