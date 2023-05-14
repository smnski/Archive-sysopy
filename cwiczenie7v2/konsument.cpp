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
    sprintf(info, "Konsument - wczytane dane: %.*s\n", NELE, towar->bufor[towar->wyjmij]);

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

    // Glowna petla
    int wDane;
    while(true) {

        opuscSem(adres_sem_kons); //zwieksz o 1

        wDane = write(fd, wpd->bufor[wpd->wyjmij], NELE);
        std::cout << "wDane kons: " << wDane << std::endl;

        std::cout<<wpd->bufor[wpd->wyjmij]<<std::endl;

        wypiszKomunikat(wDane, wpd);

        wpd->wyjmij = (wpd->wyjmij +1) % NBUF;

        podniesSem(adres_sem_prod); //zmniejsz o 1

        sleep(1);
    }
}