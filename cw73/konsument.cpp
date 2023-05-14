#include <iostream>
#include <ctime>
#include <string.h>
#include "SemMem.h"

typedef struct {
    char bufor[NBUF][NELE];
    int wstaw, wyjmij;
} SegmentPD;

bool czyKoniecDanych(SegmentPD* wpd) {
    for(int i = 0; i < NELE; i++) {
        std::cout<<wpd->bufor[wpd->wyjmij][i]<<" "; //debug
        if(wpd->bufor[wpd->wyjmij][i] == '%') {
            return true;
        }
    }
std::cout<<std::endl; //debug
return false;
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
        
        podniesSem(adres_sem_prod);
        std::cout<<"pong\n";
        opuscSem(adres_sem_kons);

        sleep(1);

    }
}