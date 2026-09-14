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


void process_requests(int clientfd)
{
    rio_t rio;
    char buf[MAXLINE],method[MAXLINE],uri[MAXLINE],version[MAXLINE];
    Rio_readinitb(&rio,clientfd);
    if(!Rio_readlineb(&rio,buf,MAXLINE))
        return;
    sscanf(buf,"%s %s %s",method,uri,version);
    if(strcasecmp(method,"GET")){
        clienterror(clientfd,method,"501","Not Implemented", "Proxy Server does only forwarding for GET Requets");
    }
    // parse the uri
    forward_request(method,uri,clientfd);

}

void read_requesthdrs(rio_t *rp)
{
    char buf[MAXLINE];
    Rio_readlineb(rp,buf,MAXLINE);
    
    while(strcmp(buf,"\r\n"))
    {
        Rio_readlineb(rp,buf,MAXLINE);
        printf("%s",buf);

    }
    return;


}

void clienterror(int fd, char* cause, char* errnum, char* shortmsg, char* longmsg)
{
    char buf[MAXLINE];
    sprintf(buf,"HTTP/1.0 %s %s\r\n",errnum,shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n\r\n");
    Rio_writen(fd, buf, strlen(buf));

    /* Print the HTTP response body */
    sprintf(buf, "<html><title>Tiny Error</title>");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<body bgcolor=""ffffff"">\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<p>%s: %s\r\n", longmsg, cause);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>Web Porxy server</em>\r\n");
    Rio_writen(fd, buf, strlen(buf));
}

void forward_request(char * method, char * uri, int clientfd){
    char *host_start;
    char hostname[MAXLINE];
    char path [MAXLINE];
    // accounts for max port number 65536
    char port[6];
    memset(hostname,0,sizeof(hostname));
    memset(path,0,sizeof(path));
    if(strncmp("http://",uri,7)==0)
    {
        host_start = uri +7;

    }

    const char *path_str  = strchr(host_start,'/');
    if(path_str !=NULL){
        strcpy(path,path_str);
        size_t hostname_len = path_str - host_start;
        // copy hostname to hostname buffer
        strncpy(hostname,host_start,hostname_len);
        // add the null terminator
        hostname[hostname_len] = '\0'; 

    }
    // given the case where there is no null terminator
    else
    {

        strcpy(path,"/");
        strcpy(hostname,host_start);
        


    }

    char* colon = strchr(hostname,':');
    if(colon !=NULL)
    {   *colon ='\0';
        strcpy(port,colon+1);
        



    }
    else{
        strcpy(port,"80");
    }

    make_connection_dest(method,hostname,path,port,clientfd);

    
    


    

}

void make_connection_dest(char* method,char * hostname, char* path, char * port,int clientfd)
{
    char buf [MAXLINE];
    int serverfd = Open_clientfd(hostname,port);
    // write the metod line
    sprintf(buf,"%s %s HTTP/1.0\r\n",method,path);
    rio_writen(serverfd,buf,strlen(buf));
    sprintf(buf,"Host: %s\r\n",hostname);
    rio_writen(serverfd,buf,strlen(buf));
    sprintf(buf,"User-Agent: %s\r\n",user_agent_hdr);
    rio_writen(serverfd,buf,strlen(buf));
    sprintf(buf,"Accept: text/html\r\n");
    rio_writen(serverfd,buf,strlen(buf));
    sprintf(buf,"Connection: close\r\n");
    rio_writen(serverfd,buf,strlen(buf));
    // End of HTTP headers for http r
    sprintf(buf,"Proxy-Connection: close\r\n\r\n");
    rio_writen(serverfd,buf,strlen(buf));
    read_server_response(clientfd,serverfd);













    
    

}

void read_server_response(int clientfd, int serverfd)
{
    // read from client fd 
    rio_t rio;
    char buf[MAXLINE];
    Rio_readinitb(&rio,serverfd);
    ssize_t n;
    while((n=Rio_readnb(&rio,buf,MAXLINE))>0){
        printf("%s\n",buf);
        Rio_writen(clientfd,buf,n);

    }

}
