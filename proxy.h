#ifndef PROXY_H
#define PROXY_H
#include "csapp.h"

void process_requests(int fd);
void forward_request(char * method , char * uri, int clientfd );
void clienterror(int fd, char* cause, char* errnum, char* shortmsg, char* longmsg);
void forward_request(char * method, char * uri, int clientfd);
void make_connection_dest(char* method,char * hostname, char* path, char* port,int clientfd);
void read_server_response(int clientfd, int serverfd);





#endif