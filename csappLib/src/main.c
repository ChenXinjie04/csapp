#include "csapp.h"
#include <fcntl.h>
#include <rpc/netdb.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

sigjmp_buf env;

void sigintHandler(int sig) {
    siglongjmp(env, 1);
}

int main(int argc, char *argv[], char *envp[]) {
    if (!sigsetjmp(env, 1)) {
        Signal(SIGINT, sigintHandler);
        Sio_puts("starting...\n");
    } else {
        Sio_puts("restarting...\n");
    }
    
    while (1) {
        Sleep(1);
        Sio_puts("process...\n");
    }
    
    exit(0);
}
