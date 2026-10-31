#include "comandos.h"

int cargarVectorComandos(tVector* vec,tParametros* param)
{
    int codRet,cantComandos,i=0;
    tComando vecComandos[] = 
    {
        {DES,CANT_ARG_OP,param->tiempoDescargaCont},procesarComandoDES,
        {REU,CANT_ARG_OP,param->tiempoReubicacion,NULL},
        {ENT,CANT_ARG_OP,param->tiempoCargaCamion,NULL},
        {ESP,CANT_ARG_COM,TIEMPO_ESP,NULL},
        {VER,CANT_ARG_COM,0,NULL}

    };
    cantComandos = sizeof(vecComandos)/sizeof(tComando);
    codRet = tVectorCrear(vec,sizeof(tComando));
    if(codRet)
    {
        return codRet;
    }
    while(codRet == 0 && i < cantComandos)
    {
        codRet = tVectorOrdInsertar(vec,vecComandos+i,cmpComando);
    }
    if(codRet)
    {
        return codRet;
    }
    return TODO_OK;
}

int ejecutarComandoOperacion(tJornada* jornada,tVector* comandos,tCola* buquesEsp, tCola* camionEsp)
{
    char linea[TAM_LINEA];
    char* argumentos[TAM_ARG];
    tComando comando;
    

    //El usuario ingresa un comando
    if(leerComandoOperacion(linea,argumentos,&comando,comandos))
    {
        return ERROR_INGRESO_COMANDO;
    }
    //utilizo el puntero a funcion de la estructura
    //Esta funcion se encarga de validar y ejecutar el comando
    comando.procesarComando(argumentos,jornada,&comando,buquesEsp,camionEsp);
    

    //Guardar historia
    return TODO_OK;
}

int leerComandoOperacion(char* linea,char** palabras,tComando* comando,tVector* comandos)
{
    char* act;
    int cant = 0;

    printf("OPERADOR> ");
    //Leo el ingreso y agrego '\0'al final de cadena
    if(fgets(linea,TAM_LINEA+1,stdin) == NULL)
    {
        return 1;
    }
    act = strchr(linea,'\n');
    if(act == NULL)
    {
        return -1;//linea larga
    }
    *act = '\0';

    //Didido la cadena en subcadenas con strtok y las guardo en "palabras"
    act = strtok(linea," ");
    while(act != NULL && cant <= TAM_ARG)
    {
        *(palabras + cant) = act;
        cant++;
        act = strtok(NULL," ");
    }

    //Valido si existe la opracion
    strcpy(comando->nomComando,*palabras);
    ///Recordar:Agregar una funcion que copie sin pasarse del tamanio de liinea
    if(tVectorOrdBuscar(comandos,comando,cmpComando))
    {
        puts("No existe el comando ingresado");
        return ERROR_INGRESO_COMANDO;
    }

    //Valido cantidad de argumentos
    if(cant - comando->cantArg != 1)
    {
        printf("Error en la cantidad de argumentos");
        return ERROR_INGRESO_COMANDO;
    }

    return TODO_OK;
}

int procesarComandoDES(const char* argv[],tJornada* jornada,tComando* comando,tCola* buquesEsp, tCola* camionEsp)
{
    tMuelle muelle;
    tZona zona;
    if(validarTiempo(jornada,comando))
    {
        puts("Tiempo insuficiente");
        return ERROR_COMANDO;
    }
    strncpy(muelle.nom,argv[arg1],TAM_NOMBRE);
    strncpy(zona.nom,argv[arg2],TAM_NOMBRE);
    
    if(validarDES(&muelle,&zona,jornada))
    {
        return ERROR_COMANDO;
    }

    //Si hay un evento en el medio antes de que termine la operacion
    //en este momento se actualiza

    operacionDES(&muelle,&zona);
    //actualizo puntos
    zona.capacidad--;
    tListaInsertarOrdConDupli(&jornada->lZonas,&zona,sizeof(tZona),cmpZona,actualizarZona);
    tListaInsertarOrdConDupli(&jornada->lMuelles,&muelle,sizeof(tMuelle),cmpMuelle,actualizarMuelle);


    return TODO_OK;
}

int validarDES(tMuelle* muelle,tZona* zona,tJornada* jornada)
{
    //Valido que existe
    if(tListaBuscarElemOrd(&jornada->lMuelles,muelle,sizeof(tMuelle),cmpMuelle) < 0)
    {
        puts("Error argumento 1");
        return ERROR_COMANDO;
    }
    if(tListaBuscarElemOrd(&jornada->lZonas,zona,sizeof(tZona),cmpZona) < 0)
    {
        puts("Error en argumento 2.");
        return ERROR_COMANDO;
    }
    //Valido que el muelle este ocupado
    if(muelle->estado == false)
    {
        puts("Muelle sin carga pendiente");
        return ERROR_COMANDO;
    }
    //Valido que hay espacio
    if(zona->capacidad == 0)
    {
        puts("Zona sin espacio");
        return ERROR_COMANDO;
    }
    return TODO_OK;
}

int operacionDES(tMuelle* muelle,tZona* zona)
{
    tPila* pilaConteBuque = muelle->buque->contenedores;
    tPila* pilaConteZona = zona->contenedores;
    tContenedor conte;

    desapilar(pilaConteBuque,&conte,sizeof(tContenedor));
    if(pilaVacia(pilaConteBuque))
    {
        muelle->estado = false;
        //sumar 2 puntos
    }
    apilar(pilaConteZona,&conte,sizeof(tContenedor));
    return TODO_OK;
}

int cmpZona(const void* a,const void* b)
{
    const tZona* zona1 = (tZona*)a;
    const tZona* zona2 = (tZona*)b;
    
    return strcmp(zona1->nom,zona2->nom);
}

int cmpMuelle(const void* a,const void* b)
{
    const tMuelle* muelle1 = (tMuelle*)a;
    const tMuelle* muelle2 = (tMuelle*)b;
    return strcmp(muelle1->nom,muelle2->nom);
}

int cmpComando(const void* a,const void* b)
{
    const tComando* comando1 = (tComando*)a;
    const tComando* comando2 = (tComando*)b;

    return strcmp(comando1->nomComando,comando2->nomComando);
}

void actualizarZona(void* actualizado,const void* actualizador)
{
    tZona* zona1 = (tZona*)actualizado;
    const tZona* zona2 = (const tZona*)actualizador;

    zona1->capacidad = zona2->capacidad;
}

void actualizarMuelle(void* actualizado,const void* actualizador)
{
    tMuelle* muelle1 = (tMuelle*)actualizado;
    const tMuelle* muelle2 = (const tMuelle*)actualizador;

    muelle1->estado = muelle2->estado;
}

int validarTiempo(tJornada* jornada,tComando* comando)
{
    int nueTiempo = comando->tiempoUso + jornada->tiempo;
    return jornada->tiempo_maximo <= nueTiempo;
}