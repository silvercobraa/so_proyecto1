#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char* const MENSAJE_INICIAL = " ____  _          _ _   _  ___   ___ _  __  ____            _   _   _       \n" \
                                    "/ ___|| |__   ___| | | / |/ _ \\ / _ (_)/ / |  _ \\ ___  __ _| | | \\ | | ___  \n" \
                                    "\\___ \\| '_ \\ / _ \\ | | | | | | | | | |/ /  | |_) / _ \\/ _` | | |  \\| |/ _ \\ \n" \
                                    " ___) | | | |  __/ | | | | |_| | |_| / /_  |  _ <  __/ (_| | | | |\\  | (_) |\n" \
                                    "|____/|_| |_|\\___|_|_| |_|\\___/ \\___/_/(_) |_| \\_\\___|\\__,_|_| |_| \\_|\\___/ \n" \
                                    "                                                                            \n" \
                                    " _____     _        \n" \
                                    "|  ___|_ _| | _____ \n" \
                                    "| |_ / _` | |/ / _ \\\n" \
                                    "|  _| (_| |   <  __/\n" \
                                    "|_|  \\__,_|_|\\_\\___|\n\n" \
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
                                    "Ingrese 'buscar_comando' para buscar un comando en el historial.\n" \
                                    "Ingrese 'borrar_log' para borrar el log.\n" \
                                    "Ingrese 'exit' para salir.\n\n";



int main(int argc, char* argv[])
{
	const char PROMPT[] = "Shell 100% Real No Fake 1 Link >>> "; // El string mostrado por la shell cuando espera un comando
	const int LARGO_MAX_BUFFER = 1000; // Largo del string leído por la shell, incluyendo el caracter nulo.

	int MAX_PIPES = LARGO_MAX_BUFFER - 1;

	char buffer[LARGO_MAX_BUFFER]; // Aquí se guardan los comandos ingresados.

	char* aux_comando = NULL; // este puntero se va modificando según strsep
	char* iterador_comando = NULL; // este puntero recorre los compandos, va apuntando a cada substring del buffer que este separado por un pipe

	char* aux_argumento = NULL;
	char* iterador_argumento = NULL;

	while (1)
	{
		printf("%s", PROMPT);
		aux_comando = &buffer[0];

		if (!strcmp(buffer, "exit\n"))
		{
			exit(EXIT_SUCCESS);
		}

		fgets(buffer, LARGO_MAX_BUFFER, stdin);

        // Esta wea lo único que hace es loopear los strings separados por "|" y luego loopear los substrings separados por " ".
		while ((iterador_comando = strsep(&aux_comando, "|")) != NULL)
		{
			printf("comando: %s\n", iterador_comando);
			aux_argumento = iterador_comando;

			while ((iterador_argumento = strsep(&aux_argumento, " ")) != NULL)
			{
				printf("\targumento: %s\n", iterador_argumento);
			}
		}

		// printf("el buffer es:\n%s", buffer);
	}
	return 0;
}
