#include "csapp.h"
#include <sys/types.h>

/* &begin errorfuns */ 
/* &begin unixerror */
void unix_error(char *msg) {
  fprintf(stderr, "%s: %s\n", msg, strerror(errno));
  exit(0);
}
/* &end unixerror */

void posix_error(int code, char *msg) /* Posix-style error */
{
  fprintf(stderr, "%s: %s\n", msg, strerror(code));
  exit(0);
}

void gai_error(int code, char *msg) /* Getaddrinfo-style error */
{
  fprintf(stderr, "%s: %s\n", msg, strerror(code));
  exit(0);
}

void app_error(char *msg) /* Application error */
{
  fprintf(stderr, "%s\n", msg);
  exit(0);
}

void dns_error(char *msg) /* Obsolete gethostbyname error */
{
  fprintf(stderr, "%s\n", msg);
  exit(0);
}
/* &end errorfuns */

/* &begin wait */
pid_t wait(int *status) {
  pid_t pid;
  if ((pid = wait(status)) < 0) {
    unix_error("Wait");
  }
  return pid;
}
/* &end wait */

pid_t Waitpid(pid_t pid, int *iptr, int options) {
  pid_t retpid;
  if ((retpid = waitpid(pid, iptr, options)) < 0) {
    unix_error("waitpid");
  }
  return retpid;
}

void Kill(pid_t pid, int signum) {
  int rc;
  
  if ((rc = kill(pid, signum)) < 0) {
    unix_error("Kill");
  }
}

void Pause() {
  (void)pause();
  return;
}

unsigned int Sleep(unsigned int secs) {
  unsigned int rc;
  
  if ((rc = sleep(secs)) < 0) {
    unix_error("Sleep");
  }
  return rc;
}

unsigned int Alarm(unsigned int seconds) { return alarm(seconds); }

void Setpgid(pid_t pid, pid_t pgid) {
  int rc;
  
  if ((rc = setpgid(pid, pgid)) < 0) {
    unix_error("setpgid");
  }
  return;
}

pid_t Getpgrp(void) { return getpgrp(); }

ssize_t rio_readn(int fd, void *usrbuf, size_t n) {
  size_t nleft = n;
  ssize_t nread;
  char *bufp = usrbuf;
  
  while (nleft > 0) {
    if ((nread = read(fd, bufp, nleft)) < 0) {
      if (errno == EINTR) {
        continue;
      } else {
        return -1;
      } 
    } else if (nread == 0) {
      break;
    }
    
    nleft -= nread;
    bufp += nread;
  }
  
  return (n-nleft);
}

ssize_t rio_writen(int fd, void *usrbuf, size_t n) {
  if (n > sizeof usrbuf) {
    printf("riowriten: n exceed buf size\n");
    exit(0);
  }
  ssize_t nleft = n;
  ssize_t nread;
  char *bufp = usrbuf;
  
  while (nleft > 0) {
    if ((nread = write(fd, bufp, nleft)) < 0) {
      if (errno == EINTR) {
        continue;
      } else {
        return -1;
      }
    }
    
    nleft -= nread;
    bufp += nread;
  }
  return n;
}

void rio_readinitb(rio_t *rp, int fd) {
  rp->rio_fd = fd;
  rp->rio_cnt = 0;
  rp->rio_bufptr = rp->rio_buf;
}

/* rio_read - This is an wrapper for the Unix read() function that 
 * transfers min(n, rio_cnt) bytes from an internal buffer to a user 
 * buffer, where n is the number of bytes requested by the user and
 * rio_cnt is the number of readed bytes in the internal buffer. On
 * entry, rio_read() will refill the internal buffer via a call to
 * read(), if the internal buffer is empty.
 *
*/
/* &begin rio_read */
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n) {
  ssize_t cnt = n;
  
  while (rp->rio_cnt <= 0) {
    rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof rp->rio_buf);  
    if (rp->rio_cnt < 0) { /* Refill if buffer is empty */
      if (errno != EINTR) {
        return -1;
      }
    } else if (rp->rio_cnt == 0) {
      return 0;
    } else {
      rp->rio_bufptr = rp->rio_buf; /* Reset buffer pointer */
    }
  }
  
  cnt = n;
  if (rp->rio_cnt < n) {
    cnt = rp->rio_cnt;
  }
  memcpy(usrbuf, rp->rio_bufptr, cnt);
  rp->rio_bufptr += cnt;
  rp->rio_cnt -= cnt;

  return cnt;
}
/* &end rio_read */

/*
 * rio_readnb - Robustly read n bytes (buffered)
*/
/* &begin rio_readnb */
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n) {
  ssize_t nleft = n;
  ssize_t nread = 0;
  char *bufp = usrbuf;
  
  while (nleft > 0) {
    if ((nread = rio_read(rp, bufp, n)) < 0) {
      return -1;
    } else if (nread == 0) {
      break;
    } else {
      nleft -= nread;
      bufp += nread;
    }
  }
  
  return (n - nread);
}
/* &end rio_readnb */

/*
 * rio_readlineb - Robustly read a text line (buffered)
*/
/* &begin rio_readlineb */
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxline) {
  int n, rc;
  char *bufp = usrbuf, c;

  for (n = 1; n < maxline; n += 1)  {
    if ((rc = rio_read(rp, &c, 1)) == 1) {
      *bufp++ = c;
      if (c == '\n') {
        n += 1;
        break;
      }
    } else if (rc == 0) {
      printf("rc == 0\n");
      if (n == 1) {
        return 0;
      } else {
        break;
      }
    } else {
      printf("rc == -1\n");
      return -1;
    }
  }

  *bufp = 0;
  return n - 1;
}
/* &end rio_readlineb */

/***********************************
 * Wrappers for robust I/O routines
 ***********************************/
ssize_t Rio_readn(int fd, void *ptr, size_t nbytes) {
  ssize_t n;
  
  if ((n = rio_readn(fd, ptr, nbytes)) < 0) {
    unix_error("Rio_readn");
  }
  return n;
}

void Rio_writen(int fd, void *usrbuf, size_t n) {
  if (rio_writen(fd, usrbuf, n) != n) {
    unix_error("Rio_writen");
  }
}

void Rio_readinitb(rio_t *rp, int fd) { rio_readinitb(rp, fd); }

ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n) {
  ssize_t rc;
  
  if ((rc = rio_readnb(rp, usrbuf, n)) < 0) {
    unix_error("Rio_readnb");
  }
  return rc;
}

ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxline) {
  int rc;
  
  if ((rc = rio_readlineb(rp, usrbuf, maxline)) < 0) {
    unix_error("Rio_readlineb");
  }
  return rc;
}
