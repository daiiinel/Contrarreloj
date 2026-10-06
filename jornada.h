#ifndef JORNADA_H_INCLUDED
#define JORNADA_H_INCLUDED

#include "constantes.h"
#include "archivos.h"

typedef struct
{
    char nom[3];
    tBuque* buque;
    bool estado; /// 0 libre ; 1 ocupado
}tMuelle;
// un vector de muelles

typedef struct jornada
{
    char nomBuque[5];
    unsigned tiempoLlegada;
    tPila* contenedores
    //una pila de contenedores?
}tBuque;

typedef struct
{
    char num[3];
    unsigned tiempoLlegada;
    char pedido[5];// una estructura de contenedor??
    bool atendido;
    //tContenedor pedido;
}tCamiones;
// cola

typedef struct
{
    char nom[5];
    unsigned capacidad;
}tZona;


// bajo los camiones yo, de puerto.txt que dai trae generado
// hago una cola de camiones
/*

por ahora solo hay un id?
typedef struct
{
    char id[5];
}tContenedor;

*/



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
    int tiempo;
    /// paso la estructura de los muelles
    /// de los contenedores
    ///
}tJornada;

/// para ver el primer u segundo camion bajamos el primero a camionActual y solo vemos el camion primero



// habria que pasar, camiones, muelles,
int nuevaJornada(const char* nomOperador); //propuesta por ahora


#endif // JORNADA_H_INCLUDED
