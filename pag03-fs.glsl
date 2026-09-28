#version 410

in vec3 colores;

out vec4 colorFragmento;

void main ()
{
    colorFragmento = vec4 ( colores, 1.0 );
}