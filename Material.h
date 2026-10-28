#ifndef PRACTICA1_MATERIAL_H
#define PRACTICA1_MATERIAL_H

#include <glad/glad.h>
#include <glm/vec3.hpp>

namespace PAG {

    enum TipoAccionMaterial {
        Anadir_material,
        Borrar_material,
        Asignar_material
    };

    class Material {

    public:
        std::string nombre;

        glm::vec3 ka;
        glm::vec3 kd;
        glm::vec3 ks;

        GLfloat shininess;

        Material(const std::string& nombre, const glm::vec3 &ka, const glm::vec3 &kd, const glm::vec3 &ks, GLfloat shininess)
            : nombre(nombre),
              ka(ka),
              kd(kd),
              ks(ks),
              shininess(shininess) {
        }

        ~Material() = default;
    };
} // PAG

#endif //PRACTICA1_MATERIAL_H
