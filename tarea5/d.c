#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define KEY 0X1111
union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};
struct sembuf p={0,-1,SEM_UNDO};
struct sembuf v={0,+1,SEM_UNDO};

int main(){
    int id=semget(KEY,1,0666 |IPC_CREAT);
    union semun u;

    u.val=0;
    semctl(id,0,SETVAL,u);
    
    while(1){
        //system("sleep");
    semop(id,&p,1);
        int x=0;
        while(x<1000){
            printf("Hola, mundo!\n");
            x++;
        }
        usleep(500000);
        system("clear");
        //system("sleep");
    semop(id,&v,1);
    }
    return 0;
}