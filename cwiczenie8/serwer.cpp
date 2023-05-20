//Autor: Szymon Urbanski
#include <iostream>
#include <signal.h>
#include "kolLib.h"

mqd_t des_serwera, des_klienta;

void zamknijMQ_Serwer_Atexit() {

    zamknijMQ(des_serwera);
    usunMQ(nazwaMQ);

    std::cout << "Zakonczenie programu - zamkniecie i usuniecie kolejki przez serwer." << std::endl;
}

void wypiszAtrybuty(mqd_t des, mq_attr* attr) {

    getAttr(des, attr);

    std::cout << "Utworzono serwer o deskryptorze: " << des << ", atrybutach kolejki: " << std::endl;
    std::cout << "  Nazwa: " << nazwaMQ << std::endl;
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

    stworzMQ(nazwaMQ, &atrybuty);
    des_serwera = otworzMQ_Read(nazwaMQ);

    atexit(zamknijMQ_Serwer_Atexit);

    wypiszAtrybuty(des_serwera, &atrybuty);

    while(!std::cin.eof()) {

        break;

    }

}