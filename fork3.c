#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define CANTIDAD_COMANDOS 3
/**
for cmd in cmds
    if there is a next cmd
        pipe(new_fds)
    fork
    if child
        if there is a previous cmd
            dup2(old_fds[0], 0)
            close(old_fds[0])
            close(old_fds[1])
        if there is a next cmd
            close(new_fds[0])
            dup2(new_fds[1], 1)
            close(new_fds[1])
        exec cmd || die
    else
        if there is a previous cmd
            close(old_fds[0])
            close(old_fds[1])
        if there is a next cmd
            old_fds = new_fds
if there are multiple cmds
    close(old_fds[0])
    close(old_fds[1])
 */

int main(int argc, char const* argv[])
{
	int pipes[CANTIDAD_COMANDOS - 1][2];
	char* comandos[CANTIDAD_COMANDOS][CANTIDAD_COMANDOS] = {{"ls", "-l", NULL}, {"sort", "-r", NULL}, {"wc", NULL, NULL}};
    pid_t pid;
    for (int i = 0; i < CANTIDAD_COMANDOS; i++)
    {
        for (int j = 0; j < CANTIDAD_COMANDOS; j++)
        {
            if (comandos[i][j] == NULL)
            {
                printf("NULL! ");
            }
            else
            {
                printf("%s ", comandos[i][j]);
            }
        }
        printf("\n");
    }
	for (int i = 0; i < CANTIDAD_COMANDOS; i++)
	{
		if (i != CANTIDAD_COMANDOS - 1)
		{
            fprintf(stderr, "abriendo pipe %d...\n", i);
			pipe(&pipes[i][0]);
		}
        fprintf(stderr, "forkeando...\n");
		pid = fork();

		if (pid == 0)
		{
			if (i != 0)
			{
				dup2(pipes[i - 1][0], 0);
                fprintf(stderr, "[H%d] duplicando %d a pipe[%d][%d]...\n", i, 0, i - 1, 0);
				close(pipes[i - 1][0]);
                fprintf(stderr, "[H%d] cerrando pipe[%d][%d]...\n", i, i - 1, 0);
				close(pipes[i - 1][1]);
                fprintf(stderr, "[H%d] cerrando pipe[%d][%d]...\n", i, i - 1, 1);
            }

			if (i != CANTIDAD_COMANDOS - 1)
			{
				close(pipes[i][0]);
                fprintf(stderr, "[H%d] cerrando pipe[%d][%d]...\n", i, i, 0);
				dup2(pipes[i][1], 1);
                fprintf(stderr, "[H%d] duplicando %d a pipe[%d][%d]...\n", i, 1, i, 1);
				close(pipes[i][1]);
                fprintf(stderr, "[H%d] cerrando pipe[%d][%d]...\n", i, i, 1);
			}
            fprintf(stderr, "[H%d] comando a ejecutar: %s\n", i, comandos[i][0]);
			if (execvp(comandos[i][0], &comandos[i][0]) != -1)
            {
                fprintf(stderr, "Fallo execvp!\n");
                exit(EXIT_FAILURE);
            }
		}
		else
		{
			if (i != 0)
			{
				close(pipes[i - 1][0]);
                fprintf(stderr, "[P] cerrando pipe[%d][%d]...\n", i - 1, 0);
				close(pipes[i - 1][1]);
                fprintf(stderr, "[P] cerrando pipe[%d][%d]...\n", i - 1, 1);
                fprintf(stderr, "[P] esperando hijo %d...\n", i);
                waitpid(pid, NULL, 0);
			}
		}
	}
    // if there are multiple cmds
    // close(old_fds[0]);
    //
    // close(old_fds[1]);
    wait(NULL); // segun yo este wait es redundante, pero el programa queda estancado si no lo pongo (lo cual es al reves de lo que uno esperaría)
    return 0;
}
