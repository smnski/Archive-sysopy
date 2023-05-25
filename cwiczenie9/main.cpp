// Oznaczenie kodu, jako wielokrotnego uzytku
#define _REENTRANT

#include <pthread.h>
#include <iostream>
#include <ctime>
#include <unistd.h>

// Zmienne globalne
int zmiennaKontrolna = 0;
int watki, sekcje;
pthread_mutex_t myMutex;

// Funkcja sprawdzajaca poprawnosc danych na wejsciu
bool czyPoprawneDane(int argc, char** argv) {
    
    if(argc != 3) {
        std::cout << "Niepoprawna ilosc danych wejsciowych.\n";
        return false;
    }

    int n;
    n = atoi(argv[1]);
    if(n < 1 || n > 25) {
        std::cout << "Niepoprawna ilosc watkow.\n";
        return false;
    }

    n = atoi(argv[2]);
    if(n < 1 || n > 25) {
        std::cout << "Niepoprawna ilosc sekcji.\n";
        return false;
    }

return true;
}

// Funkcja przenoszaca kursor konsoli na dana pozycje
void travelXY(unsigned int x, unsigned int y) {
    printf("\033[%d;%dH\033[2K", y, x);
}

// Funkcja losujaca losowa liczbe z przedzialu [a,b]
int losowaLiczba(int a, int b) {

    // Ustawienie ziarna dla generatora.
    std::srand(std::time(nullptr) + std::rand());

    // Wygenerowanie losowej liczby z przedziału [a, b].
    int num = std::rand() % (b - a + 1) + a;
  
    return num;
}

// Sekcja krytyczna watkow: zwiekszenie wspolnej wartosci kontrolnej
void sekcjaKrytyczna(int *ID, int counter) {

    // Zamkniecie mutexu na czas sekcji krytycznej
    pthread_mutex_lock(&myMutex);

    // Skorzystanie ze wspolnego zasobu
    int wlasnaWartosc = zmiennaKontrolna;

    // Komunikat na poczatek
    travelXY(70, *ID);
    printf("\033[31mWatek %d realizuje wlasna sekcje krytyczna nr.%d\033[0m\n", *ID, counter);

    // Zwiekszenie wlasnej wartosci
    wlasnaWartosc++;

    // Ustalenie wspolnej wartosci na wlasna wartosci po sleepie
    sleep(losowaLiczba(1,2));
    zmiennaKontrolna = wlasnaWartosc;

    // Odblokowanie mutexu po sekcji krytycznej
    pthread_mutex_unlock(&myMutex);

    // Komunikat na koniec
    travelXY(1, *ID);
    printf("\033[32mWatek %d zakonczyl wlasna sekcje krytyczna.\033[0m\n", *ID);
    sleep(1);
}

// Funkcja przekazywana do pthread_create.
    // Odpowiada za sekcje prywatna watku i wywolanie funkcji odpowiedzialnej za sekcje krytyczna.
static void *dzialanieWatku(void *voidID) {

    // Przypisanie ID watku do zmiennej
    int *ID = static_cast<int*>(voidID);

    // Komunikat o sekcji prywatnej
    travelXY(1, *ID);
    printf("\033[32mUtworzono watek o ID: %d\033[0m\n", *ID);
    
    sleep(losowaLiczba(1,3));

    // Wykonanie sekcji prywatnej i krytycznej dana ilosc razy
    for(int i = 0; i < sekcje; i++) {

        // Sekcja prywatna
        travelXY(1, *ID);
        printf("\033[34mWatek %d realizuje sekcje prywatna nr. %d\033[0m\n\n", *ID, i+1);
        sleep(losowaLiczba(1,3));

        // Sekcja krytyczna
        sekcjaKrytyczna(ID, i+1);
    }

    return voidID;
}

int main(int argc, char** argv) {

    if(!czyPoprawneDane(argc, argv)) exit(1);

    // Przypisanie argumentow uruchomienia programu
    watki = atoi(argv[1]), sekcje = atoi(argv[2]);

    // Zainicjowanie mutexu
    myMutex = PTHREAD_MUTEX_INITIALIZER;
    std::cout << "\033[2J\033[1;1H";
    std::cout << "Utworzono mutex o adresie: " << &myMutex << std::endl;

    // Przypisanie ID watkow do tablicy
    pthread_t watkiID[watki];
    for(int i = 0; i < watki; i++) 
        watkiID[i] = i+1;

    sleep(2);

    std::cout << "\033[2J\033[1;1H";

    for(int i = 0; i < watki; i++) {
        // Przypisanie miejsca w pamieci dla watkow
        int *watekID = new int(i);
        // Wpisanie do miejsc w pamieci wartosci ich ID
        *watekID = i+1;
        // Utworzenie watkow
        pthread_create(&watkiID[i], NULL, &dzialanieWatku, (void *)watekID);
    }

    // Oczekiwanie na zakonczenia dzialania watkow
    for(int i = 0; i < watki; i++)
        pthread_join(watkiID[i], NULL);

    sleep(2);

    std::cout << "Wartosc kontrolna po zakonczeniu programu: " << zmiennaKontrolna << std::endl;
    std::cout << "Oczekiwana wartosc kontrolna: " << watki * sekcje << std::endl;

    // Usuniecie mutexu
    pthread_mutex_destroy(&myMutex);
}