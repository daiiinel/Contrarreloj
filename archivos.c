#include "archivos.h"

#define ID_CONT    100 //identificador predeterminado para los contenedores [C101, C102, etc]

///lee config.txt y generar puerto.txt -listo
///validar que sea "simulable" - listo
///determinar buques, zonas, camiones, tiempo - list


///MEJORABLE/OPTIMIZABLE - version q funciona¿
///optimizable-> asegurar q hayan mas casos en los que los seteos no sean tan chicos
//ej: 1contxbuq, 1 buque, 1 camion -> 100% valido y simulable pero muy facil¿¿
int validaConfigYGeneraPuerto(tParametros * param)
{
    FILE* pConf= fopen(CONF_ARCH, "rt");

    if(!pConf)
        return ERROR_ARCH;

    cargarParametros(param, pConf);

    //casos insimulabless
    if(param->maxCamiones > param->maxBuques * param->maxContPorBuques)
        return NO_SIMULABLE;

    if(param->maxBuques > 0 && param->cantMuelle==0)
        return NO_SIMULABLE;//-- buques en cola infinitamente

    if(param->cantZona==0 || param->capPila==0)
        return NO_SIMULABLE; //sin esp para el comando DES

    if(param->tiempoDescargaCont + param->tiempoCargaCamion > param->minJornada)
        return NO_SIMULABLE;

    if(param->maxContPorBuques > param->cantZona * param->capPila)
        return NO_SIMULABLE;

    generarPuertoTxT(param);

    fclose(pConf);
    return TODO_OK;
}

int generarPuertoTxT(tParametros* param)
{
    //para iteraciones en ciclos
    int i, j, idC = ID_CONT, idAsignado=0;
    //seteos de parametrs (cantidades)
    int cantBuques, contXBuq, cantContenedores, cantCamiones;
    //acumuladores de tiempo para q se genere en orden temporal¿
    int tiempoBuque=0, tiempoCamion = 0;

    int limSeguro;

    tCola bancoContenedores;

    FILE* pPuerto = fopen(PUERTO_ARCH, "wt");

    if(!pPuerto)
        return ERROR_ARCH;

    cantBuques = rand() % param->maxBuques + 1;
    contXBuq = rand() % param->maxContPorBuques + 1;
    cantContenedores = cantBuques * contXBuq;

    //nunca pedir mas camiones q contenedores disponibles
    cantCamiones = MIN(param->maxCamiones, cantContenedores);

    //al tiempo total, le restamos el ultimo intervalo de tiempo posible para una accion
    //(carga y descarga), para q no llegue un buque en t=29 cuando el tiempo total es 30 por ej
    limSeguro= param->minJornada -(param->tiempoDescargaCont +param->tiempoCargaCamion);

    crearCola(&bancoContenedores);

    fprintf(pPuerto,
            "JORNADA:%d\nMUELLES:%d\nZONAS:%d\nCAPACIDAD_PILA:%d\n",
            param->minJornada, param->cantMuelle, param->cantZona, param->capPila);

    fprintf(pPuerto, "\n[BUQUES]\n");

    for(i = 0; i <cantBuques; i++)
    {
        //reparte llegadas de buques
        tiempoBuque+= rand()%(limSeguro/cantBuques+1);
        fprintf(pPuerto, "B%03d;T=%d;", i+1, tiempoBuque);

        for(j=0; j<contXBuq; j++)
        {
            int idNuevo = idC + j + 1;
            ponerEnCola(&bancoContenedores, &idNuevo, sizeof(int));

            fprintf(pPuerto, "C=C%03d", idNuevo);

            if(j!= contXBuq-1)
                fprintf(pPuerto, ",");
        }
        idC += ID_CONT;
        fprintf(pPuerto, "\n");
    }

    fprintf(pPuerto, "\n[CAMIONES]\n");

    for(i=0; i<cantCamiones; i++)
    {
        //saltos de tiempo para q queden encolados en orden
        tiempoCamion+= rand()%(param->minJornada/cantCamiones +1);

        sacarDeCola(&bancoContenedores, &idAsignado, sizeof(int));
        fprintf(pPuerto,"K%03d;T=%d;C=C%03d\n", i+1, tiempoCamion, idAsignado);
    }

    vaciarCola(&bancoContenedores);
    fclose(pPuerto);
    return TODO_OK;
}

void cargarParametros(tParametros* param, FILE*pConf)
{
    fscanf(pConf,
           "%*[^:]: %d\n\
            %*[^:]: %d\n\
            %*[^:]: %d\n\
            %*[^:]: %d\n\
            %*[^:]: %d\n\
            %*[^:]: %d\n\
            %*[^:]: %d\n\
            %*[^:]: %d\n\
            %*[^:]: %d\n\
            %*[^:]: %d\n",
            &param->minJornada,
            &param->cantMuelle,
            &param->cantZona,
            &param->capPila,
            &param->maxBuques,
            &param->maxContPorBuques,
            &param->maxCamiones,
            &param->tiempoDescargaCont,
            &param->tiempoReubicacion,
            &param->tiempoCargaCamion);
}
