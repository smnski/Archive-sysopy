//Autor: Szymon Urbanski
#include <iostream>
#include <ctime>
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
    std::cout << "Producent - Wywolanie programu." << std::endl;
    std::cout << "Producent - Przesylanie towaru po: " << NELE << " bajtow." << std::endl;
    std::cout << "Producent - Uzyskano dostep do pamieci dzielonej. ";
    std::cout << "Nazwa: " << nazwa << ", Adres: " << adres << ", Rozmiar: " << sizeof(wskaznik) << std::endl;
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

}