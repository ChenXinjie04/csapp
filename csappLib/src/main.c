#include "csapp.h"
#include <fcntl.h>
#include <rpc/netdb.h>

int main(int argc, char** argv) {
    char buf[MAXBUF];
    int fd;
    char *str = "fo\n";
    rio_t rio;
    strcpy(buf, str);

    if ((fd = open("/home/andy/Desktop/csapp/csappLib/foo.txt", 
        O_RDWR|O_APPEND, 0)) < 0) {
        unix_error("open");
    }

    Rio_readinitb(&rio, fd);
    Rio_writen(fd, buf, 3);

    return 0;
}
