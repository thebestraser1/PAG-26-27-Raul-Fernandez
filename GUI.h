//
// Created by rferr on 21/09/2026.
//

#ifndef PRACTICA1_GUI_H
#define PRACTICA1_GUI_H

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <vector>

#include "Ventanas.h"

namespace PAG {
    class GUI {
    private:
        static GUI* instancia;

        GUI(); //Constructor privado (Singletone)
    public:
        virtual ~GUI();

        static GUI &getInstancia();

        void inicializacionIMGUI();

        void finalizacionIMGUI();

        void dibujarVentana(const std::vector<PAG::Ventanas*>& ventanas);

    };
} // PAG

#endif //PRACTICA1_GUI_H
