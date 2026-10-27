#ifndef JORNADA_H_INCLUDED
#define JORNADA_H_INCLUDED

#include "constantes.h"
#include "archivos.h"
#include "TDA/pila_dinamica.h"
#include "TDA/ListaDinamica.h"

typedef struct jornada
{
    char nomBuque[MAX_NOMBRE_BUQUE];
    unsigned tiempoLlegada;
    tPila contenedores;
}tBuque;

typedef struct
{
    char nom[MAX_NOMBRE_MUELLE];
    tBuque* buque;
    bool estado; /// 0 libre ; 1 ocupado
}tMuelle;

typedef struct
{
    char num[MAX_NOMBRE_CAMION];
    unsigned tiempoLlegada;
    char pedido[MAX_NOMBRE_ZONA];// una estructura de contenedor??
    bool atendido; // 0 no atendido ;   1 atendido
}tCamiones;
//los camiones vienen en cola
typedef struct
{
    char nom[MAX_NOMBRE_ZONA];
    unsigned capacidad;
    tPila contenedores;
}tZona;


// bajo los camiones yo, de puerto.txt que dai trae generado
// hago una cola de camiones

//Uno para el archivo de operadores y el otro para el archivo de jornadas
typedef struct
{
    char *nom;
    unsigned cantMov,
             puntuacion;

}tEstadisticasOperadores;
typedef struct
{
    unsigned numJornada;
    char *nomOperador;
    unsigned puntuacionObtenida,
             movimientosRealizados;
}tEstadisticaJornada;

typedef struct
{
    char operador[MAX_NOMBRE_OP];
    int tiempo;
    int tiempo_maximo;
    int puntaje;
    tLista lMuelles;
    tCola cBuques;
    tCola cCamiones;
    tLista lZonas;
}tJornada;

typedef struct
{
    char nomre[4];
    unsigned tiempoAConsumir;
    char org[3];
    char des[3];
}tComando;

// habria que pasar, camiones, muelles,
int nuevaJornada(const char* nomOperador, tJornada *jor);
int procesarArrivo();

int bajarPuertoTxt(tJornada*jor);

#endif // JORNADA_H_INCLUDED
