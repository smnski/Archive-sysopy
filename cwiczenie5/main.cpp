// Autor: Szymon Urbanski
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

// Funkcja sprawdzajaca poprawnosc argumentow podanych do programu.
bool czyPoprawneDane(int argc, char *argv[]) {
    if(argc != 4) {
        std::cout << "ERROR: Podano nieprawidlowa ilosc argumentow.\n";
        return false;
    }

    if(strcmp(argv[1], argv[2]) == 0) {
        std::cout << "ERROR: Plik zapisu i odczytu maja ta sama nazwe.\n";
        return false;
    }

return true;
}

// Zmienna globalna przechowujaca nazwe potoku nazwanego, aby moc skorzystac z deletePipe w atexit.
char *pipeName;

// Funkcja usuwajaca potok nazwany.
void deletePipe() {
    unlink(pipeName);
} 

int main(int argc, char *argv[]) {
    //Sprawdzenie, czy program zostal uruchomiony z poprawnymi argumentami.
    if(!czyPoprawneDane(argc, argv)) exit(1);

    pipeName = argv[3];

    // Utworzenie potoku nazwanego, o nazwie podanej w argumencie 3 programu.
    if (mkfifo(pipeName, 0644) == -1) {
        perror("mkfifo error");
        exit(1);
    }

    pid_t child1, child2;

    // Pierwszy proces potomny wywoluje program producenta.
    child1 = fork();
    switch(child1) {

        case -1:
            perror("ERROR: Funkcja fork w main.cpp napotkala problem.\n");
            exit(1);
            break;

        case 0:
            if(execlp("./producent.x", "./producent.x", argv[1], argv[3], NULL) == -1) {
                perror("ERROR: Funkcja execlp w main.cpp napotkala problem.\n");
                _exit(1);
            }
            break;

        default:
            break;
    }

    // Drugi proces potomny wywoluje program konsumenta.
    child2 = fork();
    switch(child2) {

        case -1:
            perror("ERROR: Funkcja fork w main.cpp napotkala problem.\n");
            _exit(1);
            break;

        case 0:
            if(execlp("./konsument.x", "./konsument.x", argv[2], argv[3], NULL) == -1) {
                perror("ERROR: Funkcja execlp w main.cpp napotkala problem.\n");
                _exit(1);
            }
            break;

        // Oczekujemy na zakonczenie programu konsumenta przed usunieciem potoku w dalszej czesci programu.
        default:
            if(waitpid(child2, NULL, 0) == -1) {
                perror("ERROR: Funkcja waitpid w main.cpp napotkala problem.\n");
                _exit(1);
            }
            break;
    }

    // Usuniecie potoku w momencie zakonczenia dzialania programu.
    if( atexit(deletePipe) != 0 ) {
        perror("ERROR: Funkcja atexit w main.cpp napotkala problem.\n");
        exit(1);
    }
    
}