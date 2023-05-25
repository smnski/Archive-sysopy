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

void travelXY(unsigned int x, unsigned int y) {
    printf("\033[%d;%dH\033[2K", y, x);
}

int losowaLiczba(int a, int b) {

    // Ustawienie ziarna dla generatora.
    std::srand(std::time(nullptr) + std::rand());

    // Wygenerowanie losowej liczby z przedziału [a, b].
    int num = std::rand() % (b - a + 1) + a;
  
    return num;
}

void sekcjaKrytyczna(int *ID, int counter) {

    pthread_mutex_lock(&myMutex);

    int wlasnaWartosc = zmiennaKontrolna;

    travelXY(70, *ID);
    printf("\033[31mWatek %d realizuje wlasna sekcje krytyczna nr.%d\033[0m\n", *ID, counter);

    wlasnaWartosc++;
    sleep(losowaLiczba(1,3));
    zmiennaKontrolna = wlasnaWartosc;

    pthread_mutex_unlock(&myMutex);

    travelXY(1, *ID);
    printf("\033[31mWatek %d zakonczyl wlasna sekcje krytyczna.\033[0m\n", *ID);
    sleep(1);
}

static void *dzialanieWatku(void *voidID) {

    int *ID = static_cast<int*>(voidID);
    travelXY(1, *ID);
    printf("\033[32mUtworzono watek o ID: %d\033[0m\n", *ID);
    sleep(losowaLiczba(1,3));

    for(int i = 0; i < sekcje; i++) 
        sekcjaKrytyczna(ID, i+1);

    return voidID;
}

int main(int argc, char** argv) {

    if(!czyPoprawneDane(argc, argv)) exit(1);
    watki = atoi(argv[1]), sekcje = atoi(argv[2]);
    pthread_t watkiID[watki];

    printf("\e[1;1H\e[2J");
    std::cout << "Utworzono mutex o adresie: " << &myMutex << std::endl;
    sleep(2);
    printf("\e[1;1H\e[2J");

    for(int i = 0; i < watki; i++) 
        watkiID[i] = i;

    for(int i = 0; i < watki; i++) {
        int* watekID = new int(i);
        pthread_create(&watkiID[i], NULL, &dzialanieWatku, (void *)watekID);
    }

    for(int i = 0; i < watki; i++)
        pthread_join(watkiID[i], NULL);

    sleep(2);

    std::cout << "wartosc kontrolna: " << zmiennaKontrolna << std::endl;

    pthread_mutex_destroy(&myMutex);
}