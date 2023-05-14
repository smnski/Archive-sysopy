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
    std::cout << "      Konsument - Wywolanie programu." << std::endl;
    std::cout << "      Konsument - Uzyskano dostep do pamieci dzielonej. ";
    std::cout << "Nazwa: " << nazwa << ", Adres: " << adres << ", Rozmiar: " << sizeof(wskaznik) << std::endl;
}

char info[100];
void wypiszKomunikat(int ilosc, char* towar) {
    sprintf(info, "Konsument - wczytane dane: %.*s\n", NELE, towar);

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
    wpd -> wyjmij = 0;

    startMessage(wpd, nazwa_SHM, adres_SHM);

    int fd = open(nazwa_pliku, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    // Glowna petla
    int wczytDane; char towar[NELE+1];
    while(true) {

        // Opusc semafor Konsumenta //zwieksz o 1
        opuscSem(adres_sem_kons);

        // Umiesc towar w buforze
        strcpy(towar, wpd->bufor[wpd->wyjmij]);
        wczytDane = strlen(towar);
        wypiszKomunikat(wczytDane, towar);

        // Przesun pozycje wstawiania o 1 dalej
        wpd->wyjmij = (wpd->wyjmij + 1) % NBUF;

        // Podniesc semafor Producenta //zmniejsz o 20
        for(int i = 0; i < NELE; i++) 
            podniesSem(adres_sem_prod);

        // Konsumpcja towaru
        if(write(fd, towar, wczytDane) == -1) {
        perror("ERROR: Funkcja write w konsument.cpp napotkala problem.\n");
        _exit(1);
    }
        
    if(wczytDane < NELE) break;
    }
}