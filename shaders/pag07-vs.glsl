#version 410

/* NOTA: no he tocado aún este shader al no tener cálculos de iluminación (para pasar matrizMV) */

/* Entradas */
layout (location = 0) in vec3 Vposicion;
layout (location = 1) in vec3 Vnormal;

/* Uniforms */
/* Matriz de modelado trasformación y visión */
uniform mat4 matrizMVP;

/* Salidas */
/* Normal de cada vértice */
out vec3 normal;

void main ()
{
    normal = Vnormal;
    gl_Position = matrizMVP * vec4 ( Vposicion, 1 );
}