#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include "../constantes.h"

#define CAP_INI 10
#define FACTOR_INCR 1.5
#define FACTOR_OCUP 0.25
#define FACTOR_DECR 0.50

#define TODO_OK 0
#define DUPLICADO 1
#define POS_INV 2
#define SIN_MEM 3
#define ERR_ARCH 4
#define ERR_VEC 5


typedef struct
{
    void* v;
    size_t ce;
    size_t cap;
    size_t tamElem;
}tVector;

typedef int (*Cmp)(const void* a,const void*b);
typedef void (*Print)(const void *elem);

bool tVectorCrear(tVector* vec,size_t tamElem); //malloc
int tVectorOrdInsertar(tVector* vec,const void* elem,Cmp cmp); //realloc
int tVectorInsertarAlInicio(tVector* vec,const void* elem); //realloc
int tVectorInsertarAlFinal(tVector* vec,const void* elem); //realloc
int tVectorInsertarEnPos(tVector* vec,const void* elem,int pos); //realloc
int tVectorOrdBuscar(const tVector* vec,void* elem,Cmp cmp);
int tVectorDesordBuscar(const tVector* vec,void* elem,Cmp cmp);
bool tVectorOrdEleminar(tVector* vec,void* elem,Cmp cmp); //realloc
bool tVectorEleminarPos(tVector*,int pos); //realloc
void tVectorMostrar(const tVector* vec,Print print);
void tVectorVaciar(tVector* vec); //realloc
void tVectorDestruir(tVector* vec); //free
int tVectorCE(tVector* vec);
//Memmoria Dinamica
bool ampliarVector(tVector* vec);
void reducirVector(tVector* vec);

#endif // VECTOR_H_INCLUDED
