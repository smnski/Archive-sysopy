//Autor: Szymon Urbanski
#include <iostream>

#include "kolLib.h"

mqd_t des_serwera, des_klienta;

char nazwaMQ_klient[15];
void zamknijMQ_Klient_Atexit() {

    zamknijMQ(des_klienta);
    usunMQ(nazwaMQ_klient);

    std::cout << "Zakonczenie programu - zamkniecie kolejki przez klienta." << std::endl;
}

// Funkcja generujace liczbe pseudolosowa na podstawie obecnego czasu systemowego oraz losowej liczby.
int losowaLiczba(int a, int b) {

    // Ustawienie ziarna dla generatora.
    std::srand(std::time(nullptr) + std::rand());

    // Wygenerowanie losowej liczby z przedziału [a, b].
    int num = std::rand() % (b - a + 1) + a;
  
    return num;
}

int main() {

    
    
}