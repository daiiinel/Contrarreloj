#ifndef NODO_H_INCLUDED
#define NODO_H_INCLUDED

//nodo genérico para utilizar la redefinicion en pilas,colas y listas dinámicas
typedef struct sNodo
{
    void* info;
    unsigned tam;
    struct sNodo* sig;
}tNodo;

#endif // NODO_H_INCLUDED
