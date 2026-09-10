#ifndef PILA_DINAMICA_H_INCLUDED
#define PILA_DINAMICA_H_INCLUDED

#include "nodo.h"
#include "../constantes.h"

#define PILA_VACIA      1

typedef tNodo *tPila;

void crearPila(tPila*p);
void vaciarPila(tPila*p);

int apilar(tPila*p, const void *d, unsigned t);
int desapilar(tPila*p, void *d, unsigned t);

int pilaLlena(const tPila*p, unsigned t);
int pilaVacia(const tPila*p);

int verTope(const tPila*p, void *d, unsigned t);

#endif // PILA_DINAMICA_H_INCLUDED
