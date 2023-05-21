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

    char wiadomosc_wyslij[sizeMQ];
    char wiadomosc_odbierz[sizeMQ];
    char nazwaMQ_klient[15];
    char dzialanie;
    int num1, num2;

    pid_t klientID;

    struct mq_attr atrybuty;

    struct mq_attr creation_attr = {
    .mq_flags = 0,
    .mq_maxmsg = 15,
    .mq_msgsize = sizeMQ,
    .mq_curmsgs = 0
};
    // Koniec deklaracji

    stworzMQ(nazwaMQ, &atrybuty);
    des_serwera = otworzMQ_Read(nazwaMQ);

    atexit(zamknijMQ_Serwer_Atexit);

    wypiszAtrybuty(des_serwera, &atrybuty);

    while(!std::cin.eof()) {

        std::cout << "Serwer oczekuje na zapytanie." << std::endl;

        odbierzMQ(des_serwera, wiadomosc_odbierz, atrybuty.mq_msgsize);

        //sleep(losowaLiczba(1,2));

        std::cout << "Serwer otrzymal zapytanie." << std::endl;

        sscanf(wiadomosc_odbierz, "%d %d%c%d", &klientID, &num1, &dzialanie, &num2);
        sprintf(nazwaMQ_klient, "/%d", klientID);

        switch(dzialanie) {
            case '+':
                sprintf(wiadomosc_wyslij, "%d", num1 + num2);
            break;

            case '-':
                sprintf(wiadomosc_wyslij, "%d", num1 - num2);
            break;

            case '*':
                sprintf(wiadomosc_wyslij, "%d", num1 * num2);
            break;

            case '/':
                sprintf(wiadomosc_wyslij, "%d", num1 / num2);
            break;        

            default:
                sprintf(wiadomosc_wyslij, "ERROR: Serwer nie obsluguje takiego dzialania.");
            break;    
        }

        std::cout << "Serwer odpowiada na zapytanie." << std::endl;

        des_klienta = otworzMQ_Write(nazwaMQ_klient);
        wyslijMQ(des_klienta, wiadomosc_wyslij, sizeMQ, 0);
        zamknijMQ(des_klienta);
    }
}