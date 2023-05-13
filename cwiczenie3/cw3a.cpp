// Autor: Szymon Urbański
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <string.h>

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

// Funkcja obslugujaca sygnal po jego przechwyceniu.
void handle(int nrSyg) {
    std::cout << "-----------------------\n";
    std::cout << "PID procesu: " << getpid() << "\n";
    std::cout << "Numer sygnalu: " << nrSyg << "\n";
}

int main(int argc, char *argv[]) {
    // Sprawdzenie prawidlowosci argumentow programu.
    if(!czyPoprawneDane(argc, argv)) exit(1);

    // Przypisanie numeru sygnalu, jako argument podany do programu.
    int nrSygnalu = atoi(argv[2]);

    // Odpowiednie obsluzenie sygnalu, na podstawie pierwszej litery podanego mechanizmu.
    // d - default, h - handle, i - ignore.
    switch(argv[1][0]) {
        // Obsluzenie sygnalu dla default:
        case 'd':
        if(signal(nrSygnalu, SIG_DFL) == SIG_ERR) {
                perror("ERROR: Funkcja signal ma problem z nrSygnalu.");
                exit(EXIT_FAILURE);
            }
            // Oczekiwanie przez proces na sygnal.
            pause();
            break;

        // Obsluzenie sygnalu dla handle:
        case 'h':
            if(signal(nrSygnalu, handle) == SIG_ERR) {
                perror("ERROR: Funkcja signal ma problem z nrSygnalu.");
                exit(EXIT_FAILURE);
            }
            // Oczekiwanie przez proces na sygnal, wypisanie po tym informacji o obsludze sygnalu w handlerze.
            pause();
            std::cout << "Przechwycenie sygnalu.\n";
            std::cout << "-----------------------\n";
            break;

        // Obsluzenie sygnalu dla ignore:
        case 'i':
        if(signal(nrSygnalu, SIG_IGN) == SIG_ERR) {
                perror("ERROR: Funkcja signal ma problem z nrSygnalu.");
                exit(EXIT_FAILURE);
            }
            // Oczekiwanie przez proces na sygnal.
            pause();
            break; 
    }

    return 0;
}