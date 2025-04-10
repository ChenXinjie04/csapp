#include "csapp.h"
#include <netdb.h>
#include <signal.h>
#include <string.h>
#include <sys/socket.h>
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

/*******************************
 * Protocol-independent wrappers
 *******************************/
void Getaddrinfo(const char *hostname, const char *port,
                  const struct addrinfo *hints, struct addrinfo **res) {
  int rc;

  if ((rc = getaddrinfo(hostname, port, hints, res)) != 0)
    gai_error(rc, "Getaddrinfo error");
}

void Getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host,
                  size_t hostlen, char *serv, size_t servlen, int flags) 
{
  int rc;
  
  if ((rc = getnameinfo(sa, salen, host, hostlen, serv, servlen, flags)) != 0)
    gai_error(rc, "Getnameinfo error\n");
}

void Freeaddrinfo(struct addrinfo *res) { freeaddrinfo(res); }

/************************************************
 * Wrappers for Pthreads thread control functions
 ************************************************/

void Pthread_create(pthread_t *tidp, pthread_attr_t *attrp,
                    void *(*routine)(void *), void *argp) {
  int rc;

  if ((rc = pthread_create(tidp, attrp, routine, argp)) != 0)
    posix_error(rc, "Pthread_create error");
}

void Pthread_cancel(pthread_t tid) {
  int rc;

  if ((rc = pthread_cancel(tid)) != 0)
    posix_error(rc, "Pthread_cancel error");
}

void Pthread_join(pthread_t tid, void **thread_return) {
  int rc;

  if ((rc = pthread_join(tid, thread_return)) != 0)
    posix_error(rc, "Pthread_join error");
}

/* $begin detach */
void Pthread_detach(pthread_t tid) {
  int rc;

  if ((rc = pthread_detach(tid)) != 0)
    posix_error(rc, "Pthread_detach error");
}
/* $end detach */

void Pthread_exit(void *retval) { pthread_exit(retval); }

pthread_t Pthread_self(void) { return pthread_self(); }

void Pthread_once(pthread_once_t *once_control, void (*init_function)()) {
  pthread_once(once_control, init_function);
}

/*******************************
 * Wrappers for Posix semaphores
 *******************************/
void Sem_init(sem_t *sem, int pshared, unsigned int value) {
  if (sem_init(sem, pshared, value) < 0)
    unix_error("Sem_init error");
}

void P(sem_t *sem) {
  if (sem_wait(sem) < 0)
    unix_error("P error");
}

void V(sem_t *sem) {
  if (sem_post(sem) < 0)
    unix_error("V error");
}

/*********************************************
 * Wrappers for unix process control functions
 *********************************************/

/* &begin forkwrapper */
pid_t Fork(void) {
  pid_t pid;
  if ((pid = fork()) < 0) {
    unix_error("Fork error");
  }
  return pid;
}
/* &end forkwrapper */

/* &begin wait */
pid_t Wait(int *status) {
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

void Execve(const char *filename, char *const argv[], char *const envp[]) {
  if (execve(filename, argv, envp) < 0)
    unix_error("Execve error");
}

/************************************
 * Wrappers for Unix signal functions
 ************************************/

/* &begin sigaction */
handler_t *Signal(int signum, handler_t handler) {
  struct sigaction action, old_action;

  action.sa_handler = handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = SA_RESTART;

  if (sigaction(signum, &action, &old_action) < 0) {
    unix_error("sigaction");
  }

  return old_action.sa_handler;
}
/* &end sigaction */
void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset) {
  if (sigprocmask(how, set, oldset) < 0) {
    unix_error("sigprocmask error");
  }
  return;
}

void Sigemptyset(sigset_t *set) {
  if (sigemptyset(set) < 0) {
    unix_error("sigemptyset");
  }
  return;
}

void Sigfillset(sigset_t *set) {
  if (sigfillset(set) < 0) {
    unix_error("Sigfillset error");
  }
  return;
}

void Sigaddset(sigset_t *set, int signum) {
  if (sigaddset(set, signum)) {
    unix_error("Sigaddset error");
  }
  return;
}

void Sigdelset(sigset_t *set, int signum) {
  if (sigdelset(set, signum) < 0) {
    unix_error("sigdelset error");
  }
  return;
}

int Sigismember(const sigset_t *set, int signum) {
  int rc;
  if ((rc = Sigismember(set, signum)) < 0) {
    unix_error("Sigismember error");
  }
  return rc;
}

int Sigsuspend(const sigset_t *set) {
  int rc = sigsuspend(set);
  if (errno != EINTR) {
    unix_error("Sigsuspend error");
  }
  return rc;
}

/*************************************************************
 * The Sio (Signal-safe I/O) package - simple reentrant output
 * functions that are safe for signal handlers.
 *************************************************************/

/* Private sio functions */

