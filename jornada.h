#ifndef JORNADA_H_INCLUDED
#define JORNADA_H_INCLUDED

#include "constantes.h"
#include "archivos.h"
#include "TDA/ListaDinamica.h"
#include "TDA/pila_dinamica.h"

#define TAM_NOMBRE     5

typedef struct
{
    char nom[TAM_NOMBRE];
    tBuque* buque;
    bool estado; /// 0 libre ; 1 ocupado
}tMuelle;
// un vector de muelles

typedef struct jornada
{
    char nomBuque[TAM_NOMBRE];
    unsigned tiempoLlegada;
    tPila* contenedores;
}tBuque;

typedef struct
{
    char num[TAM_NOMBRE];
    unsigned tiempoLlegada;
    char pedido[5];// una estructura de contenedor??
    bool atendido;
    //tContenedor pedido;
}tCamiones;
// cola

typedef struct
{
    char nom[TAM_NOMBRE];
    unsigned capacidad;
    unsigned cantContenedores;
    tPila* contenedores;
}tZona;


// bajo los camiones yo, de puerto.txt que dai trae generado
// hago una cola de camiones

typedef struct
{
    char id[TAM_NOMBRE];
}tContenedor;


//Para la "gestion de datos"?
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

//se lo paso a pedro
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

/// para ver el primer u segundo camion bajamos el primero a camionActual y solo vemos el camion primero



// habria que pasar, camiones, muelles,
int nuevaJornada(const char* nomOperador); //propuesta por ahora


#endif // JORNADA_H_INCLUDED
