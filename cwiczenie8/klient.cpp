//Autor: Szymon Urbanski
#include <iostream>
#include <unistd.h>
#include "kolLib.h"

mqd_t des_serwera, des_klienta;

// Funkcja zamykajaca kolejke komunikatow klienta i serwera oraz usuwajaca kolejke klienta na wyjsciu.
char nazwaMQ_klient[15];
void zamknijMQ_Klient_Atexit() {

    zamknijMQ(des_klienta);
    zamknijMQ(des_serwera);
    usunMQ(nazwaMQ_klient);

    std::cout << "Zakonczenie programu klienta." << std::endl;
    exit(EXIT_SUCCESS);
}

// Funkcja wypisujaca atrybuty danej kolejki komunikatow
void wypiszAtrybuty(mqd_t des, mq_attr* attr) {

    getAttr(des, attr);

    std::cout << "Utworzono serwer o deskryptorze: " << des << ", atrybutach kolejki: " << std::endl;
    std::cout << "  Nazwa: " << nazwaMQ_klient << std::endl;
    std::cout << "  Flagi: " << attr->mq_flags << std::endl;
    std::cout << "  Max. ilosc wiadomosci: " << attr->mq_maxmsg << std::endl;
    std::cout << "  Max. rozmiar wiadomosci: " << attr->mq_msgsize << std::endl;
    std::cout << "_______________________________________" << std::endl;
}

int main() {

    // Deklaracje zmiennych

    char wiadomosc_wyslij[sizeMQ];
    char wiadomosc_odbierz[sizeMQ];
    char zapytanie[30];

    pid_t wlasneID = getpid();

    struct mq_attr atrybuty;

    struct mq_attr creation_attr = {
    .mq_flags = 0,
    .mq_maxmsg = 15,
    .mq_msgsize = sizeMQ,
    .mq_curmsgs = 0
};
    // Koniec deklaracji

    // Przypisanie nazwy do kolejki komunikatow klienta na podstawie jego ID
    sprintf(nazwaMQ_klient, "/%d", wlasneID);

    // Stworzenie kolejki komunikatow klienta i otworzenie jej w trybie do czytania
    stworzMQ(nazwaMQ_klient, &creation_attr);
    des_klienta = otworzMQ_Read(nazwaMQ_klient);

    atexit(zamknijMQ_Klient_Atexit);

    wypiszAtrybuty(des_klienta, &atrybuty);

    // Otworzenie kolejki komunikatow serwera po jej stworzeniu w trybie do czytania
    sleep(1);
    des_serwera = otworzMQ_Write(nazwaMQ);

    std::cout << "Wpisz swoje zapytanie: " << std::endl;

    // Dopoki podajemy zapytania dla klienta (nie zakonczymy jego dzialania ctrl+d)
    while(fgets(zapytanie, sizeof(zapytanie), stdin) != NULL) {

        sleep(losowaLiczba(1,5));

        // Sformatowanie i wyslanie zapytania przez kolejke komunikatow do serwera
        sprintf(wiadomosc_wyslij, "%d %s", wlasneID, zapytanie);
        wyslijMQ(des_serwera, wiadomosc_wyslij, sizeMQ, 0);

        std::cout << "Wyslano zapytanie do serwera." << std::endl;

        // Odebranie odpowiedzi na zapytanie od serwera
        odbierzMQ(des_klienta, wiadomosc_odbierz, atrybuty.mq_msgsize);

        std::cout << "Odebrano odpowiedz od serwera: " << wiadomosc_odbierz << std::endl << std::endl;

        std::cout << "Wpisz swoje zapytanie: " << std::endl;
    }
}