//
// Created by rferr on 05/10/2026.
//

#include "Camara.h"

namespace PAG {
    glm::mat4 PAG::Camara::getMatVP () const {

        glm::mat4 v, p;

        v = glm::lookAt(position, lookAt, up);
        p = glm::perspective(fovY, aspect, zNear, zFar);

        glm::mat4 devolver = p*v;
        return devolver;
    }

} // PAG