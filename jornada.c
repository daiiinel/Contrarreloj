#include "jornada.h"
#include "TDA/colaDinamica.h"

//propuesta/pseudocodigo
int nuevaJornada(const char* nomOperador)
{
    //estructura que contiene datos de puerto.txt y config.txt
    //
    tParametros param;
    tCola cMuelles;
    tMuelle aux;
    tCamiones camiones;
    tBuque buque;
    char op[4];
    unsigned i=1, tiempo=0, tiempoAConsumir;

    if(validaConfigYGeneraPuerto(&param)!=TODO_OK)
        return NO_SIMULABLE;

    crearCola(&cMuelles);

    while(i< param.cantMuelle + 1)
    {
        sprintf(aux.nom,"M%d",i);
        aux.estado=0;
        ponerEnCola(&cMuelles, &aux,sizeof(tMuelle));
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

    tiempo=param.minJornada; //pa evitar el bucle¿
    while(tiempo < param.minJornada)
    {
        ///se hace todo

        printf("accion pibe: ");
        //scanf("%s", &op);
        // validar accion validarAccion()
        // tiempoAConsumir  = llamo a tu funcion te paso el tiempo,la accion?
        // vos me lo traes sumado tiempo=2
        // hay arrivos dentro de ese tiempo?
        // ecolo,apilo, enlisto, lo que llegue
        // Hacer la accion
        // antes de hacer la accion, preguntamos si llega algo durante la accion realizandose,
        // sisi, avanzamos hasta el arrivo y lo hacemos y despues terminamos la accion

        if(tiempo + tiempoAConsumir > camiones.tiempoLlegada)
        {

        }
        // t= 2
        // arrivo en el t=4
        // t=4 hacemos la llegada, avanzamos hasta el tiempo a consumir
        // t= 7



        // sumo el tiempo + tiempoAConsumir
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
