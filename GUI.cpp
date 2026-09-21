//
// Created by rferr on 21/09/2026.
//

#include "GUI.h"

namespace PAG {
    //Inicialización de los atributos
    PAG::GUI* PAG::GUI::instancia = nullptr;

    //Constructor por defecto
    GUI::GUI(){}

    //Destructor por defecto
    GUI::~GUI(){}


    //Métodos

    /**
     * Función para consultar el objeto único de la clase
     * @return La dirección de memoria del objeto
     */
    PAG::GUI& PAG::GUI::getInstancia ()
    {
        if ( !instancia ){ // Lazy initialization: si aún no existe, lo crea
            instancia = new GUI();
        }
        return *instancia;
    }

    /**
     * Inicialización de ImGui (esto es común a todas las implementaciones --> desacoplado)
     */
    void PAG::GUI::inicializacionIMGUI ()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext ();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    }

    /**
     * Destruccion de ImGui (esto es común a todas las implementaciones --> desacoplado)
     */
    void PAG::GUI::finalizacionIMGUI()
    {
        ImGui::DestroyContext ();
    }

    /**
     * Función para consultar el objeto único de la clase
     * @return La dirección de memoria del objeto
     */
    void PAG::GUI::dibujarVentana ()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // Se dibujan los controles de Dear ImGui

        //Dibujado de ventanas
        ImGui::SetNextWindowPos ( ImVec2 (10, 10), ImGuiCond_Once );
        if ( ImGui::Begin ( "Mensajes" ) )
        { // La ventana está desplegada
            ImGui::SetWindowFontScale ( 1.0f ); // Escalamos el texto si fuera necesario
            // Pintamos los controles
        }
        // Si la ventana no está desplegada, Begin devuelve false
        ImGui::End ();

        // Aquí va el dibujado de la escena con instrucciones OpenGL
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData ( ImGui::GetDrawData() );
    }

} // PAG