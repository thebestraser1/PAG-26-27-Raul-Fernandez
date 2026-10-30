#version 410

/* Entradas */
layout (location = 0) in vec3 vPosicion;
layout (location = 1) in vec3 vNormal;

/* Uniforms */
/* Matriz de modelado trasformación y visión */
uniform mat4 matrizMVP;
uniform mat4 mModelView;

/* Salidas */
/* Normal de cada vértice */
out vec3 normal;

void main ()
{
    normal = vec3(mModelView * vec4(vNormal, 0.0));
    gl_Position = matrizMVP * vec4 ( vPosicion, 1 );
}