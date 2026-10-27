#include "jornada.h"
#include "TDA/colaDinamica.h"
#include "TDA/ListaDinamica.h"

#include "archivos.h"

///para probar que se bajaron correctamente¿

/**
void mostrarCamion(tCamiones*k)
{
    printf("ID: %s - T: %d - PED: %s\n", k->num, k->tiempoLlegada, k->pedido);
}
void mostrarBuque(tBuque*k)
{
    printf("ID: %s - T: %d -", k->nomBuque , k->tiempoLlegada);
}
void mostrarPilaContenedores(tPila* p)
{
    tPila aux;
    crearPila(&aux);
    char cad[5];

    printf("[ ");

    while(pilaVacia(p)!=PILA_VACIA)
    {
        desapilar(p, cad, sizeof(cad));
        printf("%s ", cad);
        apilar(&aux, cad, sizeof(cad));
    }
    printf("]\n");

    while(pilaVacia(&aux) != PILA_VACIA)
    {
        desapilar(&aux, cad, sizeof(cad));
        apilar(p, cad, sizeof(cad));
    }
}
*/

//propuesta/pseudocodigo
int nuevaJornada(const char* nomOperador, tJornada* jor)
{
    //estructura que contiene datos de puerto.txt y config.txt
    tParametros param;

    tLista lMuelles;
    tCola cCamiones;

    tMuelle auxMuelle;

    tZona auxZona;
    tLista lZonas;

    tCola cBuques;

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
    ///buques
    crearCola(&cBuques);

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

    //Jornada
    strcpy(jor->operador, nomOperador);
    jor->tiempo=0;
    jor->tiempo_maximo= param.minJornada;
    jor->puntaje=0;
    jor->lMuelles= lMuelles;
    jor->cCamiones= cCamiones;
    jor->lZonas= lZonas;
    jor->cBuques= cBuques;


    bajarPuertoTxt(jor);
    ///para probar que se bajaron correctamente¿ (auxiliar para probar cosas para nosotros, no para el tp¿)

    //ver camiones
    /**
    tCamiones caux;
    while(colaVacia(&jor->cCamiones)!=COLA_VACIA)
    {
        sacarDeCola(&jor->cCamiones, &caux,sizeof(tCamiones));

        mostrarCamion(&caux);
        system("pause");
    }
    */

    //ver buques con sus pilas
    /**
    tBuque baux;
    while(colaVacia(&jor->cBuques)!=COLA_VACIA)
    {
        sacarDeCola(&jor->cBuques, &baux,sizeof(tBuque));

        mostrarBuque(&baux);
        mostrarPilaContenedores(&baux.contenedores);

        system("pause");
    }
    */

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

int bajarPuertoTxt(tJornada*jor)
{
    FILE* pPuerto= fopen(PUERTO_ARCH, "rt");
    char linea[TAM_LINEA];

    int tiempoJornada, cantMuelles, cantZonas, capPila; //innecesarias¿

    tBuque buq;
    tCamiones cam;

    char cadCont[MAX_NOMBRE_CONT];

    char*pInicioCont; //puntero al inicio de contenedores en una cadena ->despues del ';'
    char* pCont;

    if(!pPuerto)
        return ERROR_ARCH;

    fscanf(pPuerto, "%*[^:]:%d\n%*[^:]:%d\n%*[^:]:%d\n%*[^:]:%d\n",
           &tiempoJornada, &cantMuelles, &cantZonas, &capPila);

    /* //--variante sin necesidad de asignar variables inusables¿
    fgets(linea, TAM_LINEA, pPuerto); //[tiempo jornad
    fgets(linea, TAM_LINEA, pPuerto); //[cantmuelle
    fgets(linea, TAM_LINEA, pPuerto); //[zonas
    fgets(linea, TAM_LINEA, pPuerto); //[capila
    fgets(linea, TAM_LINEA, pPuerto); //\n*/

    fgets(linea, TAM_LINEA, pPuerto); //[BUQUES]


    fgets(linea, TAM_LINEA, pPuerto); //B001;T=4;C=C101,C=C102
    do
    {
        sscanf(linea, "%[^;];T=%d;", buq.nomBuque, &buq.tiempoLlegada);

        pInicioCont= strrchr(linea, ';');
        pCont= strrchr(linea, ',');

        crearPila(&buq.contenedores);

        while (pCont != NULL)
        {
            sscanf(pCont+1, "C=%s", cadCont);
            apilar(&buq.contenedores, cadCont, 5);

            *pCont = '\0';
            pCont = strrchr(linea, ',');
        }
        if(pInicioCont!=NULL)
        {
            sscanf(pInicioCont+1, "C=%s", cadCont);
            apilar(&buq.contenedores, cadCont, 5);
        }

        ponerEnCola(&(jor->cBuques), &buq, sizeof(tBuque));

        fgets(linea, TAM_LINEA, pPuerto); //B001;T=4;C=C101,C=C102

    }while(strcmpi(linea, "\n")!=0);


     fgets(linea, TAM_LINEA, pPuerto); //[CAMIONES]

    while(fgets(linea, TAM_LINEA, pPuerto))
    {  //K001;T=0;C=C101

        sscanf(linea, "%[^;];T=%d;C=%s\n", cam.num, &cam.tiempoLlegada, cam.pedido);

        cam.atendido= false;

        ponerEnCola(&(jor->cCamiones), &cam, sizeof(tCamiones));
    }

    fclose(pPuerto);
    return TODO_OK;
}



