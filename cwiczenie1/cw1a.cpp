// Autor: Szymon Urbański
#include <iostream>
#include <unistd.h>

// Funkcja wypisujaca identyfikatory danego procesu.
void wypiszIdentyfikatory() {
    std::cout << "|UID: " << getuid() << "| GID: " << getgid() << "| PID: " << getpid()
     << "| PPID: " << getppid() << "| PGID: " << getpgid(0) << "|\n";
}

int main() {
    // Wypisanie identyfikatorów procesu.
    wypiszIdentyfikatory();

    return 0;
}