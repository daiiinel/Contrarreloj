#ifndef JORNADA_H_INCLUDED
#define JORNADA_H_INCLUDED

#include "constantes.h"
#include "archivos.h"
#include "operadores.h"

#include "TDA/pila_dinamica.h"
#include "TDA/ListaDinamica.h"
#include "TDA/abb.h"

//#define FIN_JORNADA 8
#define BLOQUE_OPERATIVO 9
#define SIN_TIEMPO 10
#define SIGUE 11

typedef struct jornada
{
    char nomBuque[MAX_NOMBRE_BUQUE];
    unsigned tiempoLlegada;
    tPila contenedores;
}tBuque;

tPila* tBuqueVerPila(tBuque* pBuque);

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


tBuque* tMuelleVerBuque(tMuelle* muelle);


typedef struct
{
    char nom[MAX_NOMBRE_ZONA];
    unsigned capacidad;
    unsigned cantContenedores;
    tPila contenedores;
}tZona;

tPila* tZonaVerPila(tZona* pZona);

// bajo los camiones yo, de puerto.txt que dai trae generado
// hago una cola de camiones

typedef struct
{
    char id[MAX_NOMBRE_CONT];
}tContenedor;


//Para la "gestion de datos"?
//Uno para el archivo de operadores y el otro para el archivo de jornadas
//typedef struct
//{
//    char nomOperador[MAX_NOMBRE_OP];
//    unsigned cantMov,
//             puntuacion;
//
//}tEstadisticasOperadores;
typedef struct
{
    unsigned numJornada;
    char nomOperador[MAX_NOMBRE_OP];
    unsigned puntuacionObtenida,
             movimientosRealizados, cantMov;
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
    tCola buquesEnTiempo;
    tCola camionesEnTiempo;
    tLista lZonas;
}tJornada;

//typedef struct
//{
//    char nomre[4];
//    unsigned tiempoAConsumir;
//    char org[3];
//    char des[3];
//}tComando;

// habria que pasar, camiones, muelles,
int nuevaJornada(tEstadisticasOperadores*datosOperador, tJornada *jor);

int bajarPuertoTxt(tJornada*jor);

//Jornada
int desarrolloJornada(tJornada* jor);
void verKPrimeros(tCola* camionesEspera);
int estadoDeJornada(tJornada *jor);
int hayBloqueoOperativo(tJornada* jor);

//Arrivos
int procesarArrivos(tJornada *jor);
int arrivoCamiones(tJornada* jor);
int arrivoBuques(tJornada* jor);
int acercarBuques(tJornada* jor);
int cmpMuelleLibre(const void* reg1, const void* estado);
int cmpMuelleNom(const void* reg1,const void* reg2);
void actualizarMuelleMartu(void* viejo, void* nuevo);
int hayAtracosDeMuelles(tLista* muelles);


//Zonas
void mostrarTodasLasPilasDeZonas(tLista* lisZonas);
void mostrarUnaZona(void *datoZona, void *extra);
int capacidadDeZonas(tLista* zonas);


//Fin Jornada
int mostrarResultadoJornada(tJornada *jor, int razon);
// Historial de jornada puede que no este terminado si hay que aregar algo
int archivoFinJornada(tJornada *jor);

void contarEspacioZona(void* info,void* param);
void muelleEnUso(void* info,void* param);
int buscarEnPila(tPila *pp,void *elem, unsigned tam,Cmp cmp);
int buscarContenedorEnZonas(tLista* zonas,tZona* zona,tContenedor* contenedor);
int cmpContenedor(const void* a,const void* b);

#endif // JORNADA_H_INCLUDED
