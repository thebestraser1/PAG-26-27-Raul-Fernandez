#version 410

/* Entradas */
layout (location = 0) in vec3 posicion;
layout (location = 1) in vec3 color;

/* Uniforms */
/* Matriz de transformación que combina visión y proyección de cámara */
uniform mat4 matrizVP;

/* Salidas */
/* Color (RGB) de cada vértice */
out vec3 color_vertex;

void main ()
{
    color_vertex = color;
    gl_Position = matrizVP * vec4 ( posicion, 1 );
}