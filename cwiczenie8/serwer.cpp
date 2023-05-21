//Autor: Szymon Urbanski
#include <iostream>
#include <signal.h>
#include "kolLib.h"

mqd_t des_serwera, des_klienta;

void zamknijMQ_Serwer_Atexit() {

    zamknijMQ(des_serwera);
    zamknijMQ(des_klienta);
    usunMQ(nazwaMQ);

    std::cout << "Zakonczenie programu serwera." << std::endl;
    exit(EXIT_SUCCESS);
}

void zamknijMQ_Serwer_Signal(int signum) {

    zamknijMQ_Serwer_Atexit();
}

void wypiszAtrybuty(mqd_t des, mq_attr* attr) {

    getAttr(des, attr);

    std::cout << "Utworzono serwer o deskryptorze: " << des << ", atrybutach kolejki: " << std::endl;
    std::cout << "  Nazwa: " << nazwaMQ << std::endl;
    std::cout << "  Flagi: " << attr->mq_flags << std::endl;
    std::cout << "  Max. ilosc wiadomosci: " << attr->mq_maxmsg << std::endl;
    std::cout << "  Max. rozmiar wiadomosci: " << attr->mq_msgsize << std::endl;
    std::cout << "_______________________________________" << std::endl;
}

int main() {

    if(mqd_t mq = mq_open(nazwaMQ, O_RDONLY); mq != -1) {
        mq_close(mq);
        usunMQ(nazwaMQ);
    }

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

    stworzMQ(nazwaMQ, &creation_attr);
    des_serwera = otworzMQ_Read(nazwaMQ);

    atexit(zamknijMQ_Serwer_Atexit);

    wypiszAtrybuty(des_serwera, &atrybuty);

    if(signal(SIGINT, zamknijMQ_Serwer_Signal) == SIG_ERR) {
        perror("ERROR: signal - main - serwer.cpp");
        exit(1);
    }

    while(!std::cin.eof()) {

        std::cout << "Serwer oczekuje na zapytanie." << std::endl;

        sleep(losowaLiczba(1,5));

        odbierzMQ(des_serwera, wiadomosc_odbierz, atrybuty.mq_msgsize);

        std::cout << "Serwer otrzymal zapytanie: " << wiadomosc_odbierz;

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
    
        std::cout << "Serwer odsyla wynik: " << wiadomosc_wyslij << std::endl << std::endl;
        des_klienta = otworzMQ_Write(nazwaMQ_klient);
        wyslijMQ(des_klienta, wiadomosc_wyslij, sizeMQ, 0);
        zamknijMQ(des_klienta);
    }
}