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
#define TAM_ARGUMENTO    5
#define TAM_ARG          5
#define CANT_ARG_COM     0
#define CANT_ARG_OP      2
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
    int (*procesarComando)(char* argv[],tJornada* jornada,const struct tComando* comando);
}tComando;

typedef struct
{
    char codComando[TAM_OPERACION];
    char origen[TAM_ARGUMENTO];
    char destino[TAM_ARGUMENTO];
    char contenedor[TAM_ARGUMENTO];
    int cantArg;
}tRegistroComando;

int ejecutarComandoOperacion(tJornada* jornada,tLista* comandos);

int cargarListaComandos(tLista* pList,tParametros* param);
int leerComandoOperacion(char* linea,char** palabras,tComando* comando,tLista* comandos);

int procesarComandoDES(char* argv[],tJornada* jornada,const struct tComando* comando);
int procesarComandoREU(char* argv[],tJornada* jornada,const struct tComando* comando);
int procesarComandoENT(char* argv[],tJornada* jornada,const struct tComando* comando);


int validarDES(tMuelle* muelle,tZona* zona,tJornada* jornada);
int validarREU(tZona* zona1,tZona* zona2,tJornada* jornada);
int validarENT(tLista* lZonas,tCola* cCamionesEsp,tZona* zona);

int operacionDES(tMuelle* muelle,tZona* zona);
int operacionREU(tZona* pZona1,tZona* pZona2);
int operacionENT(tZona* pZona,tCola* pCamiones);
void comandoESP(tJornada* pJornada);
void comandoVER(tLista* pZona,tLista* pMuelle,tCola* pCamion,tParametros* pParametros,tJornada* pJornada);

int cmpZona(const void* a,const void* b);
int cmpMuelle(const void* a,const void* b);
int cmpComando(const void* a,const void* b);

void actualizarZona(void* actualizado,const void* actualizador);
void actualizarMuelle(void* actualizado,const void* actualizador);

int validarTiempo(const tJornada* jornada,const int tiempoAgregado);
int buscarContenedorEnZonas(tLista* zonas,tZona* zona,tContenedor* contenedor);


#endif // COMANDOS_H_INCLUDED
