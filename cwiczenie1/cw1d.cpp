// Autor: Szymon Urbański
#include <iostream>
#include <unistd.h>

// Funkcja wypisujaca identyfikatory danego procesu.
void wypiszIdentyfikatory() {
    std::cout << "|UID: " << getuid() << "| GID: " << getgid() << "| PID: " << getpid()
     << "| PPID: " << getppid() << "| PGID: " << getpgid(0) << "|\n";
}

int main() {
    
    // Wypisanie identyfikatorow procesu macierzystego.
    std::cout << "Proces macierzysty: ";  
    wypiszIdentyfikatory();
    // Wypisanie, iz proces macierzysty nalezy do pokolenia 0.
    std::cout << "Pokolenie nr: 0\n";

    for(int i = 0; i < 3; i++) {

        // Stworzenie procesu potomnego funkcja fork, zapisanie jej wyniku do zmiennej iden.
        pid_t iden = fork();
        
        switch(iden) {
            // Wartosc -1 oznacza, ze przy uzywaniu funkcji fork cos poszlo nie tak.
            case -1:
                // Wypisanie erroru i wyjscie z programu.
                perror("ERROR");
                exit(-1);
                break;

            // Wartosc 0 oznacza, ze dzialamy na procesie potomnym.
            case 0:
                // Oczekujemy i sekund, aby procesy zostaly pogrupowane pokoleniami, gdzie starsze pokolenia dluzej czekaja.
                sleep(i);
                // Wypisanie identyfikatorow procesu potomnego.
                std::cout << "Proces potomny:     " ;
                wypiszIdentyfikatory();
                // Wypisanie pokolenia procesu potomnego.
                std::cout << "Pokolenie nr: " << i+1 << "\n";
                break;

            // Wartosci inne niz 0 i -1 oznaczaja, ze dzialamy na procesie macierzystym.
            default:
                // Oczekujemy i+1 sekund, aby proces macierzysty zostal zawsze zakonczony po potomnym.
                sleep(i+1);
                break;
        }
    }
    return 0;
}