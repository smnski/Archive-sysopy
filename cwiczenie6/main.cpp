//Autor: Szymon Urbanski
#include <iostream>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <ctime>
#include <string.h>
#include "obsluga.h"

// Funkcja generujace liczbe pseudolosowa na podstawie obecnego czasu systemowego, ID procesu oraz losowej liczby.
// Dzieki temu oba programy beda generowac rozne od siebie ilosci pobranych danych oraz czasow usypiania w kazdej iteracji.
int losowaLiczba(int a, int b) {

    // Ustawienie ziarna dla generatora.
    std::srand(std::time(nullptr) + getpid() + std::rand());

    // Wygenerowanie losowej liczby z przedziału [a, b].
    int num = std::rand() % (b - a + 1) + a;
  
    return num;
}

// Funkcja inkrementuje otrzymana liczbe, po czym zapisuje ja do pliku.
void zapiszLiczbeInkr(char* liczbaPlik) {

    int liczbaInkrement = atoi(liczbaPlik); liczbaInkrement++;

    int desWrite;
    if((desWrite = open("numer.txt", O_WRONLY, 0644)) == -1) {
        perror("ERROR: Funkcja open - zapiszLiczbeInkr - main.cpp");
        _exit(1);
    }

    char liczbaInkrementChar[50];
    if(sprintf(liczbaInkrementChar, "%d", liczbaInkrement) < 0) {
        perror("ERROR: Funkcja sprintf - zapiszLiczbeInkr - main.cpp");
        _exit(1);
    }

     if(write(desWrite, liczbaInkrementChar, strlen(liczbaInkrementChar)) == -1) {
        perror("ERROR: Funkcja write - zapiszLiczbeInkr - main.cpp");
        _exit(1);
    }
    
    if(close(desWrite) == -1) {
        perror("ERROR: Funkcja close - zapiszLiczbeInkr - main.cpp");
        _exit(1);
    }
}

// Funkcja wczytuje liczbe, wypisuje ja do konsoli, po czym zwraca.
char* wczytajLiczbe() {

    int desRead;
    if((desRead = open("numer.txt", O_RDONLY, 0644)) == -1) {
        perror("ERROR: Funkcja open - wczytajLiczbe - main.cpp");
        _exit(1);
    }

    char* liczbaWczyt = new char[30];
    if((read(desRead, liczbaWczyt, 30)) < 1) {
        perror("ERROR: Funkcja read - wczytajLiczbe - main.cpp");
        _exit(1);
    }

    std::cout << "      Wczytana liczba: " << liczbaWczyt << std::endl;

    if((close(desRead)) == -1) {
        perror("ERROR: Funkcja close - wczytajLiczbe - main.cpp");
        _exit(1);
    }

return liczbaWczyt;
}

// Sekcja krytyczna programu
void sekcjaKrytyczna() {

    char* liczba = wczytajLiczbe();

    sleep(losowaLiczba(1,2));

    zapiszLiczbeInkr(liczba);
}


int main(int argc, char** argv) {

    // Przypianie nazwy semafora oraz ilosci sekcji do zmiennych.
    const char *nazwa_sem = argv[2];
    int sekcje = atoi(argv[1]);

    // Otworzenie wczesniej utworzonego semafora
    sem_t* semafor = otworzSem(nazwa_sem);

    // Przechowanie ID danego procesu potomnego.
    pid_t procesPID = getpid();

    // Procesy potomne wykonaja wlasne sprawy oraz czesc krytyczna dla kazdej sekcji
    for(int i = 0; i < sekcje; i++) {
        // Wlasne sprawy
        std::cout << "Wlasne sprawy - proces: " << procesPID << std::endl;
        std::cout << "Wartosc semafora - poczatek: " << wartoscSem(semafor) << std::endl;
        sleep(losowaLiczba(1,2));

        // Podniesienie semafora na czas wykonywania sekcji krytycznej
        podniesSem(semafor);
        std::cout << "Wartosc semafora - podniesienie: " << wartoscSem(semafor) << std::endl;

        // Sekcja krytyczna
        std::cout << "      Sekcja krytyczna - proces: " << procesPID << std::endl;
        sekcjaKrytyczna();
        std::cout << "      Wartosc semafora: " << wartoscSem(semafor) << std::endl;

        // Opuszczenie semafora po sekcji krytycznej
        opuscSem(semafor);
        std::cout << "Wartosc semafora - opuszczenie: " << wartoscSem(semafor) << std::endl;
    }
}