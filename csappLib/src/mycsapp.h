#ifndef __CSAPP_H__
#define __CSAPP_H__

#define RIO_BUFSIZE 8192
typedef struct {
	int rio_fd;
	int rio_cnt;
	char *rio_bufp;
	char rio_buf[RIO_BUFSIZE];
} rio_t;

#endif
