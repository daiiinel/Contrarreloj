#ifndef COMANDOS_H_INCLUDED
#define COMANDOS_H_INCLUDED

#include "constantes.h"
#include "TDA/colaDinamica.h"
#include "TDA/pila_dinamica.h"
#include "TDA/ListaDinamica.h"
#include "archivos.h"
#include "jornada.h"
#include "TDA/Vector.h"

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
#define ERROR_COMANDO         -2
#define CODIGO_OP "OPERADOR>"
//OPERACIONES
#define ENT "ENT"
#define DES "DES"
#define REU "REU"
//COMANDOS
#define VER "VER"
#define ESP "ESP"

typedef struct 
{
    char nomComando[TAM_OPERACION];
    int cantArg;
    int tiempoUso;
    int (*procesarComando)(const char* argv[],tJornada* jornada,const tComando* comando,tCola* buqsEsp,tCola* camsEsp);
}tComando;

typedef struct 
{
    char codComando[TAM_OPERACION];
    char origen[TAM_ARGUMENTO];
    char destino[TAM_ARGUMENTO];
    char contenedor[TAM_ARGUMENTO];
    int cantArg;
}tRegistroComando;

int ejecutarComandoOperacion(tJornada* jornada,tVector* comandos,tCola* buquesEsp, tCola* camionEsp);

int cargarComandos(tParametros* param);
int leerComandoOperacion(char* linea,char** palabras,tComando* comando,tVector* comandos);

int procesarComandoDES(const char* argv[],tJornada* jornada,tComando* comando,tCola* buquesEsp, tCola* camionEsp);

int validarDES(tMuelle* muelle,tZona* zona,tJornada* jornada);
int validarREU(tZona* zona1,tZona* zona2,tJornada* jornada);
int validarENT(tZona* zona,tCamiones* camion,tJornada* jornada);

int operacionDES(tMuelle* muelle,tZona* zona);
int operacionREU(tZona* pZona1,tZona* pZona2);
int operacionENT(tZona* pZona,tCamiones pCamion);
void comandoESP(tJornada* pJornada);
void comandoVER(tLista* pZona,tVector* pMuelle,tCola* pCamion,tParametros* pParametros,tJornada* pJornada);

int cmpZona(const void* a,const void* b);
int cmpMuelle(const void* a,const void* b);
int cmpComando(const void* a,const void* b);

void actualizarZona(void* actualizado,const void* actualizador);
void actualizarMuelle(void* actualizado,const void* actualizador);

int validarTiempo(tJornada* jornada,tComando* comando);




#endif // COMANDOS_H_INCLUDED
