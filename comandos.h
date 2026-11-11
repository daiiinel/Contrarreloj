#ifndef COMANDOS_H_INCLUDED
#define COMANDOS_H_INCLUDED

#include "constantes.h"
#include "archivos.h"
#include "jornada.h"

typedef enum
{
    nomArg, arg1, arg2
}tArgumentos;

#define TAM_OPERACION    4
#define TAM_COMANDO      4
#define TAM_ARGUMENTO    9
#define TAM_ARG          9
#define CANT_ARG_COM     1
#define CANT_ARG_OP      3
#define TIEMPO_ESP       1

#define ERROR_INGRESO_COMANDO -1
#define CODIGO_OP "OPERADOR>"
//OPERACIONES
#define ENT "ENT"
#define DES "DES"
#define REU "REU"
//COMANDOS
#define VER "VER"
#define ESP "ESP"

typedef struct tComando
{
    char nomComando[TAM_OPERACION];
    int cantArg;
    int tiempoUso;
    int (*procesarComando)(char* argv[],tJornada* jornada,const struct tComando* comando,tLista* historial);
}tComando;

typedef struct
{
    char codComando[TAM_OPERACION];
    char origen[TAM_ARGUMENTO];
    char destino[TAM_ARGUMENTO];
    char contenedor[TAM_ARGUMENTO];
    int tiempoInicio;
    int tiempoFinal;
}tHistoriaComando;

int ejecutarComandoOperacion(tJornada* jornada,tLista* comandos,tLista* historial);

int cargarVectorComandos(tLista* pList,tParametros* param);
int leerComandoOperacion(char* linea,char** palabras,tComando* comando,tLista* comandos);

int procesarComandoDES(char* argv[],tJornada* jornada,const struct tComando* comando,tLista* historial);
int procesarComandoREU(char* argv[],tJornada* jornada,const struct tComando* comando,tLista* historial);
int procesarComandoENT(char* argv[],tJornada* jornada,const struct tComando* comando,tLista* historial);
int procesarComandoVER(char* argv[],tJornada* pJornada,const struct tComando* comando,tLista* historial);
int procesarComandoESP(char* argv[],tJornada *pJornada,const struct tComando* comando,tLista* historial);


int validarDES(tMuelle* muelle,tZona* zona,tJornada* jornada);
int validarREU(tZona* zona1,tZona* zona2,tJornada* jornada);
int validarENT(tLista* lZonas,tCola* cCamionesEsp,tZona* zona);

int operacionDES(tMuelle* muelle,tZona* zona, tContenedor *cont, tJornada* jornada);
int operacionREU(tZona* pZona1,tZona* pZona2,tContenedor* conte);
int operacionENT(tZona* pZona,tCamiones* camion,tContenedor* conte,tCola* pCamiones);
void comandoESP(tJornada* pJornada);

int cmpZona(const void* a,const void* b);
int cmpMuelle(const void* a,const void* b);
int cmpComando(const void* a,const void* b);

void actualizarZona(void* actualizado,const void* actualizador);
void actualizarMuelle(void* actualizado,const void* actualizador);

int validarTiempo(const tJornada* jornada,const int tiempoAgregado);
int buscarContenedorEnZonas(tLista* zonas,tZona* zona,tContenedor* contenedor);
void guardarComando(const char* nom,char*orig,char*dest,char*conte,int ini,int fin,tLista* historial);



#endif // COMANDOS_H_INCLUDED
