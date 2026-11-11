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

/* Uniforms de luces */
uniform vec3 Ia;
uniform vec3 Id;
uniform vec3 Is;
uniform float shininess;
uniform vec3 lightPosition;
uniform vec3 lightDirection;
uniform float spotAngle;


/* Salidas */
out vec4 colorFragmento;

/* SUBRUTINAS DE ILUMINACIÓN*/
subroutine vec4 fCalcularColor();

subroutine uniform fCalcularColor colorMallaOLuz;

/* Subrutina para asignar color de malla (azul por defecto)*/
subroutine ( fCalcularColor )
vec4 colorMalla ()
{
    return vec4 (0, 0, 1);
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
    vec3 n = normalize( normal );

    vec3 l = normalize( lightPosition-position );
    vec3 v = normalize( -position );
    vec3 r = reflect( -l, n );

    vec3 diffuse =(Id * Kd * max( dot(l,n), 0.0));
    vec3 specular;

    specular = (Is * Ks * pow( max( dot(r,v), 0.0), shininess));
    return diffuse + specular;
}


subroutine ( fCalcularColor )
vec3 luzDireccional() {
    vec3 n = normalize( normal );

    vec3 l = -lightDirection;
    vec3 v = normalize( -position );
    vec3 r = reflect( -l, n );

    vec3 diffuse =(Id * Kd * max( dot(l,n), 0.0));
    vec3 specular;

    specular = (Is * Ks * pow( max( dot(r,v), 0.0), shininess));
    return diffuse + specular;
}


subroutine ( fCalcularColor )
vec3 focoLuz() {
    vec3 l = normalize( lightPosition-position );
    vec3 d = lightDirection;
    float cosGamma = cos(spotAngle * M_PI / 180.0);
    float spotFactor = 1.0;

    if (dot(-l, d) < cosGamma) { spotFactor = 0.0; }
    vec3 n = normalize( normal );
    vec3 v = normalize( -position );
    vec3 r = reflect( -l, n );

    vec3 diffuse =(Id * Kd * max( dot(l,n), 0.0));
    vec3 specular = (Is * Ks * pow( max( dot(r,v), 0.0), shininess));
    return spotFactor * (diffuse + specular);
}


void main ()
{
    colorFragmento = vec4(colorMallaOLuz(), 1.0);
}