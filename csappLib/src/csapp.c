#include "csapp.h"

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
