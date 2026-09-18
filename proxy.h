#ifndef PROXY_H
#define PROXY_H
#include "csapp.h"
#define NUM_THREADS 8
#define SBUF_SIZE 16

typedef struct{
    int * buf;
    int n;
    int front;
    int rear;
    sem_t mutex;
    sem_t slots;
    sem_t items;
}sbuf_t;
void process_requests(int fd);
void forward_request(char * method , char * uri, int clientfd );
void clienterror(int fd, char* cause, char* errnum, char* shortmsg, char* longmsg);
void forward_request(char * method, char * uri, int clientfd);
void make_connection_dest(char* method,char * hostname, char* path, char* port,int clientfd);
void read_server_response(int clientfd, int serverfd);
void sbuf_init(sbuf_t *sp,int n);
void sbuf_deinit(sbuf_t *sp);
void sbuf_insert(sbuf_t *sp, int item);
int sbuf_remove(sbuf_t* sp);
void * worker_thread(void *vargs);







#endif