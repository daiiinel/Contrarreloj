#include "jornada.h"
#include "TDA/colaDinamica.h"
#include "TDA/ListaDinamica.h"

//propuesta/pseudocodigo
int nuevaJornada(const char* nomOperador, tJornada jor)
{
    //estructura que contiene datos de puerto.txt y config.txt
    tParametros param;
    
    tLista lMuelles;
    tCola cCamiones;

    tMuelle auxMuelle;

    tZona auxZona;
    tLista lZonas;

    unsigned i=1;

    if(validaConfigYGeneraPuerto(&param)!=TODO_OK)
        return NO_SIMULABLE;

    ///Muelles
    tListaCrear(&lMuelles);

    while(i< param.cantMuelle + 1)
    {
        sprintf(auxMuelle.nom,"M%d",i);
        auxMuelle.estado=0;
        if(tListaInsertarAlFinal(&lMuelles, &auxMuelle,sizeof(tMuelle))!= TODO_OK)
            return NO_SIMULABLE;
        i++;
    }

    ///Camiones
    crearCola(&cCamiones);

    ///Zonas
    tListaCrear(&lZonas);
    i=1;// lo vuelvo a setear
    while(i < param.cantZona)
    {
        sprintf(auxZona.nom, "Z%d", i);
        auxZona.capacidad=param.capPila;
        crearPila(&auxZona.contenedores);
        if(tListaInsertarAlFinal(&lZonas,&auxZona,sizeof(tZona))!= TODO_OK)
             return NO_SIMULABLE;
        i++;
    }
    ///Jornada
    strcpy(jor.operador, nomOperador);
    jor.tiempo=0;
    jor.tiempo_maximo= param.minJornada;
    jor.puntaje=0;
    jor.lMuelles= lMuelles;
    jor.cCamiones= cCamiones;
    jor.lZonas= lZonas;

    // una cola auzliar donde pongo todos los que buques que van a llegar
    // cuando sea el tiempo de llegada
    // lo desencolo de la auxiliar y lo encolo en la cola POSTA de espera para un muelle libre


    //inicializar reloj(t=0), zonas, buques, camiones_en_espera, etc
    //lista de mov

    //while t<puerto.durMax y no haya bloqueo y no haya terminado todos los procesos pendientes
        /*if pedirYEjecutarComando(parametros_necesarios) != TODO_OK
            return ERROR_J;
        actualizar tiempo dependiendo el tipo de operacion
        procesar lo q suceda en el nuevo tiempo
*/
    //al salir del while, actualizar estadisticas y guardarlas
    //actOperador(&operador); //funcion ajena, mandariamos a operadores.c

    return TODO_OK;
}

int procesarArrivo()
{

    return TODO_OK;
}
