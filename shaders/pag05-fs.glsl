#version 410

/* Entradas */
in vec3 normal;

/* Salidas */
out vec4 colorFragmento;

void main ()
{
    colorFragmento = vec4 ( normal, 1.0 );
}