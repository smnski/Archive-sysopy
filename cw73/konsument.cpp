#include <iostream>
#include <ctime>
#include <string.h>
#include "SemMem.h"

typedef struct {
    char bufor[NBUF][NELE];
    int wstaw, wyjmij;
} SegmentPD;

bool czyKoniecDanych(SegmentPD* wpd, int *koniec) {
    for(int i = 0; i < NELE; i++) {
        if(wpd->bufor[wpd->wyjmij][i] == '\0') {
            *koniec = i;
            return true;
        }
    }
return false;
}

char info[100];
void wypiszKomunikat(int ilosc, SegmentPD* towar) {
    sprintf(info, "         Konsument - wczytane dane: %.*s | ilosc: %d\n", NELE, towar->bufor[towar->wyjmij], ilosc);

    if(write(STDOUT_FILENO, info, strlen(info)) == -1) {
        perror("ERROR: Funkcja write w producent.cpp napotkala problem.\n");
        _exit(1);
    }

    sprintf(info, "         Aktualny indeks bufora wyjmij: %d\n", towar->wyjmij);

    if(write(STDOUT_FILENO, info, strlen(info)) == -1) {
        perror("ERROR: Funkcja write w producent.cpp napotkala problem.\n");
        _exit(1);
    }
}

void wypiszKomunikat2(sem_t* adres1, sem_t* adres2) {
    sprintf(info, "Konsument - wartosci semaforow: Producent -  %d Konsument - %d\n", wartoscSem(adres1), wartoscSem(adres2));

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
    wpd -> wyjmij = 0;

    int fd = open(nazwa_pliku, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    int wDane, koniec;
    while(true) {
        
        wypiszKomunikat2(adres_sem_prod, adres_sem_kons);

        podniesSem(adres_sem_kons);

        if(czyKoniecDanych(wpd, &koniec)) {
            wDane = write(fd, wpd->bufor[wpd->wyjmij], koniec);
            wypiszKomunikat(wDane, wpd);
            break;
        } 
        
        wDane = write(fd, wpd->bufor[wpd->wyjmij], NELE);
        wypiszKomunikat(wDane, wpd);

        opuscSem(adres_sem_prod);

        wpd->wyjmij = (wpd->wyjmij +1) % NBUF;

        wypiszKomunikat2(adres_sem_prod, adres_sem_kons); 

        sleep(1);
    }
}