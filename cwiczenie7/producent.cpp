//Autor: Szymon Urbanski
#include <iostream>
#include <ctime>
#include <string.h>
#include "SemMem.h"

// Deklaracja bufora cyklicznego
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

// Funkcja wypisujaca informacje o danych wczytanych przez proces.
char info[100];
void wypiszKomunikat(int ilosc, SegmentPD* towar) {
    sprintf(info, "         Producent - wczytane dane: %.*s | ilosc: %d\n", NELE, towar->bufor[towar->wyjmij], ilosc);

    if(write(STDOUT_FILENO, info, strlen(info)) == -1) {
        perror("ERROR: write - wypiszKomunikat - producent.cpp\n");
        _exit(1);
    }

    sprintf(info, "         Aktualny indeks bufora wstaw: %d\n", towar->wstaw);

    if(write(STDOUT_FILENO, info, strlen(info)) == -1) {
        perror("ERROR: write - wypiszKomunikat - producent.cpp\n");
        _exit(1);
    }
}

// Funkcja wypisujaca informacje o wartosciach semaforow.
void wypiszKomunikat2(sem_t* adres1, sem_t* adres2) {
    sprintf(info, "Producent - wartosci semaforow: Producent -  %d Konsument - %d\n", wartoscSem(adres1), wartoscSem(adres2));

    if(write(STDOUT_FILENO, info, strlen(info)) == -1) {
        perror("ERROR: write - wypiszKomunikat - producent.cpp\n");
        _exit(1);
    }
}

// Funkcja zamykajaca plik, usuwajaca odwzorowanie obiektu PD i zamykajaca obiekt PD.
void porzadki(int fd, SegmentPD* wpd, size_t len, int sd) {

    if(close(fd) == -1) {
        perror("ERROR: close - porzadki - producent.cpp");
        exit(1);
    }

    delMappSHM(wpd, len);
    zamknijSHM(sd);

    std::cout << "~~~ Producent zakonczyl porzadki ~~~" << std::endl;
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

    // Ustawienie indeksu wstawiania bufora cyklicznego na 0.
    wpd -> wstaw = 0;

    // Otworzenie pliku, w ktorym znajduje sie surowiec do przeslania.
        // Permisje: Czytanie
    int fd = open(nazwa_pliku, O_RDONLY, 0666);

    wypiszKomunikat2(adres_sem_prod, adres_sem_kons); 

    // Glowna sekcja producenta
    int wDane;
    while(true) {    

        // Opusc semafor producenta
        opuscSem(adres_sem_prod);

        // Produkcja towaru - Zapisz wczytane dane z pliku do bufora.
        wDane = read(fd, wpd->bufor[wpd->wstaw], NELE);

        // Jesli w pliku nie ma juz wiecej danych, umiesc w buforze symbol '\0'.
        if(wDane < NELE)
            wpd->bufor[wpd->wstaw][wDane] = '\0';

        wypiszKomunikat(wDane, wpd);

        // Przesun pozycje wstawiania do bufora
        wpd->wstaw = (wpd->wstaw +1) % NBUF;

        wypiszKomunikat2(adres_sem_prod, adres_sem_kons);

        // Podnies semafor konsumenta
        podniesSem(adres_sem_kons);

        // Jesli w pliku nie ma juz wiecej danych, przerwij dzialanie producenta.
        if(wDane < NELE) break;

        // Sleep na losowa ilosc sekund od 1 do 3.
        sleep(losowaLiczba(1, 3));
    }
    // Porzadki na koniec.
    porzadki(fd, wpd, sizeof(SegmentPD), atoi(des_SHM));
}