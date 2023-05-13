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

int main(int argc, char* argv[]) {

    std::cout << "konsument.x called." << std::endl;

}