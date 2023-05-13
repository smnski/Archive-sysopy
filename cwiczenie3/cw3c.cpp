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
    // Stworzenie pomocniczej zmiennej przechowujacej PGID procesu potomnego.
    pid_t pomPGID;
    // Stworzenie pomocniczej zmiennej przechowujacej PID pierwotnego procesu potomnego.
    pid_t pomPID;

    switch(iden) {
        // Wartosc -1 oznacza, ze przy uzywaniu funkcji fork cos poszlo nie tak.
        case -1:
            // Wypisanie erroru i wyjscie z programu.
            perror("ERROR: Funkcja fork w cw3c napotkala blad.");
            exit(-1);
            break;

        // Wartosc 0 oznacza, ze dzialamy na procesie potomnym.
        case 0: 
        // Ustawiamy pierwotny proces potomny na lidera grupy.
        setpgid(0, 0);
        std::cout << "Pierwotny proces potomny o PID: " << getpid() << ", PGID: " << getpgrp() << "\n";

        // Pierwotny proces potomny ma ignorowac sygnaly od procesu macierzystego.
        if(signal(nrSygnalu, SIG_IGN) == SIG_ERR) {
                perror("ERROR: Funkcja signal ma problem z nrSygnalu.");
                exit(EXIT_FAILURE);
            }

        // Pierwotny proces potomny uruchamia program, ktory tworzy kilka (w naszym przypadku 3) procesow potomnych
        // poprzez funkcje execlp. Nastepnie procesy te rowniez poprzez execlp wywolaja program z podpunktu a,
        // aby wypisac informacje o sobie oraz przyjetych sygnalach. Jednoczesnie obsluga bledow za pomoca perror.
        // Ponownie podajemy NULL jako jeden z argumentow, aby lacznie byly 3.
        if(execlp("./ChildGen.x", "./ChildGen.x", argv[1], argv[2], NULL) == -1 ) {
            perror("ERROR: Funkcja execlp w cw3c napotkala blad.");
        }
        break;

        // Wartosci inne niz 0 i -1 oznaczaja, ze dzialamy na procesie macierzystym.
        default:
        // Czekamy sekunde, aby programy dla pierwotnego procesu potomnego zdazyly sie uruchomic.
        sleep(1);

            // Sprawdzamy, czy PGID pierwotnego procesu potomnego jest takie samo, jak utworzonych procesow potomnych.
            if((pomPGID = getpgid(iden)) == -1) {
                perror("ERROR: Funkcja getpgid w cw3c napotkala blad.");
                exit(1);
            }

            // Bedziemy wykonywac teraz dzialania na procesach potomnych utworzonych przez pierwotny proces potomny.
            for(int i = 0; i < 3; i++) {
                // Czekamy sekunde, aby programy dla nich zdazyly sie uruchomic.
                sleep(1);

                // Sprawdzamy, czy wszystkie procesy o podanym PGID istnieja.
                if(kill(pomPGID * (-1), 0) == -1) {
                    perror("ERROR: Grupa danych procesow potomnych nie istnieje.");
                    exit(1);
                }

                // Jesli tak, wysylamy do nich sygnal, aby zakonczyly zadanie.
                if(kill(pomPGID * (-1), nrSygnalu) == -1) {
                    perror("ERROR: Funkcja kill w cw3c napotkala blad.");
                    exit(1);
                }
            }
            // Stworzenie pomocnicznej zmiennej przechowujacej status, z jakim zostal zakonczony proces.
            int status;
            // Proces macierzysty oczekuje na zakonczenie procesu potomnego.
            // Zapisuje jego PID oraz status po zakonczeniu do zmiennych pomocniczych
            pomPID = wait(&status);
            // Wypisanie informacji o zakonczeniu procesu, jego PID oraz status zakonczenia.
            std::cout << "Zakonczono pierwotny proces potomny o PID: " << pomPID <<  " ze statusem: " << status << "\n";
        break;
    }

return 0;
}