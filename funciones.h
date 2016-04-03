/**
 * Handler de SIGINT.
 */
void sigintHandler(int sig_num);

/**
 * Termina el programa si el puntero 'puntero' es nulo.
 */
void checkear_malloc(char* puntero);

/**
 * Termina el programa si el puntero 'archivo' es nulo.
 */
void checkear_fopen(FILE* archivo, const char* const nombre);

/**
 * Hace un 'cat' del archivo 'nombre_archivo_historial'.
 */
void imprimir_historial(const char* const nombre_archivo_historial);

/**
 * Retorna true si en el arreglo de strings existe un '|'. Retorna false en otro
 * caso. No se toman precauciones si el arreglo no termina con un puntero nulo.
 */
int buscar_pipe(char** arreglo);

/**
 * Imprime los strings del arreglo hasta que encuentra nulo.
 */
void imprimir_arreglo(char** arreglo);

/**
 * Llama al comando rm con argumento 'archivo'.
 */
void borrar_archivo(const char* const ruta_archivo);
