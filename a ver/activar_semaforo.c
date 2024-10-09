#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define KEY 0x1111
union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main() {
    int sem_id = semget(KEY, 1, 0666);
    union semun u;
    
    // Inicializa el semáforo en 0 para que los pares estén bloqueados al inicio
    u.val = 0;
    semctl(sem_id, 0, SETVAL, u);

    return 0;
}