/* &begin sioprivate */
/* sio_reverse - Reverse a string (from K&R) */
static void sio_reverse(char s[]) {
  int c, i, j;
  
  for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

/* sio_ltoa - Convert long to base b string (from K&R) */
static void sio_ltoa(long v, char s[], int b) {
  char *bufp = s;
  int neg = 0;

  if (v == 0) {
    *bufp++ = '0';
  }

  if (v < 0) {
    v = -v;
    neg = 1;
  }
  
  while (v) {
    *bufp++ = (v % b >= 10) ? (v % b + 'a' - 10) : (v % b + '0');
    v /= b;
  }

  if (neg) {
    *bufp++ = '-';
  }

  *bufp = '\0';
  
  sio_reverse(s);
}

/* sio_strlen - Return length of string (from K&R) */
static size_t sio_strlen(char s[]) {
  int len = 0;
  char *bufp = s;

  while (*bufp != '\0') {
    bufp += 1;
    len += 1;
  }

  return len;
}

/* &end sioprivate */

/* Public Sio functions */
/* &begin siopublic */
ssize_t Sio_puts(char s[]) /* Put string */
{
  return write(STDOUT_FILENO, s, sio_strlen(s));
}

ssize_t Sio_putl(long v) /* Put long */
{
  char buf[128];
  sio_ltoa(v, buf, 10);
  return Sio_puts(buf);
}

void Sio_puterror(char *msg) /* Put error message and exit */
{
  Sio_puts(msg);
  _exit(1);
}
/* &end siopublic */

/********************************
 * Wrappers for Unix I/O routines
 ********************************/
void Close(int fd) {
  int rc;
  
  if ((rc = close(fd)) < 0) {
    unix_error("Close error");
  }
}

int Dup2(int fd1, int fd2) {
  int rc;

  if ((rc = dup2(fd1, fd2)) < 0)
    unix_error("Dup2 error");
  return rc;
}

int Select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
           struct timeval *timeout) {
  int rc;

  if ((rc = select(n, readfds, writefds, exceptfds, timeout)) < 0)
    unix_error("Select error");
  return rc;
}

/***************************
 * Socket interface wrappers
 ***************************/
void Setsockopt(int s, int level, int optname, const void *optval, int optlen) {
  int rc;
  
  if ((rc = setsockopt(s, level, optname, optval, optlen)) < 0)
    unix_error("Setsockopt error");
}

int Accept(int s, struct sockaddr *addr, socklen_t *addrlen) {
  int rc;
  
  if ((rc = accept(s, addr, addrlen)) < 0)
    unix_error("Accept error");
  return rc;
}

/******************************************
 * Wrappers for the Standard I/O functions.
 ******************************************/
char *Fgets(char *ptr, int n, FILE *stream) {
  char *rptr;

  if (((rptr = fgets(ptr, n, stream)) == NULL) && ferror(stream))
    app_error("Fgets error\n");
  return rptr;
}

void Fputs(const char *ptr, FILE *stream) {
  if (fputs(ptr, stream) == EOF)
    app_error("Fputs error\n");
}

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
  if (n >= MAXBUF) {
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
      if (n == 1) {
        return 0;
      } else {
        break;
      }
    } else {
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

/********************************
 * Client/server helper functions
 ********************************/
/*
 * open_clientfd - open connection to server at <hostname, port>
 * and return a socket descriptor ready for reading and writing.
 * Return -1 and set errno on Unix error.
 */
int open_clientfd(char *hostname, char *port) {
  int clientfd;
  struct addrinfo *listp, *p, hints;
  
  memset(&hints, 0, sizeof hints);
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_ADDRCONFIG;
  hints.ai_flags |= AI_NUMERICSERV;
  Getaddrinfo(hostname, port, &hints, &listp);
  
  for (p = listp; p != NULL; p = p->ai_next) {
    if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
      continue;
    
    if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1)
      break;
    Close(clientfd);
  }
  
  Freeaddrinfo(listp);
  if (!p)
    return -1;
  return clientfd;
}

int open_listenfd(char *port) {
  int listenfd, optval = 1;
  struct addrinfo *listp, *p, hints;
  
  memset(&hints, 0, sizeof hints);
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
  hints.ai_flags |= AI_NUMERICSERV;
  Getaddrinfo(NULL, port, &hints, &listp);
  
  for (p = listp; p; p = p->ai_next) {
    if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
      continue;
    
    Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(int));
    if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
      break;
    Close(listenfd);
  }
  
  Freeaddrinfo(listp);
  if (listen(listenfd, LISTENQ) < 0) {
    Close(listenfd);
    return -1;
  }
  return listenfd;
}

int Open_clientfd(char *hostname, char *port) {
  int rc;
  if ((rc = open_clientfd(hostname, port)) < 0) {
    fprintf(stderr, "Open_clientfd error\n");
  }
  return rc;
}

int Open_listenfd(char *port) {
  int rc;
  
  if ((rc = open_listenfd(port)) < 0) {
    fprintf(stderr, "Open_listenfd error\n");
  }
  return rc;
}

/************************************
 * Wrappers for memory mmap functions
 ************************************/
void *Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset) {
  void *ptr;
  
  if ((ptr = mmap(addr, len, prot, flags, fd, offset)) == (void *)-1) {
    unix_error("mmap error");
  }
  return ptr;
}

void Munmap(void *start, size_t length) {
  if (munmap(start, length) < 0)
    unix_error("munmap error");
}
