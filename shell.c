#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <signal.h>

const char* const PROMPT = "Shell 100% Real No Fake 1 Link >>> ";

void sigintHandler(int sig_num)
{
	signal(SIGINT, sigintHandler);
	printf("\n Esta shell es demasiado chora, no se puede matar con Ctrl-C\n");
	printf("%s", PROMPT);
	fflush(stdout);
}

/**
 * Termina el programa si el puntero es nulo.
 */
void checkear_malloc(char* puntero)
{
	if (puntero == NULL)
	{
		printf("ERROR AL RESERVAR MEMORIA\n");
		exit(-1);
	}
}

/**
 * Retorna true si en el arreglo de strings existe un '|'. Retorna false en otro
 * caso. No se toman precauciones si el arreglo no termina con un puntero nulo.
 */
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

int main (int argc, char *argv[])
{
	signal(SIGINT, sigintHandler);

	/**
	 * Incluyendo el caracter nulo.
	 */
	int LARGO_MAX_COMANDO = 1000;

	/**
	 * La cantidad máxima de palabras que puede haber es la cantidad de
	 * caracteres no nulos dividida en 2 (en el caso de que hayan solo palabras
	 * de largo 1 separadas por 1 espacio), redondeada hacia arriba.
	 */
	int MAX_TOKENS = (LARGO_MAX_COMANDO - 1)/2 + LARGO_MAX_COMANDO%2;

	const char* const MENSAJE_INICIAL = "SHELL 100% REAL NO FAKE 1 LINK\n autor: César Bolívar Severino\n\n Ingrese un comando o ingrese 'exit' para salir.\n\n";
	char* string_leida = malloc(LARGO_MAX_COMANDO);

	/**
	 * Este string es usado para guardar una copia de la string leída, para que
	 * al usar strsep no se modifique la original.
	 */
	char* aux = malloc(LARGO_MAX_COMANDO);

	/**
	 * Más uno para incluir un puntero nulo al final (strsep lo exige así).
	 */
	char* tokens[MAX_TOKENS + 1];
	char* comando1[MAX_TOKENS + 1];
	char* comando2[MAX_TOKENS + 1];

	char* token = NULL;

	int i = 0;
	int j = 0;
	int posicion_pipe = -1;
	time_t t_inicio;
	time_t t_fin;

	pid_t pid;

	checkear_malloc(string_leida);
	checkear_malloc(aux);

	printf("%s", MENSAJE_INICIAL);
	while (1)
	{
		printf("%s", PROMPT);
		fgets(string_leida, LARGO_MAX_COMANDO, stdin);
		aux = strdup(string_leida);

		if (!strcmp(aux, "\n"))
		{
			continue;
		}

		/**
		 * Por alguna razón los comandos no funcionan cuando terminan con salto
		 * de línea. Por lo tanto, hay que eliminarlo.
		 */
		aux[strlen(string_leida) - 1] = '\0';

		if (!strcmp(aux, "exit"))
		{
			exit(-1);
		}
		while ((token = strsep(&aux, " ")) != NULL)
		{
			tokens[i] = strdup(token);
			i++;
		}
		tokens[i] = NULL;
		tokens[MAX_TOKENS] = NULL;
		//printf("Arreglo: [ ");
		//for (j = 0; tokens[j] != NULL; j++)
		//{
		//	printf("%s, ", tokens[j]);
		//}
		//printf("]\n");
		posicion_pipe = buscar_pipe(tokens);
		if (posicion_pipe > 0)
		{
			printf("Se encontró un pipe!\n");
			for (j = 0; j < posicion_pipe; j += 1)
			{
				comando1[j] = tokens[j];
			}
			comando1[posicion_pipe] = NULL;

			for (j = posicion_pipe + 1; ; j += 1)
			{
				if (tokens[j] == NULL)
				{
					printf("parece que esto no se ejecuta\n");
					comando2[j - (posicion_pipe + 1)] = NULL;
					break;
				}
				comando2[j - (posicion_pipe + 1)] = tokens[j];
			}
			printf("COMANDO1:");
			for (j = 0; comando1[j] != NULL; j += 1)
			{
				printf("%s, ", comando1[j]);
			}
			printf("\n");
			printf("COMANDO2:");
			for (j = 0; comando2[j] != NULL; j += 1)
			{
				printf("%s, ", comando2[j]);
			}
			printf("\n");
			//exit(-1);
		}


		//t_inicio = time(NULL);
		//pid = fork();
		//if (pid == 0)
		//{
		//	if (execvp(tokens[0], tokens) == -1)
		//	{
		//		printf("ERROR: comando no reconocido (Si se imprime esto es porque execvp retornó -1)\n");
//
		//		/**
		//		 * Si falla hay que salir del programa porque si no el proceso
		//		 * hijo queda ahí incluso cuando el programa originial termina.
		//		 */
		//		exit(-1);
		//	}
		//}
		//else if (pid < 0)
		//{
		//	printf("ERROR: No se pudo hacer el fork\n");
		//	exit(-1);
		//}
		//else
		//{
		//	wait();
		//	t_fin = time(NULL);
		//	printf("(tiempo de ejecución: %ld s)\n", t_fin - t_inicio);
		//}
		i = 0;
	}
	return 0;
}
