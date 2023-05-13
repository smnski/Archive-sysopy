//Autor: Szymon Urbanski
#include <iostream>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "obsluga.h"

// Funkcja sprawdzajaca poprawnosc danych na wejsciu
bool czyPoprawneDane(int argc, char** argv) {
    
    if(argc != 4) {
        std::cout << "Niepoprawna ilosc danych wejsciowych.\n";
        return false;
    }

    int n;
    n = atoi(argv[2]);
    if(n < 1 || n > 100) {
        std::cout << "Niepoprawna ilosc ilosc powielen.\n";
        return false;
    }

    n = atoi(argv[3]);
    if(n < 1 || n > 100) {
        std::cout << "Niepoprawna ilosc ilosc sekcji.\n";
        return false;
    }

return true;
}

// Funkcja otwierajaca plik i zapisujaca do niego "0".
void zapiszZero() {

    int desWrite;
    if((desWrite = open("numer.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1) {
        perror("ERROR: Funkcja open - zapiszZero - powielanie.cpp");
        exit(1);
    }

     if(write(desWrite, "0", 1) == -1) {
        perror("ERROR: Funkcja write - zapiszZero - powielanie.cpp");
        exit(1);
    }
    
    if(close(desWrite) == -1) {
        perror("ERROR: Funkcja close - zapiszZero - powielanie.cpp");
        exit(1);
    }
}

// Funkcja wczytujaca finalna liczbe w pliku numer.txt po zakonczeniu programu i porownujaca ja do oczekiwanej wartosci.
void wczytajWynik(int powielenia, int sekcje) {

    int desRead;
    if((desRead = open("numer.txt", O_RDONLY, 0644)) == -1) {
        perror("ERROR: Funkcja open - wczytajWynik - powielanie.cpp");
        exit(1);
    }

    char liczba[30];
    if((read(desRead, liczba, 30)) < 1) {
        perror("ERROR: Funkcja read - wczytajWynik - powielanie.cpp");
        exit(1);
    }
    std::cout << "Wartosc numeru w pliku: " << liczba << std::endl;
    std::cout << "Oczekiwana wartosc: " << powielenia * sekcje << std::endl;

    if((close(desRead)) == -1) {
        perror("ERROR: Funkcja close - wczytajWynik - powielanie.cpp");
        exit(1);
    }

}

// Zmienna przechowujaca nazwe semafora, aby moc korzystac z odpowiednich funkcji w SIGNAL() oraz atexit().
const char *nazwa_sem = "semafor1";
// Funkcja usuwajaca semafor, dostosowana do wymagan SIGNAL().
void usunSemSignal(int signum) { usunSem(nazwa_sem); }
// Funkcja usuwajaca semafor, dostosowana do wymagan atexit().
void usunSemAtexit() { usunSem(nazwa_sem); }


int main(int argc, char** argv) {

    // Sprawdzenie poprawnosci danych przy wywolaniu programu.
    if(!czyPoprawneDane(argc, argv)) exit(1);

    // Odpowiednie sformatowanie nazwy programu podanej w 1 argumencie, do jego uruchomienia funkcja execlp.
    char nazwaProgramu[100];
    snprintf(nazwaProgramu, 100, "./%s", argv[1]);

    // Przypisanie powielen i sekcji do zmiennych.
    int powielenia = atoi(argv[2]);
    int sekcje = atoi(argv[3]);

    // Otworzenie/stworzenie pliku numer.txt i zapisanie do niego wartosc "0".
    zapiszZero();

    // Utworzenie semafora o nazwie "semafor1" i jego inicjalizacja.
    sem_t* adres_sem = stworzSem(nazwa_sem);
    int wartosc_sem = wartoscSem(adres_sem);
    std::cout << "Utworzono semafor o adresie: " << adres_sem << ", wartosci: " << wartosc_sem << std::endl;
    
    // Dla kazdego powielenia, ktore program ma wykonac, 
    // tworzymy fork procesu i wywolujemy funkcje realizujaca wlasne sprawy oraz sekcje krytyczna dla tego procesu.
    for(int i = 0; i < powielenia; i++) {
        switch(fork()) {
            case -1:
                perror("ERROR: Funkcja fork - main - powielanie.cpp");
                exit(1);
                break;

            case 0:
                if((execlp(nazwaProgramu, argv[1], argv[3], nazwa_sem, NULL)) != 0) {
                    perror("ERROR: Funkcja execlp - main - powielanie.cpp");
                    exit(1);
                }
                break;
            // Obsluga przerwania programu za pomoca ctrl + c
            default:
                if((signal(SIGINT, usunSemSignal)) == SIG_ERR) {
                    perror("signal error");
                    exit(EXIT_FAILURE);
                }
                break;
        }
    }
    // Oczekujemy zakonczenia dzialania procesu potomnego kazdego powielenia.
    for(int i = 0; i < powielenia; i++) {

        if(waitpid(-1, NULL, 0) == -1) {
            perror("ERROR: Funkcja waitpid - main - powielenia.cpp");
            exit(403);
        }
    }
    // Po zakonczeniu dzialania wszystkich procesow potomnych, odczytujemy wynik z pliku.
    wczytajWynik(powielenia, sekcje);

    // Na koniec dzialania programu usuwamy semafor.
    if( atexit(usunSemAtexit) != 0) {
        perror("ERROR: Funkcja atexit - main - powielanie.cpp");
        exit(1);
    }
}