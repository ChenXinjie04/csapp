#include "csapp.h"
#include <fcntl.h>
#include <rpc/netdb.h>
#include <stdlib.h>

void sigchldHandler(int sig) {
    int status;
    while (wait(&status) != -1) {
        Sio_puts("Handler reaped child\n");
    }
    sleep(1);
    return;
}

int main(int argc, char *argv[], char *envp[]) {
    pid_t pid;
    if (signal(SIGCHLD, sigchldHandler) == SIG_ERR) {
        perror("signal");
        _exit(1);
    }

    for (int i = 0; i < 3; ++i) {
        if ((pid = Fork()) == 0) {
            COLOR_PRINT(GRN, "hello from child %d\n", getpid());
            exit(0);
        }
    }
    while (1)
        ;
    return 0;
}
