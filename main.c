#include <stdio.h>
#include <stdlib.h>

#include "jornada.h"

int main()
{
    int opc;
    char nombreOperador[MAX_NOMBRE_OP];

    do
    {
        puts("\nPuerto de contenedores - Operacion Contrarreloj");
        puts("\nSeleccione:");
        puts("1- Iniciar una nueva jornada");
        puts("2- Ver ranking de operadores");
        puts("3- Salir");

        scanf("%d", &opc);

        switch(opc)
        {
            case 1:
                printf("Ingrese su nombre: ");
                fflush(stdin);
                gets(nombreOperador);
                nuevaJornada(nombreOperador);
                break;
            case 2:
                //verRanking(); //--prox en operadores.h (persistencia de datos)
                break;
            case 3:
                break;
            default: puts("Opcion invalida, reingrese..");
                    system("pause");
                    break;
        }

        system("cls");
    }while(opc!=3);

    return 0;
}
