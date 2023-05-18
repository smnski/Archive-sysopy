//Autor: Szymon Urbanski
#include <iostream>
#include <ctime>
#include <string.h>
#include "SemMem.h"

typedef struct {
    char bufor[NBUF][NELE];
    int wstaw, wyjmij;
} SegmentPD;

// Funkcja generujace liczbe pseudolosowa na podstawie obecnego czasu systemowego, ID procesu oraz losowej liczby.
// Dzieki temu oba programy beda generowac rozne od siebie ilosci pobranych danych oraz czasow usypiania w kazdej iteracji.
int losowaLiczba(int a, int b) {

    // Ustawienie ziarna dla generatora.
    std::srand(std::time(nullptr) + getpid() + std::rand());

    // Wygenerowanie losowej liczby z przedziału [a, b].
    int num = std::rand() % (b - a + 1) + a;
  
    return num;
}

// Funkcja sprawdzajaca, czy w buforze cyklicznym znajduje sie symbol '\0'.
    // Jesli tak, przypisuje do zmiennej 'koniec' ilosc wczytanych danych.
bool czyKoniecDanych(SegmentPD* wpd, int *koniec) {

    for(int i = 0; i < NELE; i++) {
        if(wpd->bufor[wpd->wyjmij][i] == '\0') {
            *koniec = i;
            return true;
        }
    }

return false;
}

// Funkcja wypisujaca informacje o danych wczytanych przez proces.
char info[100];
void wypiszKomunikat(int ilosc, SegmentPD* towar) {
    sprintf(info, "         Konsument - wczytane dane: %.*s | ilosc: %d\n", NELE, towar->bufor[towar->wyjmij], ilosc);

    if(write(STDOUT_FILENO, info, strlen(info)) == -1) {
        perror("ERROR: write - wypiszKomunikat - konsument.cpp\n");
        _exit(1);
    }

    sprintf(info, "         Aktualny indeks bufora wyjmij: %d\n", towar->wyjmij);

    if(write(STDOUT_FILENO, info, strlen(info)) == -1) {
        perror("ERROR: write - wypiszKomunikat - konsument.cpp\n");
        _exit(1);
    }
}

// Funkcja wypisujaca informacje o wartosciach semaforow.
void wypiszKomunikat2(sem_t* adres1, sem_t* adres2) {
    sprintf(info, "Konsument - wartosci semaforow: Producent -  %d Konsument - %d\n", wartoscSem(adres1), wartoscSem(adres2));

    if(write(STDOUT_FILENO, info, strlen(info)) == -1) {
        perror("ERROR: write - wypiszKomunikat - konsument.cpp\n");
        _exit(1);
    }
}

// Funkcja zamykajaca plik, usuwajaca odwzorowanie obiektu PD i zamykajaca obiekt PD.
void porzadki(int fd, SegmentPD* wpd, size_t len, int sd) {

    if(close(fd) == -1) {
        perror("ERROR: close - porzadki - konsument.cpp");
        exit(1);
    }

    delMappSHM(wpd, len);
    zamknijSHM(sd);

    std::cout << "~~~ Konsument zakonczyl porzadki ~~~" << std::endl;
}

int main(int argc, char* argv[]) {

    // Przypisanie argumentow uruchomienia programu do zmiennych.
    const char* nazwa_pliku = argv[1];
    const char* nazwa_sem_prod = argv[2];
    const char* nazwa_sem_kons = argv[3];
    const char* nazwa_SHM = argv[4];
    const char* des_SHM = argv[5];

    // Otworzenie semaforow producenta i konsumenta
    sem_t* adres_sem_prod = otworzSem(nazwa_sem_prod);
    sem_t* adres_sem_kons = otworzSem(nazwa_sem_kons);

    // Otworzenie obiektu pamieci dzielonej.
    int adres_SHM = otworzSHM(nazwa_SHM);

    // Odwzorowanie obiektu pamieci dzielonej w wirtualnej przestrzeni adresowej procesu.
    SegmentPD* wpd = (SegmentPD *) mmapSHM(sizeof(SegmentPD), adres_SHM);

    // Ustawienie indeksu wyjmowania bufora cyklicznego na 0.
    wpd -> wyjmij = 0;

    // Otworzenie pliku, do ktorego bedzie zapisywany przeslany surowiec.
        // Permiscje: Zapisywanie, tworzenie pliku, wyczyszczenie zawartosci pliku.
    int fd = open(nazwa_pliku, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    // Glowna sekcja konsumenta
    int wDane, koniec;
    while(true) {
        
        wypiszKomunikat2(adres_sem_prod, adres_sem_kons);

        // Opusc semafor konsumenta
        opuscSem(adres_sem_kons);

        // Jesli w buforze znajduje sie symbol '\0', zapisz ostatnie dane do pliku i przerwij dzialanie konsumenta.
        if(czyKoniecDanych(wpd, &koniec)) {
            wDane = write(fd, wpd->bufor[wpd->wyjmij], koniec);
            wypiszKomunikat(wDane, wpd);
            break;
        } 
        
        // Konsumpcja towaru - Zapisz wczytane z bufora dane do pluku.
        wDane = write(fd, wpd->bufor[wpd->wyjmij], NELE);
        wypiszKomunikat(wDane, wpd);

        // Przesun pozycje wyjmowania z bufora
        wpd->wyjmij = (wpd->wyjmij +1) % NBUF;

        // Podnies semafor producenta
        podniesSem(adres_sem_prod);

        // Sleep na losowa ilosc sekund od 1 do 3.
        sleep(losowaLiczba(1, 3));
    }
    porzadki(fd, wpd, sizeof(SegmentPD), atoi(des_SHM));
}