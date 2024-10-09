#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>

#define KEY 0X1111

// Estructura para el semáforo
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

struct sembuf p = {0, -1, SEM_UNDO};  // Operación "P"
struct sembuf v = {0, +1, SEM_UNDO};  // Operación "V"

int main() {
    pid_t pid;
    int sem_id;

    // Crear o obtener el semáforo
    sem_id = semget(KEY, 1, 0666 | IPC_CREAT);
    if (sem_id == -1) {
        perror("Error al obtener el semáforo");
        exit(EXIT_FAILURE);
    }

    // Inicializar el semáforo con valor 1 (empieza el proceso impar)
    union semun u;
    u.val = 1;
    if (semctl(sem_id, 0, SETVAL, u) == -1) {
        perror("Error al inicializar el semáforo");
        exit(EXIT_FAILURE);
    }

    // Crear archivo de salida
    int fd = open("xd.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Error al abrir el archivo xd.txt");
        exit(EXIT_FAILURE);
    }

    // Crear el proceso hijo (pares)
    pid = fork();
    if (pid < 0) {
        perror("Error en fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Proceso hijo: Ejecuta el programa de pares
        dup2(fd, STDOUT_FILENO);  // Redirigir stdout al archivo xd.txt
        close(fd);

        char *args[] = {"./pares", NULL};
        execv(args[0], args);  // Ejecutar el programa pares

        perror("Error en execv para pares");
        exit(EXIT_FAILURE);
    } else {
        // Proceso padre: Ejecuta el programa de impares
        dup2(fd, STDOUT_FILENO);  // Redirigir stdout al archivo xd.txt
        close(fd);

        char *args[] = {"./impares", NULL};
        execv(args[0], args);  // Ejecutar el programa impares

        perror("Error en execv para impares");
        exit(EXIT_FAILURE);
    }

    // Esperar al hijo
    wait(NULL);

    return 0;  // Aquí estaba el error, faltaba el `0`
}
