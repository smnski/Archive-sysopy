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
        if(wpd->bufor[wpd->wyjmij][i] == '%') {
            *koniec = i;
            return true;
        }
    }
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

    int wDane, koniec;
    while(true) {
        
        std::cout << "Konsument - wartosc " << nazwa_sem_prod << ": " << wartoscSem(adres_sem_prod) << std::endl;
        std::cout << "Konsument - wartosc " << nazwa_sem_kons << ": " << wartoscSem(adres_sem_kons) << std::endl;

        podniesSem(adres_sem_kons);
        
        std::cout << "Towar konsumenta: " << wpd->bufor[wpd->wyjmij] << std::endl;

        if(czyKoniecDanych(wpd, &koniec)) {
            wDane = write(fd, wpd->bufor[wpd->wyjmij], koniec);
            std::cout << "Konsument wczytal " << wDane << " bajtow danych." << std::endl;
            break;
        } 
        
        wDane = write(fd, wpd->bufor[wpd->wyjmij], NELE);
        std::cout << "Konsument wczytal " << wDane << " bajtow danych." << std::endl;
        wpd->wyjmij = (wpd->wyjmij +1) % NBUF;

        opuscSem(adres_sem_prod);

        sleep(1);
    }
}