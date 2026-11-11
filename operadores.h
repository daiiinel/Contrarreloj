#ifndef OPERADORES_H_INCLUDED
#define OPERADORES_H_INCLUDED

#include "constantes.h"

#include "TDA/ListaDinamica.h"
#include "TDA/abb.h"

typedef struct
{
    char nombre[MAX_NOMBRE_OP];
    unsigned id,
             pos;
}tOperadorIdx;

typedef struct
{
    char nomOperador[MAX_NOMBRE_OP];
    unsigned    id,
                puntuacion;
}tEstadisticasOperadores;

typedef struct
{
    tLista lNombres;
    unsigned cant;
}tRepetidos;


int actualizarOperador(tEstadisticasOperadores* datosOperador, tArbol*abb);

int buscarOperador(const char* nomOperador, tArbol* abb, FILE* pf, tEstadisticasOperadores* datosOperador);
int gestionarOperador(char* nomOperador, tArbol*abb, tEstadisticasOperadores*datosOperador);

int seleccionaOperador(tEstadisticasOperadores*datosOperador, tRepetidos*encontrados, FILE*pf);
int crearOperador(const char* nomOperador, tArbol* abb, FILE* pfOperadores,tEstadisticasOperadores* datosOperador);

long obtenerCantidadRegFILE(FILE* pf, size_t tam);
int inicializarIndiceOperadores(tArbol* pa);
int reconstruirIndice(tArbol* pa);

long cantidadRegistros(const char* nomArch, unsigned tam);

int cmpOperadorIdx(const void* a, const void* b);
int cmpOperadorNombre(const void* a, const void* b);
void mostrarYEnlistar(void* info, void* param);
void imprimirOperadorRanking(void* info, void* param);

int verRanking(tArbol* arbolIdx);
int cmpPorPuntaje(const void* a, const void* b);
void accionVolcarALista(void* infoNodo, unsigned tamNodo, unsigned nivel, void* params);


#endif // OPERADORES_H_INCLUDED
