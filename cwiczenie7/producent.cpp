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

void startMessage(SegmentPD* wskaznik, const char* nazwa, int adres) {
    std::cout << "      Producent - Wywolanie programu." << std::endl;
    std::cout << "      Producent - Uzyskano dostep do pamieci dzielonej. ";
    std::cout << "      Nazwa: " << nazwa << ", Adres: " << adres << ", Rozmiar: " << sizeof(wskaznik) << std::endl;
}

char info[100];
void wypiszKomunikat(int ilosc, SegmentPD* wpd) {
    std::cout << "Ilosc wczytanych danych: " << ilosc << std::endl;
    sprintf(info, "Producent - wczytane dane: %.*s\n", NELE, wpd->bufor[wpd->wstaw]);

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
    int wczytDane;
    while( (wczytDane = read(fd, wpd->bufor[wpd->wstaw], NELE)) ) {

        if(wczytDane < NELE) 
            wpd->bufor[wpd->wstaw][wczytDane] = '\0';
        
        wypiszKomunikat(wczytDane, wpd);

        podniesSem(adres_sem_kons); //podnies konsument - zmniejsz wartosc
        std::cout << "Wartosc semafora konsumenta - podniesienie: " << wartoscSem(adres_sem_kons) << std::endl;

        for(int i = 0; i < NELE; i++) //opusc producent - zwieksz wartosc
            opuscSem(adres_sem_prod);
        std::cout << "Wartosc semafora producenta - opuszczenie: " << wartoscSem(adres_sem_prod) << std::endl;

        wpd->wstaw = (wpd->wstaw + 1) % NELE;
    }

}