#include <semaphore.h>
typedef struct {
  int *buf;
  int n;
  int front;
  int rear;
  sem_t mutex;
  sem_t slots;
  sem_t items;
  sem_t full;
  sem_t empty;
} sbuf_t;

void sbuf_init(sbuf_t *sp, int n);
void sbuf_deinit(sbuf_t *sp);
void sbuf_insert(sbuf_t *sp, int item);
int sbuf_isfull(sbuf_t *sp);
int sbuf_isempty(sbuf_t *sp);
int sbuf_remove(sbuf_t *sp);
void sbuf_Pfull(sbuf_t *sbufp);
void sbuf_Vfull(sbuf_t *sbufp);
void sbuf_Pempty(sbuf_t *sbufp);
void sbuf_Vempty(sbuf_t *sbufp);
