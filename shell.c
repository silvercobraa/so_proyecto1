#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <signal.h>
#include <fcntl.h>
#include "funciones.h"

const char* const PROMPT = "Shell 100% Real No Fake 1 Link >>> ";

int main (int argc, char *argv[])
{
	signal(SIGINT, sigintHandler);

	/**
	 * Incluyendo el caracter nulo.
	 */
	const int LARGO_MAX_COMANDO = 1000;

	/**
	 * La cantidad máxima de palabras que puede haber es la cantidad de
	 * caracteres no nulos dividida en 2 (en el caso de que hayan solo palabras
	 * de largo 1 separadas por 1 espacio), redondeada hacia arriba.
	 */
	int MAX_TOKENS = (LARGO_MAX_COMANDO - 1)/2 + LARGO_MAX_COMANDO%2;

	const char* const MENSAJE_INICIAL = " ____  _          _ _   _  ___   ___ _  __  ____            _   _   _       \n" \
										"/ ___|| |__   ___| | | / |/ _ \\ / _ (_)/ / |  _ \\ ___  __ _| | | \\ | | ___  \n" \
										"\\___ \\| '_ \\ / _ \\ | | | | | | | | | |/ /  | |_) / _ \\/ _` | | |  \\| |/ _ \\ \n" \
										" ___) | | | |  __/ | | | | |_| | |_| / /_  |  _ <  __/ (_| | | | |\\  | (_) |\n" \
										"|____/|_| |_|\\___|_|_| |_|\\___/ \\___/_/(_) |_| \\_\\___|\\__,_|_| |_| \\_|\\___/ \n" \
<<<<<<< HEAD
=======
>>>>>>> log
										" _____     _        \n" \
										"|  ___|_ _| | _____ \n" \
										"| |_ / _` | |/ / _ \\\n" \
										"|  _| (_| |   <  __/\n" \
										"|_|  \\__,_|_|\\_\\___|\n\n" \
<<<<<<< HEAD
                    					"Autor: César Bolívar Severino\n\nIngrese un comando o ingrese 'exit' para salir.\n\n";
=======
										"███████▀▀▀░░░░░░░▀▀▀███████\n" \
										"████▀░░░░░░░░░░░░░░░░░▀████\n" \
										"███│░░░░░░░░░░░░░░░░░░░│███\n" \
										"██▌│░░░░░░░░░░░░░░░░░░░│▐██\n" \
										"██░└┐░░░░░░░░░░░░░░░░░┌┘░██\n" \
										"██░░└┐░░░░░░░░░░░░░░░┌┘░░██\n" \
										"██░░┌┘▄▄▄▄▄░░░░░▄▄▄▄▄└┐░░██\n" \
										"██▌░│██████▌░░░▐██████│░▐██\n" \
										"███░│▐███▀▀░░▄░░▀▀███▌│░███\n" \
										"██▀─┘░░░░░░░▐█▌░░░░░░░└─▀██\n" \
										"██▄░░░▄▄▄▓░░▀█▀░░▓▄▄▄░░░▄██\n" \
										"████▄─┘██▌░░░░░░░▐██└─▄████\n" \
										"█████░░▐█─┬┬┬┬┬┬┬─█▌░░█████\n" \
										"████▌░░░▀┬┼┼┼┼┼┼┼┬▀░░░▐████\n" \
										"█████▄░░░└┴┴┴┴┴┴┴┘░░░▄█████\n\n" \
										"Autor: César Bolívar Severino\n\n" \
										"Ingrese un comando.\n" \
										"Ingrese 'historial' para ver una lista de comandos previamente usados.\n" \
										"Ingrese 'borrar_log' para borrar el log.\n" \
										"Ingrese 'exit' para salir.\n\n";

	/**
	 * Aquí se guardan los comandos ingresados.
	 */
>>>>>>> log
	char* string_leida = malloc(LARGO_MAX_COMANDO);

	/**
	 * Este string es usado para guardar una copia de la string leída, para que
	 * al usar strsep no se modifique la original.
	 */
	char* aux = malloc(LARGO_MAX_COMANDO);

	/**
	 * En este archivo se escriben los comandos utilizados sus respectivos outputs
	 * y su tiempo de ejecución.
	 */
	FILE* mishell_log = NULL;
	const char* const NOMBRE_ARCHIVO_LOG = "Log/mishell.log";

	/**
	 * En este archivo se escriben sólamente los comandos utilizados.
	 */
	FILE* archivo_historial = NULL;
	 const char* const NOMBRE_ARCHIVO_HISTORIAL = ".historial.txt";

	/**
	 * En este archivo guardo el output de cada comando.
	 */
	int archivo_output = -1;
	const char* const NOMBRE_ARCHIVO_OUTPUT = ".output.txt";

	/**
	 * Más uno para incluir un puntero nulo al final (strsep lo exige así).
	 */
	char* tokens[MAX_TOKENS + 1];
	char* comando1[MAX_TOKENS + 1];
	char* comando2[MAX_TOKENS + 1];
	char* token = NULL;

	int cantidad_tokens = 0;
	int j = 0;
	int posicion_pipe = -1;
	int pipefd[2];
	time_t t_inicio;
	time_t t_fin;

	if (system("mkdir -p Log") == -1)
	{
		printf("No se pudo crear la carpeta Log\n");
		exit(-1);
	}

	mishell_log = fopen(NOMBRE_ARCHIVO_LOG, "w");
	archivo_historial = fopen(NOMBRE_ARCHIVO_HISTORIAL, "w");
	checkear_fopen(mishell_log, NOMBRE_ARCHIVO_LOG);
	checkear_fopen(archivo_historial, NOMBRE_ARCHIVO_HISTORIAL);
	fflush(mishell_log);
	fflush(archivo_historial);
	fclose(mishell_log);
	fclose(archivo_historial);
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
			free(aux);
			continue;
		}

		/**
		 * Por alguna razón los comandos no funcionan cuando terminan con salto
		 * de línea. Por lo tanto, hay que eliminarlo.
		 */
		aux[strlen(aux) - 1] = '\0';

		if (!strcmp(aux, "exit"))
		{
			free(string_leida);
			free(aux);
			exit(-1);
		}
		if (!strcmp(aux, "historial"))
		{
			imprimir_historial(NOMBRE_ARCHIVO_HISTORIAL);
			free(aux);
			continue;
		}
		if (!strcmp(aux, "borrar_log"))
		{
			borrar_archivo(NOMBRE_ARCHIVO_LOG);
			borrar_archivo(NOMBRE_ARCHIVO_HISTORIAL);
			free(aux);
			continue;
		}

		while ((token = strsep(&aux, " ")) != NULL)
		{
			tokens[cantidad_tokens] = strdup(token);
			cantidad_tokens++;
		}
		tokens[cantidad_tokens] = NULL;
		tokens[MAX_TOKENS] = NULL;

		posicion_pipe = buscar_pipe(tokens);

		/**
		 * Si hay pipe, se forkea 2 veces y se ejecutan 2 comandos.
		 */
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
			printf("COMANDO1: ");
			imprimir_arreglo(comando1);
			printf("COMANDO2: ");
			imprimir_arreglo(comando2);

			archivo_output = open(NOMBRE_ARCHIVO_OUTPUT, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
			if (archivo_output == -1)
			{
				printf("No se pudo crear el archivo 'archivo_output\n'");
				exit(-1);
			}
			/**
			 * Si falla al crear el pipe, salimos del programa.
			 */
			if (pipe(pipefd) == -1)
			{
				perror("Falló el pipe");
				exit(1);
			}

			t_inicio = time(NULL);
			/**
			 * Se crea un proceso hijo, que ejecutará el primer comando y
			 * escribirá en el pipe.
			 */
			if (fork() == 0)
			{
				dup2(pipefd[1], 1);
				close(pipefd[0]);
				close(pipefd[1]);
				execvp(comando1[0], comando1);
				perror("Falló el execvp del comando 1");
				exit(1);
			}

			/**
			 * Se crea otro proceso hijo, que ejecutará el segundo comando y
			 * leerá en el pipe.
			 */
			if (fork() == 0)
			{
				close(STDIN_FILENO);
				dup2(pipefd[0], 0);
				dup2(archivo_output, 1);
				close(pipefd[1]);
				close(pipefd[0]);
				execvp(comando2[0], comando2);
				perror("Falló el execvp del comando 2");
				exit(1);
			}

			close(pipefd[0]);
			close(pipefd[1]);

			/**
			 * Aquí el proceso principal espera que terminen los 2 comandos.
			 */
			wait(NULL);
			wait(NULL);
			t_fin = time(NULL);
		}

		/**
		 * Si esto sucede, es porque el pipe se encontraba al inicio.
		 */
		else if (posicion_pipe == 0)
		{
			printf("ERROR: FALTA UN COMANDO\n");
			cantidad_tokens = 0;
			continue;
		}

		/**
		 * Si no hay pipe, se ejecuta el comando completo.
		 */
		else
		{
			t_inicio = time(NULL);
			/**
			 * Proceso hijo que ejecuta el comando, o termina si falla.
			 */
			if (fork() == 0)
			{
				archivo_output = open(NOMBRE_ARCHIVO_OUTPUT, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
				dup2(archivo_output, 1);
				execvp(tokens[0], tokens);
				perror("Falló el execvp de tokens");
				exit(-1);
			}
			/**
			 * Proceso principal, que espera al proceso hijo anterior.
			 */
			wait(NULL);
			t_fin = time(NULL);
		}
		cantidad_tokens = 0;
		free(aux);
		close(archivo_output);

		mishell_log = fopen(NOMBRE_ARCHIVO_LOG, "a");
		checkear_fopen(mishell_log, NOMBRE_ARCHIVO_LOG);
		archivo_historial = fopen(NOMBRE_ARCHIVO_HISTORIAL, "a");
		checkear_fopen(archivo_historial, NOMBRE_ARCHIVO_HISTORIAL);

		fprintf(archivo_historial, "%s", string_leida);
		fprintf(mishell_log, "%s", string_leida);
		printf("%s", string_leida);

		// tal vez no es necesaria esta linea
		fclose(mishell_log);
		if (system("tee -a Log/mishell.log < .output.txt") == -1)
		{
			printf("Falló tee\n");
		}
		mishell_log = fopen(NOMBRE_ARCHIVO_LOG, "a");
		checkear_fopen(mishell_log, NOMBRE_ARCHIVO_LOG);
		fprintf(mishell_log, "(tiempo de ejecución: %ld s)\n", t_fin - t_inicio);
		printf("(tiempo de ejecución: %ld s)\n", t_fin - t_inicio);
		fclose(mishell_log);
		fclose(archivo_historial);
	}
	return 0;
}
