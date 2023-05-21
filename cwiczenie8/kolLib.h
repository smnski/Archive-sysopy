//Autor: Szymon Urbanski
#include <mqueue.h>
#include <ctime>

//___________________________________________________//~Wspolne~//___________________________________________________//

// Nazwa kolejki komunikatow i rozmiar
#define nazwaMQ "/kolejka"
#define sizeMQ 150

// Funkcja generujace liczbe pseudolosowa na podstawie obecnego czasu systemowego oraz losowej liczby.
int losowaLiczba(int a, int b) {

    // Ustawienie ziarna dla generatora.
    std::srand(std::time(nullptr) + std::rand());

    // Wygenerowanie losowej liczby z przedziału [a, b].
    int num = std::rand() % (b - a + 1) + a;
  
    return num;
}

//___________________________________________________//~Biblioteka~//___________________________________________________//

// Funkcja tworzaca kolejke komunikatow o danych atrybutach
mqd_t stworzMQ(const char* nazwa, struct mq_attr *attr) {

    mqd_t des = mq_open(nazwa, O_CREAT | O_EXCL | O_RDWR, 0666, NULL);
    if(des == -1) {
        perror("ERROR: mq_open - stworzMQ - kolLib.h");
        exit(1);
    }

return des;
}

// Funkcja otwierajaca istniejaca kolejke komunikatow w trybie do czytania
mqd_t otworzMQ_Read(const char* nazwa) {

    mqd_t des = mq_open(nazwa, O_RDONLY);
    if(des == -1) {
        perror("ERROR: mq_open - otworzMQ_Read - kolLib.h");
        exit(1);
    }

return des;
}

// Funkcja otwierajaca istniejaca kolejke komunikatow w trybie do zapisywania
mqd_t otworzMQ_Write(const char* nazwa) {

    mqd_t des = mq_open(nazwa, O_WRONLY);
    if(des == -1) {
        perror("ERROR: mq_open - otworzMQ_Write - kolLib.h");
        exit(1);
    }

return des;
}

// Funkcja zamykajaca kolejke o danym deskryptorze
void zamknijMQ(mqd_t des) {

    if((mq_close(des)) == -1) {
        perror("ERROR: mq_close - zamknijMQ - kolLib.h");
        exit(1);
    }
}

// Funkcja usuwajaca kolejke o danej nazwie
void usunMQ(const char* nazwa) {

    if((mq_unlink(nazwa)) == -1) {
        perror("ERROR: mq_unlink - usunMQ - kolLib.h");
        exit(1);
    }
}

// Funkcja wysylajaca dana wiadomosc przez kolejke komunikatow
void wyslijMQ(mqd_t des, const char* msg, int size, int prio) {

    if((mq_send(des, msg, size, prio)) == -1) {
        perror("ERROR: mq_send - wyslijMQ - kolLib.h");
        exit(1);
    }
}

// Funkcja odbierajaca wiadomosci z kolejki komunikatow
void odbierzMQ(mqd_t des, char* msg, int size) {

    if((mq_receive(des, msg, size, NULL)) == -1) {
        perror("ERROR: mq_receive - odbierzMQ - kolLib.h");
        exit(1);
    }
}

// Funkcja zwracaja atrybuty danej kolejki komunikatow
void getAttr(mqd_t des, mq_attr* attr) {

    if((mq_getattr(des, attr)) == -1) {
        perror("ERROR: mq_getattr - getAttr - kolLib.h");
        exit(1);
    }
}

// Funkcja ustawiajaca nowe atrybuty dla danej kolejki komunikatow
void setAttr(mqd_t des, mq_attr* oldAttr, mq_attr* newAttr) {

    if((mq_setattr(des, newAttr, oldAttr)) == -1) {
        perror("ERROR: mq_setattr - setAttr - kolLib.h");
        exit(1);
    }
}