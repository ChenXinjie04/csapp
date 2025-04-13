/*
 * tiny.c - A simple iterative HTTP/1.0 Web server that uses the
 * GET method to serve static and dynamic content.
 */
#include "csapp.h"
#include "dbg.h"
#include "sbuf.h"
#include "threadPool.h"

#include <sys/signal.h>
#include <sys/socket.h>

#define VERBOSE(msg, ...)                                                      \
  {                                                                            \
    if (verbose)                                                               \
      printf(msg, ##__VA_ARGS__);                                              \
  }

#define NBUF 4
#define MAXNTHREAD 32

static int verbose = 0;

/* &begin struct for thread communication */
typedef struct {
  thpool_t *tp;
  sbuf_t *sbufp;
} tpool_sbuf_t;
/* &end struct for thread communication */

void doit(int fd);
int read_requesthdrs(rio_t *rp, char *method);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int filesize, int only_head);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *cgiargs, int only_head);
void clienterror(int fd, char *cause, char *errnum, char *shortmsg,
                 char *longmsg);
void sigchldHandler(int sig);
void sigpipeHandler(int sig);
void *worker_thread(void *vargp);
void *load_detect_double(void *vargp);
void *load_detect_halve(void *vargp);
void *get_work_thread(void *vargp);

int main(int argc, char *argv[]) {
  int listenfd, connfd;
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  pthread_t detector_tid_double, detector_tid_halve, get_work_tid;
  char hostname[MAXLINE], port[MAXLINE];
  static sbuf_t sbuf;
  static thpool_t thpool;
  static tpool_sbuf_t tpool_sbuf;
  tpool_sbuf.sbufp = &sbuf;
  tpool_sbuf.tp = &thpool;

  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(0);
  }

  Signal(SIGCHLD, sigchldHandler);
  Signal(SIGPIPE, sigpipeHandler);

  sbuf_init(&sbuf, NBUF);

  listenfd = Open_listenfd(argv[1]);

  thpool_init(&thpool, worker_thread, &tpool_sbuf);
  Pthread_create(&detector_tid_double, NULL, load_detect_double, &tpool_sbuf);
  Pthread_create(&detector_tid_halve, NULL, load_detect_halve, &tpool_sbuf);
  Pthread_create(&get_work_tid, NULL, get_work_thread, &tpool_sbuf);
  debug("detector create success");

  while (1) {
    debug("main: loop onece");
    clientlen = sizeof(struct sockaddr_storage);
    connfd = Accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
    Getnameinfo((struct sockaddr *)&clientaddr, clientlen, hostname, MAXLINE,
                port, MAXLINE, 0);
    printf("Accepted connetion from (%s, %s)\n", hostname, port);
    sbuf_insert(&sbuf, connfd);
  }

  return 0;
}

void doit(int fd) {
  debug("entering fd=%d", fd);
  int is_static, only_head = 0, n;
  struct stat sbuf;
  char buf[MAXLINE], uri[MAXLINE], method[MAXLINE], version[MAXLINE];
  char filename[MAXLINE], cgiargs[MAXLINE];
  rio_t rio;

  /* Read request line and headers */
  Rio_readinitb(&rio, fd);
  Rio_readlineb(&rio, buf, MAXLINE);
  printf("Request headers:");
  printf("%s", buf);
  sscanf(buf, "%s %s %s", method, uri, version);
  if (strcasecmp(method, "GET") && strcasecmp(method, "HEAD") &&
      strcasecmp(method, "POST")) {
    clienterror(fd, method, "501", "Not implemented",
                "Tiny does not implement this method");
    return;
  }
  is_static = parse_uri(uri, filename, cgiargs);

  n = read_requesthdrs(&rio, method);
  if (!strcmp("POST", method)) {
    Rio_readnb(&rio, cgiargs, n);
  }
  only_head = !strcmp(method, "HEAD");
  debug("cgiargs=%s", cgiargs);

  /* Parse URI from GET request */
  if (stat(filename, &sbuf) < 0) {
    clienterror(fd, filename, "404", "Not found",
                "Tiny couldn't find this file");
    return;
  }
  debug("is_static=%d filename=%s", is_static, filename);

  if (is_static) {
    if (!S_ISREG(sbuf.st_mode) || !(S_IRUSR & sbuf.st_mode)) {
      clienterror(fd, filename, "403", "Forbidden",
                  "Tiny couldn't read the file");
    }
    serve_static(fd, filename, sbuf.st_size, only_head);
  } else {
    if (!S_ISREG(sbuf.st_mode) || !(S_IXUSR & sbuf.st_mode)) {
      clienterror(fd, filename, "403", "Forbidden",
                  "Tiny couldn't run the CGI program");
    }
    serve_dynamic(fd, filename, cgiargs, only_head);
  }
  Close(fd);
  debug("exiting");
}

void clienterror(int fd, char *cause, char *errnum, char *shortmsg,
                 char *longmsg) {
  VERBOSE("clienterror: entering\n");
  char buf[MAXLINE], body[MAXLINE];

  /* Build the HTTP response body */
  sprintf(body, "<html><title>Tiny Error</title>");
  sprintf(body,
          "%s<body bgcolor="
          "ffffff"
          ">\r\n",
          body);
  sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
  sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
  sprintf(body, "%s<hr><em>The Tiny web server</em>\r\n", body);

  /* Print the HTTP response */
  sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Content-type: text/html\r\n");
  Rio_writen(fd, buf, strlen(buf));
  sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
  Rio_writen(fd, buf, strlen(buf));
  Rio_writen(fd, body, strlen(body));
  VERBOSE("clienterror: exiting\n");
}

