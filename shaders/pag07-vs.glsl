#version 410

/* NOTA: no he tocado aún este shader al no tener cálculos de iluminación (para pasar matrizMV) */

/* Entradas */
layout (location = 0) in vec3 vPosicion;
layout (location = 1) in vec3 vNormal;

/* Uniforms */
/* Matriz de modelado trasformación y visión */
uniform mat4 matrizMVP;
uniform mat4 mModelView;

/* Salidas */
/* Normal y posición de cada vértice */

out salidaVS
{
 vec3 posicionV;
 vec3 normalV;
} salida;

void main ()
{
    salida.normalV = vec3(mModelView * vec4(vNormal, 0.0));
    salida.posicionV = vec3(mModelView * vec4(vPosicion, 1.0));
    gl_Position = matrizMVP * vec4 ( vPosicion, 1 );
}