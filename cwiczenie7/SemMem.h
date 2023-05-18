//Autor: Szymon Urbanski
#include <iostream>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>

//___________________________________________________//~Definicje~//___________________________________________________//

#define NBUF 3
#define NELE 20

//___________________________________________________//~Semafory~//___________________________________________________//

// Funkcja tworzaca semafor o danej nazwie.
    // Argument O_CREAT pozwala na jego stworzenie.
sem_t* stworzSem(const char* nazwa, int size) {

    sem_t* adres = sem_open(nazwa, O_CREAT, 0644, size);
    if(adres == SEM_FAILED) {
        perror("ERROR: Funkcja sem_open - stworzSem - obsluga.h");
        _exit(1);
    }

    return adres;
}

// Funkcja usuwajaca semafor o danym adresie.
void usunSem(const char* adres) {

    if((sem_unlink(adres)) == -1) {
        perror("ERROR: Funkcja sem_unlink - usunSem - obsluga.h");
        _exit(1);
    }
}

// Funkcja otwierajaca semafor o danej nazwie.
    // Argument 0 w sem_open sprawia, ze jest on otwierany do czytania i zapisywania.
sem_t* otworzSem(const char* nazwa) {
    sem_t* adres = sem_open(nazwa, 0);

    if(adres == SEM_FAILED) {
        perror("ERROR: Funkcja sem_open - otworzSem - obsluga.h");
        _exit(1);
    }

    return adres;
}

// Funkcja zamykajaca semafor o danym adresie.
    // Zwalnia to przypisane do niego zasoby.
void zamknijSem(sem_t* adres) {

    if((sem_close(adres)) == -1) {
        perror("ERROR: Funkcja sem_close - zamknijSem - obsluga.h");
        _exit(1);
    }
}

// Funkcja podnoszaca semafor, tzn. inkrementujaca jego wartosc o 1
void podniesSem(sem_t* adres) {

    if((sem_post(adres)) == -1) {
        perror("ERROR: Funkcja sem_wait - podniesSem - obsluga.h");
        _exit(1);
    }
}

// Funkcja opuszczajaca semafor, tzn. dekrementujaca jego wartosc o 1
void opuscSem(sem_t* adres) {

    if((sem_wait(adres)) == -1) {
        perror("ERROR: Funkcja sem_post - opuscSem - obsluga.h");
        _exit(1);
    }
}

// Funkcja zwracajaca wartosc semafora o danym adresie.
int wartoscSem(sem_t* adres) {
    int wartosc;

    if((sem_getvalue(adres, &wartosc)) == -1) {
        perror("ERROR: Funkcja sem_getvalue - wartoscSem - obsluga.h");
        _exit(1);
    }

    return wartosc;
}

//___________________________________________________//~Pamiec Dzielona~//___________________________________________________//

// Funkcja tworzaca obiekt pamieci dzielonej o danej nazwie.
int stworzSHM(const char* nazwa) {

    int des_SHM = shm_open(nazwa, O_RDWR | O_CREAT | O_EXCL, 0644);
    if(des_SHM == -1) {
        perror("ERROR: shm_open - stworzSHM - SemMem.h\n");
    }

return des_SHM;
}

// Funkcja usuwajaca obiekt pamieci dzielonej o danej nazwie.
void usunSHM(const char* nazwa) {

    if(shm_unlink(nazwa) == -1) {
        perror("ERROR: shm_unlink - usunSHM - SemMem.h\n");
    }
}

// Funkcja otwierajaca obiekt pamieci dzielonej.
    // Permisje: Czytanie i zapisywanie
int otworzSHM(const char* nazwa) {
    int des_SHM = shm_open(nazwa, O_RDWR, 0644);
    if(des_SHM == -1) {
        perror("ERROR: shm_open - otworzSHM - SemMem.h\n");
    }

return des_SHM;
}

// Funkcja zamykajaca obiekt pamieci dzielonej.
void zamknijSHM(int des) {

    if(close(des) == -1) {
        perror("ERROR: close - zamknijSHM - SemMem.h\n");
    }
}

// Funkcja ustawiajaca rozmiar danego obiektu pamieci dzielonej.
int truncSHM(int des, int len) {

    if(ftruncate(des, len) == -1) {
        perror("ERROR: ftruncate - truncSHM - SemMem.h\n");
    }

return len;
}

// Funkcja odwzorowujaca obiekt pamieci dzielonej do wirtualnej przestrzeni adresowej procesu.
void* mmapSHM(size_t len, int des) {

    void* adres = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, des, 0);
    if(adres == MAP_FAILED) {
        perror("ERROR: mmap - mmapSHM - SemMem.h\n");
    }

return adres;
}

// Funkcja usuwajaca odwzorowanie obiektu pamieci dzielonej z przestrzeni adresowej procesu.
void delMappSHM(void* adres, size_t len) {

    if(munmap(adres, len) == -1) {
        perror("ERROR: munmap - delMappSHM - SemMem.h\n");
    }
}