#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<stdarg.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/time.h>
#include<sys/ioctl.h>
#include<netdb.h>
#include<string.h>
#define  SERVER_PORT 80

#define MAXLINE 4096  
#define SA struct  sockaddr

void error_hendling(char *error);

int main(int argc, char **argv)  //arg ip addres 
{
    int sockfd, n;       
    int sendbytes;
    struct sockaddr_in servaddr;
    char sendline[MAXLINE];
    char recvline[MAXLINE];
    
    if(argc!=2)
        error_hendling("input ip address!!!");
    
    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)  //creating socket
        error_hendling("Error while creating the socket");
    bzero(&servaddr,sizeof(servaddr));          //init addres with a zero BYTE
    servaddr.sin_family= AF_INET;                   //For adress parametars
    servaddr.sin_port=htons(SERVER_PORT);

    if (inet_pton(AF_INET,argv[1],&servaddr.sin_addr)<= 0)  //Adress dosent depend of a endianes and create it
        error_hendling("error in a inet!!!");
    
    
    if(connect(sockfd, (SA*) &servaddr, sizeof(servaddr))<0) // Conecting to a socket 
        error_hendling("connect failed!!!");

    sprintf(sendline,"GET / HTTP/1.1\r\n\r\n");  //Formating for a GET request 1.1 html  
    sendbytes=strlen(sendline);                  //Number of bytes in a line
    if(write(sockfd,sendline,sendbytes) != sendbytes)  //SENDING REQUEST socket , string byte
        error_hendling("write error!!!"); 
    
    while ((n=read(sockfd,recvline,MAXLINE-1)) > 0) //Response print 
    {
        printf("%s",recvline);
        memset(recvline,0,MAXLINE);
    }
    if(n<0)
        error_hendling("read error!!!");
    exit(0);
}
void error_hendling(char *error)    // error hendiling
{
    printf("erorr:\n %s",error);
    exit(1);
}