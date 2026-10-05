#version 410

/* Entradas */
in vec3 color_vertex;

/* Salidas */
out vec4 colorFragmento;

void main ()
{
    colorFragmento = vec4 ( color_vertex, 1.0 );
}