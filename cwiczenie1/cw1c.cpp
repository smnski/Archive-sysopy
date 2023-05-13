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
                // Oczekujemy sekunde, aby proces macierzysty zakonczyl dzialanie przed procesem potomnym.
                // W wyniku tego proces potomny zostanie adoptowany przez init lub systemd.
                sleep(1);
                // Wypisanie identyfikatorow procesu potomnego.
                std::cout << "Proces potomny: ";
                wypiszIdentyfikatory();
                break;
                
            // Wartosci inne niz 0 i -1 oznaczaja, ze dzialamy na procesie macierzystym.
            // Nie chcemy wykonywac na nim zadnych dzialan.
            default:
                break;
        }
    }
    return 0;
}