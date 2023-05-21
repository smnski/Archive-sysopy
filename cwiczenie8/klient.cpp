//Autor: Szymon Urbanski
#include <iostream>
#include <unistd.h>
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

int main() {

    // Deklaracje zmiennych

    char wiadomosc_wyslij[sizeMQ];
    char wiadomosc_odbierz[sizeMQ];
    char zapytanie[30]; // dzialanie przed konwersja na format do wyslania do serwera

    pid_t wlasneID = getpid();

    struct mq_attr atrybuty;

    struct mq_attr creation_attr = {
    .mq_flags = 0,
    .mq_maxmsg = 15,
    .mq_msgsize = sizeMQ,
    .mq_curmsgs = 0
};
    // Koniec deklaracji

    sprintf(nazwaMQ_klient, "/%d", wlasneID);

    stworzMQ(nazwaMQ_klient, &creation_attr);
    des_klienta = otworzMQ_Read(nazwaMQ_klient);

    atexit(zamknijMQ_Klient_Atexit);

    wypiszAtrybuty(des_klienta, &atrybuty);

    //des_serwera = otworzMQ_Write(nazwaMQ);

    while(fgets(zapytanie, sizeof(zapytanie), stdin) != NULL) {

        std::cout << "Wpisz swoje zapytanie: " << std::endl;

        //sleep(losowaLiczba(1,2));

        sprintf(wiadomosc_wyslij, "%d %s", wlasneID, zapytanie);
        wyslijMQ(des_serwera, wiadomosc_wyslij, sizeMQ, 0);

        std::cout << "Wyslano zapytanie do serwera." << std::endl;

        odbierzMQ(des_klienta, wiadomosc_odbierz, atrybuty.mq_msgsize);

        std::cout << "Odebrano odpowiedz od serwera." << std::endl;
    }
}