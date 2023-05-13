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

    // Ustawienie ziarna dla generatora
    std::srand(std::time(nullptr) + getpid() + std::rand());

    // Wygenerowanie losowej liczby z przedziału [a, b]
    int num = std::rand() % (b - a + 1) + a;
  
    return num;
}

int main(int argc, char *argv[]) {

    // Stworzenie deskryptora dla pliku, do ktorego zapisujemy surowiec.
        // Permisje: Zapisywanie danych, tworzenie pliku, wyczyszczenie zawartosci pliku.
    int desPliku;
    if( (desPliku = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1 ) {
        perror("ERROR: Funkcja open w konsument.cpp napotkala blad.");
        _exit(1);
    }

    // Stworzenie deskryptora dla potoku, z ktorego czytamy dane.
        //Permisje: Czytanie danych.
    int desPotoku;
    if((desPotoku = open(argv[2], O_RDONLY, 0644)) == -1)
    {
        perror("ERROR: Funkcja open w konsument.cpp napotkala problem.");
        _exit(1);
    }

    // Zmienne przechowujace losowe wartosci, okreslajace ilosc pobranych danych oraz czas na jaki usypiamy program.
    int iloscDanych = losowaLiczba(10, 30);
    int sleepLength;
    // Tablica przechowujaca pobrana czesc surowca.
    char dane[31];
    // Tablica przechowujaca odpowiedni komunikat do wyswietlenia.
    char info[100];
    // Zmienna przechowujaca ilosc pobranych danych z pliku.
    int wczytaneDane;

    // Dopoki w potoku znajduje sie surowiec do pobrania, pobierz z niego wczesniej wylosowana ilosc danych.
    while( (wczytaneDane = read(desPotoku, dane, iloscDanych)) > 0) {

        // Utworzenie wlasciwego komunikatu oraz jego wypisanie.
        sprintf(info, "Konsument - zapisane dane: %.*s\n", wczytaneDane, dane);

        if(write(STDOUT_FILENO, info, strlen(info)) == -1) {
            perror("ERROR: Funkcja write w konsument.cpp napotkala problem.\n");
            _exit(1);
        }

        // Zapisanie pobranych z potoku danych do pliku.
        if(write(desPliku, dane, wczytaneDane) == -1) {
            perror("ERROR: Funkcja write w konsument.cpp napotkala problem.\n");
            _exit(1);
        }

        // Wylosowanie nowej ilosci danych do pobrania oraz czasu na jaki usypiamy program.
        sleepLength = losowaLiczba(1, 3);
        iloscDanych = losowaLiczba(10, 30);

        // Uspienie programu.
        sleep(sleepLength);  
    }
    std::cout<<"-----Zakonczono przesylanie danych-----\n";
}