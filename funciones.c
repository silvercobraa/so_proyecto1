#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#include "funciones.h"

extern const char* const PROMPT;

void sigintHandler(int sig_num)
{
	signal(SIGINT, sigintHandler);
	printf("\n Esta shell es demasiado chora, no se puede matar con Ctrl-C\n");
	printf("%s", PROMPT);
	fflush(stdout);
}

void checkear_malloc(char* puntero)
{
	if (puntero == NULL)
	{
		printf("ERROR AL RESERVAR MEMORIA\n");
		exit(-1);
	}
}

void checkear_fopen(FILE* archivo, const char* const nombre)
{
	if (archivo == NULL)
	{
		printf("No se pudo abrir el archivo %s\n", nombre);
		exit(-1);
	}
}

void imprimir_historial(const char* const nombre_archivo_historial)
{
	if (fork() == 0)
	{
		char* comando_cat[] = {"cat", nombre_archivo_historial, NULL};
		execvp(comando_cat[0], comando_cat);
		perror("No se pudo mostrar el historial\n");
		exit(-1);
	}
	else
	{
		wait(NULL);
		printf("\nDone!\n");
		return;
	}
}

int buscar_pipe(char** arreglo)
{
	int i;
	for (i = 0; arreglo[i] != NULL; i++)
	{
		if (!strcmp(arreglo[i], "|"))
		{
			return i;
		}
	}
	return -1;
}

void imprimir_arreglo(char** arreglo)
{
	int i;
	for (i = 0; arreglo[i] != NULL; i++)
	{
		printf("%s, ", arreglo[i]);
	}
	printf("\n");
}

void borrar_archivo(const char* const ruta_archivo)
{
	if (fork() == 0)
	{
		char* const comando_rm[] = {"rm", ruta_archivo, NULL};
		execvp(comando_rm[0], comando_rm);
		perror("No se pudo ejecutar el comando 'rm'\n");
		exit(-1);
	}
	else
	{
		wait(NULL);
		printf("\nArchivo %s borrado!\n", ruta_archivo);
		return;
	}
}
