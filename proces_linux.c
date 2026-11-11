#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <time.h>

#define SHM_NAME "/mem_comuna"
#define SEM_NAME "/sem_moneda"

int main() {
    srand(time(NULL) ^ getpid());

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(int));
    int *memorie = mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    
    sem_t *semafor = sem_open(SEM_NAME, O_CREAT, 0666, 1);

    while (1) {
        sem_wait(semafor);

        if (*memorie >= 1000) {
            sem_post(semafor); 
            break;
        }

        int ban = (rand() % 2) + 1;

        while (ban == 2 && *memorie < 1000) {
            (*memorie)++;
            printf("[PID %d] A picat 2 -> am scris %d\n", getpid(), *memorie);
            ban = (rand() % 2) + 1;
        }

        sem_post(semafor);

        usleep(1000);
    }

    munmap(memorie, sizeof(int));
    close(shm_fd);
    sem_close(semafor);

    return 0;
}
