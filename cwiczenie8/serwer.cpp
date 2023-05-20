//Autor: Szymon Urbanski
#include <iostream>
#include <signal.h>
#include "kolLib.h"

mqd_t des_serwera, des_klienta;

void usunSerwerAtexit() {

    zamknijMQ(des_serwera);
    usunMQ(nazwa_MQ);

    std::cout << "Zakonczenie programu - usuniecie serwera." << std::endl;
}

/*
void usunSerwerSignal(int signum) {

    zamknijMQ(des_serwera);
    usunMQ(nazwa_MQ);
    
    std::cout << "Przerwanie programu - usuniecie serwera." << std::endl;
}
*/

void wypiszAtrybuty(mqd_t des, mq_attr* attr) {

    getAttr(des, attr);

    std::cout << "Utworzono serwer o deskryptorze: " << des << ", atrybutach kolejki: " << std::endl;
    std::cout << "  Nazwa: " << nazwa_MQ << std::endl;
    std::cout << "  Flagi: " << attr->mq_flags << std::endl;
    std::cout << "  Max. ilosc wiadomosci: " << attr->mq_maxmsg << std::endl;
    std::cout << "  Max. rozmiar wiadomosci: " << attr->mq_msgsize << std::endl;
}

int main() {

    // Deklaracje zmiennych
    char wiadomosc_wyslij[size_MQ];
    char wiadomosc_odbierz[size_MQ];
    char dzialanie;
    int num1, num2;

    pid_t clientID;

    struct mq_attr atrybuty;

    struct mq_attr creation_attr = {
    .mq_flags = 0,
    .mq_maxmsg = 15,
    .mq_msgsize = size_MQ,
    .mq_curmsgs = 0
};

    stworzMQ(nazwa_MQ, &atrybuty);
    des_serwera = otworzMQ_Read(nazwa_MQ);

    atexit(usunSerwerAtexit);

    wypiszAtrybuty(des_serwera, &atrybuty);

    while(!std::cin.eof()) {

        break;

    }

}