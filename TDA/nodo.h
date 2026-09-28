#ifndef NODO_H_INCLUDED
#define NODO_H_INCLUDED

//nodo gen�rico para utilizar la redefinicion en pilas,colas y listas din�micas
typedef struct sNodo
{
    void* info;
    unsigned tam;
    struct sNodo* sig;
}tNodo;

#endif // NODO_H_INCLUDED
