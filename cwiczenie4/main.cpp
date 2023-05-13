// Autor: Szymon Urbanski
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>

// Funkcja sprawdzajaca poprawnosc argumentow podanych do programu.
bool czyPoprawneDane(int argc, char *argv[]) {
    if(argc != 3) {
        std::cout << "ERROR: Podano nieprawidlowa ilosc argumentow.\n";
        return false;
    }

    if(strcmp(argv[1], argv[2]) == 0) {
        std::cout << "ERROR: Plik zapisu i odczytu maja ta sama nazwe.\n";
        return false;
    }

return true;
}

int main(int argc, char *argv[]) {

    // Sprawdzenie, czy program zostal uruchomiony z poprawnymi argumentami.
    if(!czyPoprawneDane(argc, argv)) exit(1);

    // Stworzenie potoku nienazwanego do odczytywania oraz zapisywania danych.
    int potoki[2];
    if (pipe(potoki) == -1) {
        perror("ERROR: Funkcja pipe w main.cpp napotkala problemm.\n");
        exit(1);
    }

    // Konwersja deskryptorow z int na char, aby przekazac je do podprogramow.
    char potokRead[5], potokWrite[5];
    sprintf(potokRead, "%d", potoki[0]);
    sprintf(potokWrite, "%d", potoki[1]);

    // Stworzenie procesu potomnego funkcja fork.
    switch(fork()) {
        case -1: 
            perror("ERROR: Funkcja fork w main.cpp napotkala problem.\n");
            exit(1);
            break;
        
        // Proces potomny wywoluje program konsumenta.
        case 0:
            if(execlp("./konsument.x", "./konsument.x", argv[2], potokRead, potokWrite, NULL) == -1) {
                perror("ERROR: Funkcja execlp w main.cpp napotkala problem.\n");
                exit(1);  
            }
            break;

        // Proces macierzysty wywoluje program producenta.
        default:
            if(execlp("./producent.x", "./producent.x", argv[1], potokRead, potokWrite, NULL) == -1) {
                perror("ERROR: Funkcja execlp w main.cpp napotkala problem.\n");
                exit(1);
            }
            break;
    } 
}