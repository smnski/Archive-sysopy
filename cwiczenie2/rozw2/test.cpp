// Autor: Szymon Urbański
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

// Funkcja wypisująca identyfikatory danego procesu.
void wypiszIdentyfikatory() {
    std::cout << "|UID: " << getuid() << "| GID: " << getgid() << "| PID: " << getpid()
     << "| PPID: " << getppid() << "| PGID: " << getpgid(0) << "|\n";
}

int main() {

    std::cout << "Proces macierzysty: ";
    wypiszIdentyfikatory();

    for(int i = 0; i < 3; i++) {
        
        // Stworzenie procesu potomnego funkcja fork, zapisanie jej wyniku do zmiennej iden.
        pid_t iden = fork();
        
        switch(iden) {
            // Wartosc -1 oznacza, ze przy uzywaniu funkcji fork cos poszlo nie tak.
            case -1:
                // Wypisanie erroru i wyjscie z programu.
                perror("FORK ERROR");
                exit(-1);
                break;

            // Wartosc 0 oznacza, ze dzialamy na procesie potomnym.
            case 0:
                // Inicjujemy program wypisz.x za pomoca execlp, aby wypisac identyfikatory procesow potomnych.
                execlp("./wypisz.x", "wypisz.x", (char *) NULL); 
                perror("EXECLP ERROR");

            default:
                // Proces macierzysty ma zaczekac na zakonczenie procesu potomnego.
                wait(NULL);
                break;
        }
    }
    return 0;
}