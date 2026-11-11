#include "constantes.h"
#include "jornada.h"

int main()
{
    int opc,c;
    char nombreOperador[MAX_NOMBRE_OP];
    tJornada jornada;
    tEstadisticasOperadores datosOperador;
    tParametros param;

    tArbol arbolIdx;

    crearArbol(&arbolIdx);

    if(inicializarIndiceOperadores(&arbolIdx)!=TODO_OK)
        return ERROR_ARCH;

    srand(time(NULL));

    do
    {
        puts(MENU_INICIO);
        scanf("%d", &opc);

        while((c=getchar()) != '\n' && c != EOF);
        switch(opc)
        {
            case 1:
                    printf("Ingrese su nombre: ");
                    do
                    {
                        if(fgets(nombreOperador, MAX_NOMBRE_OP+1, stdin) == NULL)
                            return 1;

                    }while(nombreOperador[0] == '\n');
                    nombreOperador[strcspn(nombreOperador, "\n")] = '\0';

                    if(gestionarOperador(nombreOperador, &arbolIdx, &datosOperador)!=TODO_OK)
                    {
                        puts(MSG_ERROR_OPERADOR);
                        system("pause");
                        break;
                    }

                    if(nuevaJornada(&datosOperador, &jornada, &param)==NO_SIMULABLE)
                    {
                        puts(MSG_NO_SIMULABLE);
                        break;
                    }
                    desarrolloJornada(&jornada, &param, &datosOperador);

                    actualizarOperador(&datosOperador, &arbolIdx);

                break;
            case 2:
                verRanking(&arbolIdx);
                break;
            case 3:
                break;
            default:
                puts("Opcion invalida, reingrese..");
                system("pause");
                break;
        }


         system("pause");
        system("cls");

    }while(opc!=3);

    vaciarArbol(&arbolIdx);
    return 0;
}
