#include <iostream>
#include <ctime>
#include <string.h>
#include "SemMem.h"

typedef struct {
    char bufor[NBUF][NELE];
    int wstaw, wyjmij;
} SegmentPD;

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

    // Glowna petla
    int wDane;
    while(true) {

        wDane = read(fd, wpd->bufor[wpd->wstaw], NELE);
        std::cout << "wDane prod: " << wDane << std::endl;
        if(wDane < NELE)
            wpd->bufor[wpd->wstaw][wDane] = '\0';
        std::cout << "Towar - prod: " << wpd->bufor[wpd->wstaw] << std::endl;

        opuscSem(adres_sem_prod); //zwieksz o 1

        wpd->wstaw = (wpd->wstaw +1) % NBUF;

        podniesSem(adres_sem_kons); //zmniejsz o 1

        if(wDane < NELE) break;
    }
}