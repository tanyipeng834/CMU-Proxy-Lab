#include <stdio.h>
#include "csapp.h"
#include "proxy.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

int main(int argc , char** argv)
{
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    if (argc!=2){
        fprintf(stderr,"usage: %s <port>\n",argv[0]);
    }
    // this is to open a socket, bind to a port and 
    listenfd = Open_listenfd(argv[1]);
    while(1){
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA*)&clientaddr,&clientlen);
        Getnameinfo((SA*)&clientaddr,clientlen,hostname,MAXLINE,port,MAXLINE,0);
        printf("Accepted connection from (%s,%s)\n",hostname,port);
        process_requests(connfd);
        Close(connfd);

        }

}


void process_requests(int fd)
{
    rio_t rio;
    char buf[MAXLINE]
    Rio_readinitb(&rio,fd);
    if(!Rio_readlineb(&rio,buf,MAXLINE))
        return;
    printf("%s\n",buf);


}
