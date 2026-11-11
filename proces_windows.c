#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SHM_NAME "Local\\mem_comuna"
#define SEM_NAME "Local\\sem_moneda"

int main() {
    srand((unsigned int)time(NULL) ^ GetCurrentProcessId());

    HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(int), SHM_NAME);
    int *memorie = (int *) MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(int));
    
    HANDLE hSemaphore = CreateSemaphore(NULL, 1, 1, SEM_NAME);

    printf("Procesul %lu a pornit.\n", GetCurrentProcessId());

    while (1) {
        WaitForSingleObject(hSemaphore, INFINITE);

        if (*memorie >= 1000) {
            ReleaseSemaphore(hSemaphore, 1, NULL);
            break;
        }

        int ban = (rand() % 2) + 1;

        while (ban == 2 && *memorie < 1000) {
            (*memorie)++;
            printf("[PID %lu] A picat 2 -> am scris %d\n", GetCurrentProcessId(), *memorie);
            ban = (rand() % 2) + 1;
        }

        ReleaseSemaphore(hSemaphore, 1, NULL);
        
        Sleep(1); 
    }

    UnmapViewOfFile(memorie);
    CloseHandle(hMapFile);
    CloseHandle(hSemaphore);

    return 0;
}