int read_requesthdrs(rio_t *rp, char *method) {
  VERBOSE("read_requesthdrs: entering method=%s\n", method);
  char buf[MAXLINE];
  int n = 0;

  Rio_readlineb(rp, buf, MAXLINE);
  while (strcmp(buf, "\r\n")) {
    Rio_readlineb(rp, buf, MAXLINE);
    if (!strcmp(method, "POST") && strstr(buf, "Content-Length")) {
      VERBOSE("hit Content-Length\n");
      sscanf(buf, "Content-Length: %d", &n);
    }
    printf("%s", buf);
  }
  VERBOSE("read_requesthdrs: exiting\n");
  return n;
}

int parse_uri(char *uri, char *filename, char *cgiargs) {
  VERBOSE("parse_uri: entering\n");

  if (!strstr(uri, "cgi-bin")) {
    VERBOSE("parse_uri: static content\n")
    strcpy(cgiargs, "");
    strcpy(filename, ".");
    strcat(filename, uri);
    if (filename[strlen(filename) - 1] == '/') {
      strcat(filename, "home.html");
    }
    VERBOSE("parse_uri: exiting\n");
    return 1;
  } else {
    VERBOSE("parse_uri: dynamic content\n");
    char *ptr;
    ptr = index(uri, '?');
    if (ptr) {
      *ptr = '\0';
      strcpy(cgiargs, ptr + 1);
    } else {
      strcpy(cgiargs, "");
    }
    strcpy(filename, ".");
    strcat(filename, uri);
    VERBOSE("parse_uri: exiting\n");
    return 0;
  }
}

void serve_static(int fd, char *filename, int filesize, int only_head) {
  VERBOSE("serve_static: entering\n");
  int srcfd;
  char *srcp, filetype[MAXLINE], buf[MAXBUF];

  /* Send response head to client */
  get_filetype(filename, filetype);
  sprintf(buf, "HTTP/1.0 200 OK\r\n");
  sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
  sprintf(buf, "%sConnection: close\r\n", buf);
  sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
  sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
  Rio_writen(fd, buf, strlen(buf));
  printf("Response headers:\n");
  printf("%s", buf);

  if (!only_head) {
    srcfd = open(filename, O_RDONLY, 0);
    srcp = malloc(filesize);
    Rio_readn(srcfd, srcp, filesize);
    Rio_writen(fd, srcp, filesize);
    free(srcp);
  }
}

/*
 * filetype - Derive file type from filename
 */
void get_filetype(char *filename, char *filetype) {
  if (strstr(filename, ".html")) {
    strcpy(filetype, "text/html");
  } else if (strstr(filename, ".gif")) {
    strcpy(filetype, "image/gif");
  } else if (strstr(filename, ".png")) {
    strcpy(filetype, "image/png");
  } else if (strstr(filename, ".jpg")) {
    strcpy(filetype, "image/jpg");
  } else if (strstr(filename, ".mpg")) {
    strcpy(filetype, "video/mpg");
  } else {
    strcpy(filetype, "text/plain");
  }
}

/*
 * serve_dynamic - serve_dynamic content
 */
void serve_dynamic(int fd, char *filename, char *cgiargs, int only_head) {
  VERBOSE("serve_dynamic: entering\n");
  char buf[MAXLINE], *emptylist[] = {NULL};

  /* Return first part of HTTP response */
  if (Fork() == 0) {
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));
    setenv("QUERY_STRING", cgiargs, 1);
    if (only_head) {
      setenv("REQUEST_METHOD", "HEAD", 1);
    } else {
      setenv("REQUEST_METHOD", "GET", 1);
    }
    Dup2(fd, STDOUT_FILENO);
    Execve(filename, emptylist, environ);
  }
}

void sigchldHandler(int sig) {
  while (waitpid(-1, NULL, 0) > 0) {
    printf("reap child\n");
  }
  return;
}

void sigpipeHandler(int sig) {
  Pthread_cancel(Pthread_self());
  return;
}

void *worker_thread(void *vargp) {
  tpool_sbuf_t *tpool_sbufp = (tpool_sbuf_t *)vargp;
  thpool_t *tp = tpool_sbufp->tp;
  while (1) {
    int connfd = thpool_thread_get_work(tp, Pthread_self());
    doit(connfd);
    thpool_changeStatusTid(tp, Pthread_self(), THP_WAITING);
    debug("tid [%lu] commit a work", Pthread_self());
  }
}

void *get_work_thread(void *vargp) {
  tpool_sbuf_t *tpool_sbufp = (tpool_sbuf_t *)vargp;
  sbuf_t *sbufp = tpool_sbufp->sbufp;
  thpool_t *tp = tpool_sbufp->tp;
  while (1) {
    int connfd = sbuf_remove(sbufp);
    thpool_assign_work(tp, connfd);
  }
}

void *load_detect_double(void *vargp) {
  debug("load_detect: entering");
  tpool_sbuf_t *tpool_sbufp = (tpool_sbuf_t *)vargp;
  sbuf_t *sp = tpool_sbufp->sbufp;
  thpool_t *tp = tpool_sbufp->tp;
  while (1) {
    sbuf_Pfull(sp);
    thpool_double(tp);
  }
  return NULL;
error:
  log_err("load_detect: error");
  return NULL;
}

void *load_detect_halve(void *vargp) {
  debug("load_detect: entering");
  tpool_sbuf_t *tpool_sbufp = (tpool_sbuf_t *)vargp;
  sbuf_t *sp = tpool_sbufp->sbufp;
  thpool_t *tp = tpool_sbufp->tp;
  while (1) {
    sbuf_Pempty(sp);
    thpool_halve(tp);
  }
  return NULL;
error:
  log_err("load_detect: error");
  return NULL;
}
