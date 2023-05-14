//Autor: Szymon Urbanski
#include <iostream>
#include <ctime>
#include <string.h>
#include "SemMem.h"

typedef struct {
    char bufor[NBUF][NELE];
    int wstaw, wyjmij;
} SegmentPD;

char info[100];
void wypiszKomunikat(int ilosc, SegmentPD* towar) {
    sprintf(info, "         Producent - wczytane dane: %.*s | ilosc: %d\n", NELE, towar->bufor[towar->wyjmij], ilosc);

    if(write(STDOUT_FILENO, info, strlen(info)) == -1) {
        perror("ERROR: Funkcja write w producent.cpp napotkala problem.\n");
        _exit(1);
    }

    sprintf(info, "         Aktualny indeks bufora wstaw: %d\n", towar->wstaw);

    if(write(STDOUT_FILENO, info, strlen(info)) == -1) {
        perror("ERROR: Funkcja write w producent.cpp napotkala problem.\n");
        _exit(1);
    }
}

void wypiszKomunikat2(sem_t* adres1, sem_t* adres2) {
    sprintf(info, "Producent - wartosci semaforow: Producent -  %d Konsument - %d\n", wartoscSem(adres1), wartoscSem(adres2));

    if(write(STDOUT_FILENO, info, strlen(info)) == -1) {
        perror("ERROR: Funkcja write w producent.cpp napotkala problem.\n");
        _exit(1);
    }
}

int main(int argc, char* argv[]) {
    
    const char* nazwa_pliku = argv[1];
    const char* nazwa_sem_prod = argv[2];
    const char* nazwa_sem_kons = argv[3];
    const char* nazwa_SHM = argv[4];

    sem_t* adres_sem_prod = otworzSem(nazwa_sem_prod);
    sem_t* adres_sem_kons = otworzSem(nazwa_sem_kons);

    int adres_SHM = otworzSHM(nazwa_SHM);

    SegmentPD* wpd = (SegmentPD *) mmapSHM(sizeof(SegmentPD), adres_SHM);
    wpd -> wstaw = 0;

    int fd = open(nazwa_pliku, O_RDONLY, 0666);

    wypiszKomunikat2(adres_sem_prod, adres_sem_kons); 

    int wDane;
    while(true) {    

        podniesSem(adres_sem_prod);

        wDane = read(fd, wpd->bufor[wpd->wstaw], NELE);
        if(wDane < NELE)
            wpd->bufor[wpd->wstaw][wDane] = '\0';
        wypiszKomunikat(wDane, wpd);
        wpd->wstaw = (wpd->wstaw +1) % NBUF;

        wypiszKomunikat2(adres_sem_prod, adres_sem_kons);

        opuscSem(adres_sem_kons);

        if(wDane < NELE) break;
        sleep(1);
    }
}