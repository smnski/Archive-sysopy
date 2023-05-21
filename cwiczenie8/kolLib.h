//Autor: Szymon Urbanski
#include <mqueue.h>
#include <ctime>

//___________________________________________________//~Wspolne~//___________________________________________________//

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

mqd_t stworzMQ(const char* nazwa, struct mq_attr *attr) {

    mqd_t des = mq_open(nazwa, O_CREAT | O_EXCL | O_RDWR, 0666, NULL);
    if(des == -1) {
        perror("ERROR: mq_open - stworzMQ - kolLib.h");
        exit(1);
    }

return des;
}

mqd_t otworzMQ_Read(const char* nazwa) {

    mqd_t des = mq_open(nazwa, O_RDONLY);
    if(des == -1) {
        perror("ERROR: mq_open - otworzMQ_Read - kolLib.h");
        exit(1);
    }

return des;
}

mqd_t otworzMQ_Write(const char* nazwa) {

    mqd_t des = mq_open(nazwa, O_WRONLY);
    if(des == -1) {
        perror("ERROR: mq_open - otworzMQ_Write - kolLib.h");
        exit(1);
    }

return des;
}

void zamknijMQ(mqd_t des) {

    if((mq_close(des)) == -1) {
        perror("ERROR: mq_close - zamknijMQ - kolLib.h");
        exit(1);
    }
}

void usunMQ(const char* nazwa) {

    if((mq_unlink(nazwa)) == -1) {
        perror("ERROR: mq_unlink - usunMQ - kolLib.h");
        exit(1);
    }
}

void wyslijMQ(mqd_t des, const char* msg, int size, int prio) {

    if((mq_send(des, msg, size, prio)) == -1) {
        perror("ERROR: mq_send - wyslijMQ - kolLib.h");
        exit(1);
    }
}

void odbierzMQ(mqd_t des, char* msg, int size) {

    if((mq_receive(des, msg, size, NULL)) == -1) {
        perror("ERROR: mq_receive - odbierzMQ - kolLib.h");
        exit(1);
    }
}

void getAttr(mqd_t des, mq_attr* attr) {

    if((mq_getattr(des, attr)) == -1) {
        perror("ERROR: mq_getattr - getAttr - kolLib.h");
        exit(1);
    }
}

void setAttr(mqd_t des, mq_attr* oldAttr, mq_attr* newAttr) {

    if((mq_setattr(des, newAttr, oldAttr)) == -1) {
        perror("ERROR: mq_setattr - setAttr - kolLib.h");
        exit(1);
    }
}