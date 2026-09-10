#include "archivos.h"

#define ID_CONT    100 //identificador predeterminado para los contenedores [C101, C102, etc]

///lee config.txt y generar puerto.txt -listo
///validar que sea "simulable" - "listo" a medias
///determinar buques, zonas, camiones, tiempo - falta


///MEJORABLE - version temprana q funciona con parametros muy basicos
///mas adelante la optimizo con casos bordes y etc
int generarPuerto(tParametros * param)
{
    FILE* pConf= fopen(CONF_ARCH, "rt");
    FILE*pPuerto= fopen(PUERTO_ARCH, "wt");

    int i, j, idC=ID_CONT, tiempoSobra;

    if(!pConf || !pPuerto)
    {
        if(pConf) fclose(pConf);
        if(pPuerto) fclose(pPuerto);

        return ERROR_ARCH;
    }

    cargarParametros(param, pConf);

    int cantCont= param->maxCamiones;

    //validar q haya el tiempo posible [[por ahora]]
    tiempoSobra=param->minJornada - cantCont * param->tiempoDescargaCont
                    - param->tiempoCargaCamion * param->maxCamiones - param->tiempoReubicacion* cantCont;

    if(tiempoSobra<0)
        return NO_SIMULABLE;


    //seteos iniciales
    fprintf(pPuerto,
            "JORNADA:%d\nMUELLES:%d\nZONAS:%d\nCAPACIDAD_PILA:%d\n",
            param->minJornada, param->cantMuelle, param->cantZona, param->capPila);

    fprintf(pPuerto, "\n[BUQUES]\n");

    for(i=0;i<param->maxBuques;i++)
    {
        fprintf(pPuerto, "B%03d;T=%d;", i+1, i);

        for(j=0;j<=cantCont/2;j++)
        {
            fprintf(pPuerto, "C=C%3d", idC+j+1);

            if(j!=cantCont/2)
                fprintf(pPuerto, ",");
        }
        idC+=ID_CONT;
        cantCont= cantCont/2 + cantCont%2;
        fprintf(pPuerto, "\n");
    }

    fprintf(pPuerto, "\n[CAMIONES]\n");

    //reinicio de variables
    cantCont= param->maxCamiones;
    idC=ID_CONT;
    j=1;
    for(i=0;i<param->maxCamiones;i++)
    {
        fprintf(pPuerto,"K%03d;T=%d;", i+1, rand()% param->minJornada+ 1);

        fprintf(pPuerto, "C=C%03d", idC+j);

        if(i==cantCont/2)
        {
            idC+=ID_CONT;
            cantCont= cantCont/2 + cantCont%2;
            j=1;
        }
        else
            j++;
        fprintf(pPuerto, "\n");
    }


    fclose(pConf);
    fclose(pPuerto);
    return TODO_OK;
}

void cargarParametros(tParametros* param, FILE*pConf)
{
    fscanf(pConf,
           "duracion_jornada_minutos: %d\n\
            cantidad_muelles: %d\n\
            cantidad_zonas_almacenamiento: %d\n\
            capacidad_pila: %d\n\
            maximo_buques: %d\n\
            maximo_contenedores_por_buque: %d\n\
            maximo_camiones: %d\n\
            tiempo_descarga_contenedor: %d\n\
            tiempo_reubicacion_contenedor: %d\n\
            tiempo_carga_camion: %d\n",
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
