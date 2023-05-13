//Autor: Szymon Urbanski
#include <iostream>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include "SemMem.h"

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

const char* nazwa_sem = "Semafor";
const char* nazwa_SHM = "/SHM_Object";

void usun_Sem_SHM() {
    usunSem(nazwa_sem);
    usunSHM(nazwa_SHM);
}

void usun_Sem_SHM_Sig(int signum) {
    usunSem(nazwa_sem);
    usunSHM(nazwa_SHM);
}

int main(int argc, char* argv[]) {
    //Sprawdzenie, czy program zostal uruchomiony z poprawnymi argumentami.
    if(!czyPoprawneDane(argc, argv)) exit(1);

    sem_t* adres_sem = stworzSem(nazwa_sem);
    int wartosc_sem = wartoscSem(adres_sem);
    std::cout << "Utworzono semafor o adresie: " << adres_sem << ", wartosci: " << wartosc_sem << std::endl;

    int des_SHM = stworzSHM(nazwa_SHM);
    truncSHM(des_SHM);
    std::cout << "Utworzono nowy Shared Memory Object." << std::endl;

    // Pierwszy fork
    switch(fork()) {
        case -1:
            perror("ERROR: x - x - SemMem.h"); //update
            exit(1);
        break;

        case 0:
            if(execlp("./producent.x", "producent", argv[1], nazwa_sem, nazwa_SHM, NULL) == -1) {
                perror("ERROR: x - x - SemMem.h"); //update
                exit(1);
            }
        break;

        default:
        break;
    }

    // Drugi fork
    switch(fork()) {
        case -1:
            perror("ERROR: x - x - SemMem.h"); //update
            exit(1);
        break;

        case 0:
            if(execlp("./konsument.x", "konsument", argv[2], nazwa_sem, nazwa_SHM, NULL) == -1) {
                perror("ERROR: x - x - SemMem.h"); //update
                exit(1);
            }
        break;

        default:
        break;
    }

    if(signal(SIGINT, usun_Sem_SHM_Sig) == SIG_ERR) {
        perror("ERROR: x - x - SemMem.h"); //update
        exit(1);
    }

    for(int i = 0; i < 2; i++) {

        if(waitpid(-1, NULL, 0) == -1) {
            perror("ERROR: x - x - SemMem.h"); //update
            exit(1);
        }
    }

    if(atexit(usun_Sem_SHM) != 0) {
        perror("ERROR: x - x - SemMem.h"); //update
        exit(1);
    }
}