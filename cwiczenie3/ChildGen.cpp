// Autor: Szymon Urbański
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

// Nie musimy sprawdzac, czy argumenty przyjete przez program sa poprawne, poniewaz zrobilismy to juz w programie cw3c.cpp
// a nastepnie jedynie dalej przekazalismy argumenty, ktore do niego trafily.
int main(int argc, char *argv[]) {
   
   // Stworzenie procesu potomnego funkcja fork, zapisanie jej wyniku do zmiennej iden.
   pid_t iden; 
    // Stworzenie pomocniczej zmiennej przechowujacej PID procesu potomnego.
   pid_t pomPID;
   // Stworzenie pomocnicznej zmiennej przechowujacej status, z jakim zostal zakonczony proces.
   int status;

   // Tworzymy kilka (w naszym przypadku 3) procesow potomnych.
    for(int i = 0; i < 3; i++) {
        // Stworzenie procesu potomnego funkcja fork, zapisanie jej wyniku do zmiennej iden.
        iden = fork();

        switch(iden) {
            // Wartosc -1 oznacza, ze przy uzywaniu funkcji fork cos poszlo nie tak.
            case -1:
                // Wypisanie erroru i wyjscie z programu.
                perror("ERROR: Funkcja fork w ChildGen napotkala blad.");
                exit(-1);
                break;

            // Wartosc 0 oznacza, ze dzialamy na procesie potomnym.
            case 0:
            // Proces potomny uruchamia za pomoca execlp program z podpunktu a, aby wypisac informacje
            // o sobie oraz otrzymanym sygnale. Jednoczesnie obsluga bledow za pomoca perror.
            // Podajemy NULL jako jeden z argumentow, aby ich liczba sie zgadzala po wywolaniu przez program a nie z konsoli.
            if(execlp("./cw3a.x", "./cw3a.x", argv[1], argv[2], NULL) == -1) {
                perror("ERROR: Funkcja execlp w ChildGen napotkala blad.");
            }
            break;

            // Wartosci inne niz 0 i -1 oznaczaja, ze dzialamy na procesie macierzystym.
            default:
            // Proces macierzysty oczekuje na zakonczenie procesu potomnego.
            // Zapisuje jego PID oraz status po zakonczeniu do zmiennych pomocniczych.
            pomPID = wait(&status);
            // Wypisanie informacji o zakonczeniu procesu, jego PID oraz status zakonczenia.
            std::cout << "Zakonczono proces potomny o PID: " << pomPID << " status zakonczenia: " << status << "\n";
            break;
        }
    }
            
    return 0;
}