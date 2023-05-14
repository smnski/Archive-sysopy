//Autor: Szymon Urbanski
#include <iostream>
#include <ctime>
#include <string.h>
#include "SemMem.h"

typedef struct {
    char bufor[NBUF][NELE];
    int wstaw, wyjmij;
} SegmentPD;

void startMessage(SegmentPD* wskaznik, const char* nazwa, int adres) {
    std::cout << "      Producent - Wywolanie programu." << std::endl;
    std::cout << "      Producent - Uzyskano dostep do pamieci dzielonej. ";
    std::cout << "Nazwa: " << nazwa << ", Adres: " << adres << ", Rozmiar: " << sizeof(wskaznik) << std::endl;
}

char info[100];
void wypiszKomunikat(int ilosc, char* towar) {
    sprintf(info, "Producent - wczytane dane: %.*s\n", NELE, towar);

    if(write(STDOUT_FILENO, info, strlen(info)) == -1) {
        perror("ERROR: Funkcja write w producent.cpp napotkala problem.\n");
        _exit(1);
    }
}

int main(int argc, char* argv[]) {
    
    // Przypisanie wartosci podanych w argumentach
    const char* nazwa_pliku = argv[1];
    const char* nazwa_sem_prod = argv[2];
    const char* nazwa_sem_kons = argv[3];
    const char* nazwa_SHM = argv[4];

    sem_t* adres_sem_prod = otworzSem(nazwa_sem_prod);
    sem_t* adres_sem_kons = otworzSem(nazwa_sem_kons);

    int adres_SHM = otworzSHM(nazwa_SHM);

    SegmentPD* wpd = (SegmentPD *) mmapSHM(sizeof(SegmentPD), adres_SHM);
    wpd -> wstaw = 0;

    startMessage(wpd, nazwa_SHM, adres_SHM);

    int fd = open(nazwa_pliku, O_RDONLY, 0666);

    // Glowna petla
    int wczytDane; char towar[NELE+1];
    while(true) {

        // Produkcja towaru
        wczytDane = read(fd, towar, NELE);
        if (wczytDane < NELE)
            towar[wczytDane] = '\0';
        wypiszKomunikat(wczytDane, towar);

        // Opusc semafor producenta //zwieksz o 20
        for(int i = 0; i < NELE; i++) 
            opuscSem(adres_sem_prod);

        // Umiesc towar w buforze
        strcpy(wpd->bufor[wpd->wstaw], towar);

        // Przesun pozycje wstawiania o 1 dalej
        wpd->wstaw = (wpd->wstaw + 1) % NELE;

        // Podniesc semafor Konsumenta //zmniejsz o 1
        podniesSem(adres_sem_kons);
    }
}