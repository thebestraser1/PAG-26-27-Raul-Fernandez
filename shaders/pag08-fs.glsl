#version 410
const float M_PI = 3.14159265358979;

/* Entradas */
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

/* Uniforms de luces */
uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;
uniform vec3 lightPosition;
uniform vec3 lightDirection;
uniform float spotAngle;


/* Salidas */
out vec4 colorFragmento;

/* SUBRUTINAS DE ILUMINACIÓN*/
subroutine vec3 fCalcularColor();

subroutine uniform fCalcularColor colorMallaOLuz;

/* Subrutina para asignar color de malla (negro por defecto)*/
subroutine ( fCalcularColor )
vec3 colorFijo ()
{
    return vec3 (0, 0, 0);
}


/* Subrutina para asignar color ambiental al modelo*/
subroutine ( fCalcularColor )
vec3 luzAmbiental() {
    vec3 ambient = (Ia * Ka);
    return ambient;
}


/* Subrutina para una luz puntual*/
subroutine ( fCalcularColor )
vec3 luzPuntual() {
    vec3 n = normalize( entrada.normalV );

    vec3 l = normalize( lightPosition-entrada.posicionV );
    vec3 v = normalize( -entrada.posicionV );
    vec3 r = reflect( -l, n );

    vec3 diffuse =(Id * Kd * max( dot(l,n), 0.0));
    vec3 specular;

    specular = (Is * Ks * pow( max( dot(r,v), 0.0), brillo));
    return diffuse + specular;
}

/* Subrutina para una luz direccional*/
subroutine ( fCalcularColor )
vec3 luzDireccional() {
    vec3 n = normalize( entrada.normalV );

    vec3 l = normalize(-lightDirection);
    vec3 v = normalize( -entrada.posicionV );
    vec3 r = reflect( -l, n );

    vec3 diffuse =(Id * Kd * max( dot(l,n), 0.0));
    vec3 specular;

    specular = (Is * Ks * pow( max( dot(r,v), 0.0), brillo));
    return diffuse + specular;
}

/* Subrutina para un foco de luz*/
subroutine ( fCalcularColor )
vec3 focoLuz() {
    vec3 l = normalize( lightPosition-entrada.posicionV );
    vec3 d = normalize(lightDirection);
    float cosGamma = cos(spotAngle * M_PI / 180.0);
    float spotFactor = 1.0;

    if (dot(-l, d) < cosGamma) { spotFactor = 0.0; }
    vec3 n = normalize( entrada.normalV );
    vec3 v = normalize( -entrada.posicionV );
    vec3 r = reflect( -l, n );

    vec3 diffuse =(Id * Kd * max( dot(l,n), 0.0));
    vec3 specular = (Is * Ks * pow( max( dot(r,v), 0.0), brillo));
    return spotFactor * (diffuse + specular);
}


void main ()
{
    colorFragmento = vec4(colorMallaOLuz(), 1.0);
}