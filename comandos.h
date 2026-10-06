#ifndef COMANDOS_H_INCLUDED
#define COMANDOS_H_INCLUDED

#include "constantes.h"
#include "TDA/colaDinamica.h"
#include "TDA/pila_dinamica.h"
#include "TDA/ListaDinamica.h"
#include "archivos.h"
#include "jornada.h"
#include "TDA/Vector.h"

#define TAM_OPERACION 4
#define TAM_ARGUMENTO 5
#define ENT 6
#define DES 7
#define REU 8
#define VER 9
#define ESP 10

typedef struct 
{
    int codOperacion;
    char argumento1[TAM_ARGUMENTO];
    char argumento2[TAM_ARGUMENTO];
    bool band;
}tOperacion;


int ejecutarComandoOperacion(tLista* zona,tVector* muelle,tCola* camion,tParametros* parametros,tJornada* jornada);

void ingresarComandoOperacion(tOperacion* operacion);

int validarDES(tVector* pMuelles,tLista* pZonas,tOperacion* pOper);
int validarREU(tLista* pZonas,tOperacion pOper);
int validarENT(tLista* pZonas,tCola* pCamiones,tOperacion* pOper);

int operacionDES(tZona* pZona,tMuelle* pMuelle);
int operacionREU(tZona* pZona1,tZona* pZona2);
int operacionENT(tZona* pZona,tCamiones pCamion);
void comandoESP(tJornada* pJornada);
void comandoVER(tLista* pZona,tVector* pMuelle,tCola* pCamion,tParametros* pParametros,tJornada* pJornada);

int cmpZona(const void* a,const void* b);




#endif // COMANDOS_H_INCLUDED
