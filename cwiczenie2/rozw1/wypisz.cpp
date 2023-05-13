#include <iostream> 
#include <unistd.h>

// Funkcja wypisująca identyfikatory danego procesu.
void wypiszIdentyfikatory() {
    std::cout << "|UID: " << getuid() << "| GID: " << getgid() << "| PID: " << getpid()
     << "| PPID: " << getppid() << "| PGID: " << getpgid(0) << "|\n";
}

int main() {
    wypiszIdentyfikatory();
}