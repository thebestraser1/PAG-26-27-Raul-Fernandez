#version 410

/* Entradas (aún no se usan para iluminación, pues no se tiene) */
in salidaVS
{
 vec3 posicionV;
 vec3 normalV;
} entrada;

/* Uniforms de material */
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float brillo;

/* Salidas */
out vec4 colorFragmento;

/* SUBRUTINAS */
subroutine vec4 fCalcularColor();

subroutine uniform fCalcularColor colorMallaODifuso;

/* Subrutina para asignar color de malla */
/* Por defecto quiero que sea azul */
subroutine ( fCalcularColor )
vec4 colorMalla ()
{
    return vec4 (0, 0, 1, 1);
}


/* Subrutina para asignar color de material (difuso) */
subroutine ( fCalcularColor )
vec4 colorMaterial ()
{
    return vec4 (Kd, 1.0);
}


void main ()
{
    colorFragmento = colorMallaODifuso();
}