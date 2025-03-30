#include "csapp.h"
#include <fcntl.h>

int main(int argc, char** argv) {
    rio_t r;
    char buf[MAXBUF];
    int rc, fd;
    char *str = "bar";
    strcpy(buf, str);

    if ((fd = open("/home/andy/Desktop/csapp/csappLib/foo.txt", 
        O_RDWR, 0)) < 0) {
        unix_error("open");
    }

    if ((rc = rio_writen(fd, buf, strlen(str))) != strlen(str)) {
        unix_error("rio_readn");
    }
    
    printf("write succeed\n");

    return 0;
}
