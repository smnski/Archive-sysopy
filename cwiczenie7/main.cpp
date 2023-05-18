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

// Przypisanie nazw semaforow i PD do zmiennych
const char* nazwa_sem_prod = "semaforProd";
const char* nazwa_sem_kons = "semaforKons";
const char* nazwa_SHM = "/SHM_Object";

// Funkcja usuwajaca semafory i PD, dostosowana do wymagan atexit.
void usun_Sem_SHM() {
    usunSem(nazwa_sem_prod);
    usunSem(nazwa_sem_kons);
    usunSHM(nazwa_SHM);

    std::cout << "Zakonczenie programu - usunieto semafory oraz obiekt PD." << std::endl;
}

// Funkcja usuwajaca semafory i PD, dostosowana do wymagan signal.
void usun_Sem_SHM_Sig(int signum) {
    usunSem(nazwa_sem_prod);
    usunSem(nazwa_sem_kons);
    usunSHM(nazwa_SHM);

    std::cout << "Przerwano program - usunieto semafory oraz obiekt PD." << std::endl;
}

int main(int argc, char* argv[]) {
    //Sprawdzenie, czy program zostal uruchomiony z poprawnymi argumentami.
    if(!czyPoprawneDane(argc, argv)) exit(1);

    // Utworzenie semaforow producenta i konsumenta.
    std::cout << "____________________________________________________________________" << std::endl;
    sem_t* adres_sem_prod = stworzSem(nazwa_sem_prod, NBUF);
    sem_t* adres_sem_kons = stworzSem(nazwa_sem_kons, 0);

    int wartosc_sem_prod = wartoscSem(adres_sem_prod);
    int wartosc_sem_kons = wartoscSem(adres_sem_kons);

    std::cout << "Utworzono semafor producenta o adresie: " << adres_sem_prod << ", wartosci: " << wartosc_sem_prod << std::endl;
    std::cout << "Utworzono semafor konsumenta o adresie: " << adres_sem_kons << ", wartosci: " << wartosc_sem_kons << std::endl;

    // Utworzenie obiektu pamieci dzielonej.
    int des_SHM = stworzSHM(nazwa_SHM);
    int dl_SHM = truncSHM(des_SHM, 200);
    std::cout << "Utworzono nowy obiekt pamieci dzielonej o deskryptorze: " << des_SHM << ", rozmiarze: " << dl_SHM << std::endl;
    std::cout << "____________________________________________________________________" << std::endl;

    // Funkcja usuwajaca semafory i PD przy wyjsciu z programu.
    if(atexit(usun_Sem_SHM) != 0) {
        perror("ERROR: atexit - main.cpp\n");
        exit(1);
    }

    // Pierwszy fork - wywolanie producenta
    switch(fork()) {
        case -1:
            perror("ERROR: fork - main.cpp\n");
            exit(1);
        break;

        case 0:
            if(execlp("./producent.x", "producent", argv[1], nazwa_sem_prod, nazwa_sem_kons, nazwa_SHM, std::to_string(des_SHM).c_str(), NULL) == -1) {
                perror("ERROR: fork - main.cpp\n");
                exit(1);
            }
        break;

        default:
        sleep(1);
        break;
    }

    // Drugi fork - wywolanie konsumenta
    switch(fork()) {
        case -1:
            perror("ERROR: fork - main.cpp\n");
            exit(1);
        break;

        case 0:
            if(execlp("./konsument.x", "konsument", argv[2], nazwa_sem_prod, nazwa_sem_kons, nazwa_SHM, std::to_string(des_SHM).c_str(), NULL) == -1) {
                perror("ERROR: fork - main.cpp\n");
                exit(1);
            }
        break;

        default:
        break;
    }

    // Funkcja usuwajaca semafory i PD przy zatrzymaniu programu ctrl+c.
    if(signal(SIGINT, usun_Sem_SHM_Sig) == SIG_ERR) {
        perror("ERROR: signal - main.cpp"); //update
        exit(1);
    }

    // Oczekiwanie na zakonczenie dzialania producenta i konsumenta.
    for(int i = 0; i < 2; i++) {

        if(waitpid(-1, NULL, 0) == -1) {
            perror("ERROR: waitpid - main.cpp"); //update
            exit(1);
        }
    }
}