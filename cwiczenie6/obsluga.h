//Autor: Szymon Urbanski
#include <iostream>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>

// Funkcja tworzaca semafor o danej nazwie.
    // Argument O_CREAT pozwala na jego stworzenie, argument 1 sprawia, ze staje sie on dostepny dla dowolnego procesu.
sem_t* stworzSem(const char* nazwa) {

    sem_t* adres = sem_open(nazwa, O_CREAT, 0644, 1);
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

// Funkcja podnoszaca semafor, tzn. dekrementujaca jego wartosc o 1, zamykajac go do uzytku.
void podniesSem(sem_t* adres) {

    if((sem_wait(adres)) == -1) {
        perror("ERROR: Funkcja sem_wait - podniesSem - obsluga.h");
        _exit(1);
    }
}

// Funkcja opuszczajaca semafor, tzn. inkrementujaca jego wartosc o 1, otwierajac go do uzytku.
void opuscSem(sem_t* adres) {

    if((sem_post(adres)) == -1) {
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