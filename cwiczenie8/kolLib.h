//Autor: Szymon Urbanski
#include <mqueue.h>
#include <ctime>

//___________________________________________________//~Definicje~//___________________________________________________//

#define nazwaMQ "/kolejka"
#define size_MQ 150

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