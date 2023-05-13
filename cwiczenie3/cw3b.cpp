// Autor: Szymon Urbański
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>

// Funkcja sprawdzajaca, czy argumenty podane do programu sa prawidlowe.
bool czyPoprawneDane(int argc, char *argv[]) {
    if(argc != 3) {
        std::cout << "ERROR: Podano zla ilosc argumentow.\n";
        return false;
    }

    if(strcmp(argv[1], "default") != 0 && strcmp(argv[1], "handle") != 0 && strcmp(argv[1], "ignore") != 0) {
        std::cout << "ERROR: Podano nieprawidlowy mechanizm reagowania.\n";
        return false;
    }

    if(atoi(argv[2]) < 1 || atoi(argv[2]) > 64) {
        std::cout << "ERROR: Podano nieprawidlowy numer sygnalu.\n";
        return false;
    }

return true;
} 

int main(int argc, char *argv[]) {
    // Sprawdzenie prawidlowosci argumentow programu.
    if(!czyPoprawneDane(argc, argv)) exit(1);

    // Przypisanie numeru sygnalu, jako argument podany do programu.
    int nrSygnalu = atoi(argv[2]);

    // Stworzenie procesu potomnego funkcja fork, zapisanie jej wyniku do zmiennej iden.
    pid_t iden = fork();

    switch(iden) {
            // Wartosc -1 oznacza, ze przy uzywaniu funkcji fork cos poszlo nie tak.
            case -1:
                // Wypisanie erroru i wyjscie z programu.
                perror("ERROR: Funkcja fork w cw3b napotkala blad.");
                exit(-1);
                break;

            // Wartosc 0 oznacza, ze dzialamy na procesie potomnym.
            case 0:
                // Uruchomienie programu z podpunktu a, aby wypisac informacje o procesie i sygnale,
                // za pomoca funkcji execlp. Jednoczesnie obsluga mozliwych bledow za pomoca perror.
                // Za jeden z argumentow podajemy NULL, gdyz program bedzie odpalony przez proces, a nie z konsoli,
                // przez co dostanie tylko 2 argumenty, a do prawidlowego dzialania wymaga 3.
                if(execlp("./cw3a.x", "./cw3a.x", argv[1], argv[2], NULL) == -1) {
                    perror("ERROR: Funkcja execlp w cw3b napotkala blad.");
                    exit(1);
                }
                break;

            // Wartosci inne niz 0 i -1 oznaczaja, ze dzialamy na procesie macierzystym.
            default:
            // Czekamy sekunde, aby programy dla procesow potomnych zdazyly sie uruchomic.
            sleep(1);
            // Sprawdzenie, czy proces istnieje.
            if(kill(iden, 0) == -1) {
                perror("ERROR: Proces potomny nie istnieje.");
                exit(1);
            }
            // Wyslanie sygnalu zakonczenia dzialania do procesu.
            if(kill(iden, nrSygnalu) == -1) {
                perror("ERROR: Funkcja kill w cw3b napotkala blad.");
                exit(1);
            }
                break;
        }
        
    return 0;
}