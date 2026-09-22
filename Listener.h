//
// Created by rferr on 22/09/2026.
//

#ifndef PRACTICA1_LISTENER_H
#define PRACTICA1_LISTENER_H

namespace PAG {

    /**
     * Enumerado de tipo de ventanas (de Ventanas.h)
     *
     * Ellas usan este enumerado para avisar de un evento y que el Listener sepa quién lo produjo
     */
    enum TipoVentana {
        V_Mensajes,
        V_Selecc_Color,
        V_Selecc_Escala
    };

    class Listener {
    public:
        Listener () = default;
        virtual ~Listener () = default;
        // TipoVentana es un enum para identificar el tipo de ventana
        // de la interfaz que quiere despertarme
        virtual void wakeUp ( TipoVentana t, ... ) = 0;

    };
} // PAG

#endif //PRACTICA1_LISTENER_H
