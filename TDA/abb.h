#ifndef ABB_H_INCLUDED
#define ABB_H_INCLUDED

#include "../constantes.h"

#include "nodo.h"

#define VACIO -1
#define ERROR_ARCHIVO -1
//#define NO_EXISTE -1

typedef tNodoA* tArbol;

void crearArbol(tArbol*pa);

int recorridoPreorden(const tArbol* pa, unsigned n, void*params,
                      void(*accion)(void*,unsigned, unsigned, void*));
int recorridoInorden(const tArbol* pa, unsigned n, void*params,
                      void(*accion)(void*,unsigned, unsigned, void*));
int recorridoPosorden(const tArbol* pa, unsigned n, void*params,
                      void(*accion)(void*,unsigned, unsigned, void*));


int insertarArbolRec(tArbol*pa, const void* elem, unsigned tam, Cmp);
int insertarArbolIte(tArbol*pa, const void* elem, unsigned tam, Cmp);


void vaciarArbol(tArbol* pa);

int buscarElemArbol(const tArbol*p, void *d, unsigned tam, Cmp cmp);

void* buscarEnArbol(const tArbol* p, const void* claveBuscada, Cmp cmp);

int crearArbolDeArchivo(tArbol* pa, const char* nomArch, unsigned tam, Cmp cmp);

int crearArbolDeArchivoRec(tArbol*pa, FILE* pf, int ini,
                            int fin, unsigned tam, Cmp cmp);
int guardarArbolEnArchivo(const tArbol* pa, const char* nomArch);
int guardarArbolEnArchivoRec(const tArbol* pa, FILE* pf);
int buscarConFiltroEnArbol(const tArbol*pa, const void* clave, Cmp cmp, Accion accion, void*paam);

#endif // ABB_H_INCLUDED
