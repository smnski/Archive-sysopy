//Autor: Szymon Urbanski
#include <iostream>
#include <signal.h>
#include "kolLib.h"

mqd_t des_serwera, des_klienta;

// Funkcja zamykajaca i usuwajaca kolejke komunikatow serwera.
void zamknijMQ_Serwer() {

    zamknijMQ(des_serwera);
    usunMQ(nazwaMQ);

    std::cout << "Zakonczenie programu serwera." << std::endl;
}

// Funkcja korzystajaca z powyzszej funkcji na wyjsciu programu, gdy uzytkownik uzyje ctrl+c.
void zamknijMQ_Serwer_Signal(int signum) {

    atexit(zamknijMQ_Serwer);
    exit(EXIT_SUCCESS);
}

// Funkcja wypisujaca atrybuty danej kolejki komunikatow
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

    // Stworzenie kolejki komunikatow serwera i otworzenie jej w trybie do czytania
    stworzMQ(nazwaMQ, &creation_attr);
    des_serwera = otworzMQ_Read(nazwaMQ);

    // Obsluga zakonczenia dzialania procesu za pomoca ctrl+c dzieki signal oraz atexit.
    if(signal(SIGINT, zamknijMQ_Serwer_Signal) == SIG_ERR) {
        perror("ERROR: signal - main - serwer.cpp");
        exit(1);
    }

    wypiszAtrybuty(des_serwera, &atrybuty);

    // Petla nieskonczona - proces demon
    while(true) {

        std::cout << "Serwer oczekuje na zapytanie." << std::endl;

        sleep(losowaLiczba(1,5));

        // Odebranie zapytania wyslanego przez lkienta
        odbierzMQ(des_serwera, wiadomosc_odbierz, atrybuty.mq_msgsize);

        std::cout << "Serwer otrzymal zapytanie: " << wiadomosc_odbierz;

        // Odebranie z zapytania ID klienta, liczb oraz dzialania na nich
        sscanf(wiadomosc_odbierz, "%d %d%c%d", &klientID, &num1, &dzialanie, &num2);
        
        // Odpowiednie sformatowanie nazwy kolejki komunikatow klienta
        sprintf(nazwaMQ_klient, "/%d", klientID);

        // Wykonanie pozadanego dzialania
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

        // Otworzenie kolejki komunikatow klienta w trybie do czytania
        des_klienta = otworzMQ_Write(nazwaMQ_klient);

        // Odeslanie wyniku zapytania do klienta
        wyslijMQ(des_klienta, wiadomosc_wyslij, sizeMQ, 0);

        // Zamkniecie kolejki komunikatow klienta
        zamknijMQ(des_klienta);
    }
}