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

/**
 * Imprime los strings del arreglo hasta que encuentra nulo.
 */
void imprimir_arreglo(char** arreglo)
{
	int i;
	for (i = 0; arreglo[i] != NULL; i++)
	{
		printf("%s, ", arreglo[i]);
	}
	printf("\n");
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
	int des_p[2];
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


		t_inicio = time(NULL);

		pid = fork();

		if (pid == 0)
		{
			posicion_pipe = buscar_pipe(tokens);
			if (posicion_pipe > 0)
			{
				printf("SE ENCONTRÓ UN PIPE!\n");
				for (j = 0; j < posicion_pipe; j += 1)
				{
					comando1[j] = tokens[j];
				}
				comando1[posicion_pipe] = NULL;

				for (j = posicion_pipe + 1; ; j += 1)
				{
					if (tokens[j] == NULL)
					{
						comando2[j - (posicion_pipe + 1)] = NULL;
						break;
					}
					comando2[j - (posicion_pipe + 1)] = tokens[j];
				}
				//printf("COMANDO1: ");
				//imprimir_arreglo(comando1);
				//printf("COMANDO2: ");
				//imprimir_arreglo(comando2);

				// aqui habria que volver a forkear para poder usar los pipes. De
				// momento solo se ejecutara el primer comando.
				//printf("(pipes aún no implementados, se ejecutará solo el primer comando)\n");
				//if (execvp(comando1[0], comando1) == -1)
				//{
				//	exit(-1);
				//}
				if(pipe(des_p) == -1)
				{
					perror("Falló el pipe");
					exit(1);
				}

				/**
				 * Se crea un hijo del proceso hijo (nieto?), que ejecutará el primer
				 * comando y escribirá en el pipe.
				 */
				if(fork() == 0)            //first fork
				{
					close(STDOUT_FILENO);  //closing stdout
					dup(des_p[1]);         //replacing stdout with pipe write
					close(des_p[0]);       //closing pipe read
					close(des_p[1]);

					execvp(comando1[0], comando1);
					perror("Falló el execvp del comando 1");
					exit(1);
				}

				/**
				 * Se crea otro hijo del proceso hijo, que ejecutará el segundo
				 * comando y leerá en el pipe.
				 */
				if(fork() == 0)            //creating 2nd child
				{
					close(STDIN_FILENO);   //closing stdin
					dup(des_p[0]);         //replacing stdin with pipe read
					close(des_p[1]);       //closing pipe write
					close(des_p[0]);

					execvp(comando2[0], comando2);
					perror("Falló el execvp del comando 2");
					exit(1);
				}

				close(des_p[0]);
				close(des_p[1]);

				/**
				 * Aquí hay dos wait para que se espere que terminen los 2 comandos.
				 */
				wait(0);
				wait(0);
				exit(0);
				printf("Exito!!!\n");

			}
			/**
			 * Si esto sucede, es porque el pipe se encontraba al inicio.
			 */
			else if (posicion_pipe == 0)
			{
				printf("ERROR: FALTA UN COMANDO\n");
				exit(-1);
			}
			else
			{
				printf("(Si esto se ejecuta es porque el string no contenía pipes.)\n");
				if (execvp(tokens[0], tokens) == -1)
				{
					printf("ERROR: comando no reconocido (Si se imprime esto es porque execvp retornó -1)\n");

					/**
					 * Si falla hay que salir del programa porque si no el proceso
					 * hijo queda ahí incluso cuando el programa originial termina.
					 */
					exit(-1);
				}
			}
		}
		else if (pid < 0)
		{
			printf("ERROR: No se pudo hacer el fork\n");
			exit(-1);
		}
		else
		{
			wait();
			t_fin = time(NULL);
			printf("(tiempo de ejecución: %ld s)\n", t_fin - t_inicio);
		}
		i = 0;
	}
	return 0;
}

//
