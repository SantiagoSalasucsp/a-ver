#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

#define KEY 0X1111

struct sembuf p = {0, -1, SEM_UNDO};  // Operación "P" (wait)
struct sembuf v = {0, +1, SEM_UNDO};  // Operación "V" (signal)

int main() {
    int sem_id = semget(KEY, 1, 0666);
    if (sem_id == -1) {
        perror("Error al obtener el semáforo");
        exit(EXIT_FAILURE);
    }

    for (int i = 2; i <= 10; i += 2) {
        // Esperar turno del proceso par
        semop(sem_id, &p, 1);

        printf("Par: %d\n", i);
        fflush(stdout);  // Asegurar que se escriba de inmediato

        // Liberar turno para el proceso impar
        semop(sem_id, &v, 1);

        sleep(1);  // Simulación de trabajo
    }

    return 0;
}
