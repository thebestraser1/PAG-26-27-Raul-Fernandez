//
// Created by rferr on 11/11/2026.
//

#include "Luz.h"

namespace PAG {

    PAG::Luz::Luz(PropiedadesLuz p_iniciales) :
        _tipoLuz(TipoLuz::Luz_Ambiental),
        _encendida(true),
        _propiedades(p_iniciales){

        //Creamos el vector con los distintos "tipos de luces que se pueden invocar"
        //El tipo de luz determinará el índice que quiero invocar en este vector

        _aplicadores.push_back(std::make_unique<LuzAmbiental_Aplicador>());
        _aplicadores.push_back(std::make_unique<LuzPuntual_Aplicador>());
        _aplicadores.push_back(std::make_unique<LuzDireccional_Aplicador>());
        _aplicadores.push_back(std::make_unique<Foco_de_Luz_Aplicador>());

    }

    /**
     * Función para aplicar la luz independientemente de su tipo
     * @param sp
     */
    void PAG::Luz::aplicarLuz(ShaderProgram* sp) const {

        int indice_tipo_luz = static_cast<int>(_tipoLuz);

        _aplicadores[indice_tipo_luz]->aplicarLuz(_propiedades, *sp);
    }


    /**
     * Función para aplicar cambiar el tipo de luz
     */
    void PAG::Luz::setTipoLuz(TipoLuz tipo_luz) {
        _tipoLuz = tipo_luz;
    }



    /**
     * FUNCIONES DE APLICACIÓN DE LAS DISTINTAS LUCES
     */





} // PAG