// Autor: Szymon Urbanski
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

// Funkcja wypisująca identyfikatory danego procesu.
void wypiszIdentyfikatory() {
    std::cout << "|UID: " << getuid() << "| GID: " << getgid() << "| PID: " << getpid()
     << "| PPID: " << getppid() << "| PGID: " << getpgid(0) << "|\n";
}

// argc to ilosc argumentow podana przy wywolywaniu programy, same argumenty przechowujemy w tablicy argv
int main(int argc, char *argv[]) {
    if(argc < 2) {
        std::cerr << "Nie podano nazwy programu do uruchomienia\n";
        return -1;
    }

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
                // Aby uruchomic program wypisujacy id procesow potomnych, musimy umiescic "./" przed jego nazwa dla funkcji execlp.
                // W tym celu tworzymy tablice znakow, do ktorej wstawiamy "./", a nastepnie nazwe programu podanego do funkcji.
                char nazwaProgramu[100];
                snprintf(nazwaProgramu, 100, "./%s", argv[1]);

                // Uruchamiamy program wypisujacy id procesow potomnych, na podstawie nazwy podanej do funkcji.
                execlp(nazwaProgramu, argv[1], (char *) NULL); 

                // Jesli przy uzywaniu funkcji EXECLP cos pojdzie nie tak, wypisujemy error i wychodzimy z programu.
                perror("EXECLP ERROR");
                exit(-1);
                break;

            // Wartosci inne niz 0 i -1 oznaczaja, ze dzialamy na procesie macierzystym.
            default:
                // Proces macierzysty ma zaczekac na zakonczenie procesu potomnego.
                wait(NULL);
                break;
        }
    }
    return 0;
}