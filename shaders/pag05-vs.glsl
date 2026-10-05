#version 410

/* Entradas */
layout (location = 0) in vec3 posicion;
layout (location = 1) in vec3 color;

/* Uniforms */
/* Matriz de transformación que combina modelado, visión y proyección */
uniform mat4 matrizMVP;

/* Salidas */
/* Color (RGB) de cada vértice */
out vec3 color_vertex;

void main ()
{
    color_vertex = color;
    gl_Position = matrizMVP * vec4 ( posicion, 1 );
}