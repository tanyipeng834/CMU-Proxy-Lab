#ifndef PROXY_H
#define PROXY_H
#include "csapp.h"
#define NUM_THREADS 8
#define SBUF_SIZE 16
#define NBUCKETS 1024

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


typedef struct cache_entry
{
    char * url;
    // pointer to response data from server
    char * data;
    size_t size;
    struct cache_entry *hash_next;
    // Doubly linked list for keeping track of LRU state
    struct cache_entry* prev;
    struct cache_entry * next;

}cache_entry_t;

typedef struct{
    cache_entry* buckets[NBUCKETS];
}hashmap_t;

typedef struct{
    hashmap_t hashmap;
    // dummy sentinal nodes
    cache_entry_t lru_head;
    cache_entry_t lru_tail;
    // this is the total size of the cache that we are validating
    size_t size_cache;
}cache_t;

void cache_init(cache_t * cache);
void lru_remove(cache_entry_t* node);

void lru_insert(cache_t * cache, cache_entry_t* node);

unsigned long hash_url(const char *str);




#endif