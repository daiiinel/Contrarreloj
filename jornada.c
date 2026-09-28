#include "jornada.h"
#include "TDA/colaDinamica.h"

//propuesta/pseudocodigo
int nuevaJornada(const char* nomOperador)
{
    //estructura que contiene datos de puerto.txt y config.txt
    //
    tParametros param;
    tMuelle muelles, aux;
    tCamiones camiones;
    unsigned i=1, tiempo=0;

    if(generarPuerto(&param)!=TODO_OK)
        return NO_SIMULABLE;
    
    crearCola(&muelles);
     
    while(i< param.cantMuelle + 1)
    {
        sprintf(aux.nom,"M%d",i);
        aux.estado=0;
        ponerEnCola(&muelles, &aux,sizeof(tMuelle));
        i++;
    }

    //para camiones
    //para las zonas (al principio vacias)

    ///param no trae cantidad de contenedores
    //por ahora esa cantidad se determina como cantCont= param->maxCamiones, pq hay un solo cont por camion�

    //cargarOperador(nomOperador); //funcion ajena de operadores.h



    //inicializar reloj(t=0), zonas, buques, camiones_en_espera, etc
    //lista de mov
    //procesar lo q suceda cuando t=0

    while(tiempo < param.minJornada)
    {
        ///se hace todo
    }

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
