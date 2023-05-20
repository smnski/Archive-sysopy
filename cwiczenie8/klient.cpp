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

void wypiszAtrybuty(mqd_t des, mq_attr* attr) {

    getAttr(des, attr);

    std::cout << "Utworzono serwer o deskryptorze: " << des << ", atrybutach kolejki: " << std::endl;
    std::cout << "  Nazwa: " << nazwaMQ_klient << std::endl;
    std::cout << "  Flagi: " << attr->mq_flags << std::endl;
    std::cout << "  Max. ilosc wiadomosci: " << attr->mq_maxmsg << std::endl;
    std::cout << "  Max. rozmiar wiadomosci: " << attr->mq_msgsize << std::endl;
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

    // Deklaracje zmiennych

    char wiadomosc_wyslij[size_MQ];
    char wiadomosc_odbierz[size_MQ];

    pid_t wlasnieID;

    struct mq_attr atrybuty;

    struct mq_attr creation_attr = {
    .mq_flags = 0,
    .mq_maxmsg = 15,
    .mq_msgsize = size_MQ,
    .mq_curmsgs = 0
};
    // Koniec deklaracji

    
}