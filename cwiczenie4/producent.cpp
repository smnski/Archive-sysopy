// Autor: Szymon Urbanski
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <ctime>

// Funkcja generujace liczbe pseudolosowa na podstawie obecnego czasu systemowego, ID procesu oraz losowej liczby.
// Dzieki temu oba programy beda generowac rozne od siebie ilosci pobranych danych oraz czasow usypiania w kazdej iteracji.
int losowaLiczba(int a, int b) {

    // Ustawienie ziarna dla generatora.
    std::srand(std::time(nullptr) + getpid() + std::rand());

    // Wygenerowanie losowej liczby z przedziału [a, b].
    int num = std::rand() % (b - a + 1) + a;
  
    return num;
}

int main(int argc, char *argv[]) {

    // Przypisanie deskryptorow do czytania oraz zapisu danych do zmiennych.
    int ptkRead = atoi(argv[2]), ptkWrite = atoi(argv[3]);

    // Zamkniecie nieuzywanego konca potoku - do czytania danych z potoku.
    if(close(ptkRead) == -1) {
        perror("ERROR: Funkcja close w producent.cpp napotkala problem.\n");
        _exit(1);
    }

    // Stworzenie deskryptora dla pliku, z ktorego pobieramy surowiec.
        // Permisje: Czytanie z pliku
    int des = open(argv[1], O_RDONLY);

    // Zmienne przechowujace losowe wartosci, okreslajace ilosc pobranych danych oraz czas na jaki usypiamy program.
    int iloscDanych = losowaLiczba(10, 30);
    int sleepLength;
    // Tablica przechowujaca pobrana czesc surowca.
    char dane[31];
    // Tablica przechowujaca odpowiedni komunikat do wyswietlenia.
    char info[100];

    // Zmienna przechowujaca ilosc pobranych danych z pliku.
    int wczytaneDane;

    // Dopoki w podanym pliku znajduje sie surowiec do pobrania, pobierz z niego wczesniej wylosowana ilosc danych.
    while( (wczytaneDane = read(des, dane, iloscDanych)) > 0) {

        // Utworzenie wlasciwego komunikatu oraz jego wypisanie.
        sprintf(info, "Producent - wczytane dane: %.*s\n", wczytaneDane, dane);

        if(write(STDOUT_FILENO, info, strlen(info)) == -1) {
            perror("ERROR: Funkcja write w producent.cpp napotkala problem.\n");
            _exit(1);
        }

        // Zapisanie pobranych z pliku danych do potoku.
        if(write(ptkWrite, dane, wczytaneDane) == -1) {
            perror("ERROR: Funkcja write w producent.cpp napotkala problem.\n");
            _exit(1);
        }

        // Wylosowanie nowej ilosci danych do pobrania oraz czasu na jaki usypiamy program.
        sleepLength = losowaLiczba(1, 3);
        iloscDanych = losowaLiczba(10, 30);

        // Uspienie programu.
        sleep(sleepLength);
    }
    // Sleep, aby komunikaty w konsumencie zdazyly sie wypisac.
    sleep(5);
}