/*
*   File:       myclient.c
*
*   Purpose:    This is a skeleton file for a client.
*/

#include "myunp.h"
#include <time.h>
#include <stdio.h>


int main(int argc, char **argv)
{
    int sockfd, n,portno, bytesReceived;
    char recvline[MAXLINE+1];
    struct sockaddr_in servaddr;
    char buff[MAXLINE];
    
    if (argc!=3)
        err_quit("\n\nusage: myclient <IPaddress> <port_number>\n");
 
    portno = atoi(argv[2]);
    printf("Debug: The port number entered is: %d\n",portno);

    for ( ; ; )
    {
 
        if ( (sockfd = socket(AF_INET, SOCK_STREAM,0))<0)
            err_sys("socket error\n");
        printf ("Verifying the sockfd value for debugging:%d \n ",sockfd);
   
        bzero(&servaddr,sizeof(servaddr));
        servaddr.sin_family = AF_INET;
   
     
  
        servaddr.sin_port = htons(portno);                                  /*integer hostlong from host byte order to network byte order */
  
        if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)           /*Getting server address by converting IP address to bnary form */
            err_quit("inet_pton error for %s\n", argv[1]);

        if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)       /*connect - initiate a connection on a socket int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen); */
            err_sys("connect error\n");



        bzero(buff,MAXLINE);

        printf("Please enter the command: ");
        fgets(buff,MAXLINE-1,stdin);
  
        printf("Debug: CMD entered is: %s",buff);

        if (  write(sockfd,buff,strlen(buff)) < 0)
            err_sys("ERROR writing to socket");

        bzero(buff, MAXLINE);
    
        while  ( (bytesReceived =  read(sockfd,buff, MAXLINE)) >  0) /*read(int fd, void *buf, size_t count);attempts to read up to count bytes from file descriptor fd into the buffer starting at buf.*/
        {
            puts(buff);
        }
   

        if (bytesReceived <0) 
        {
            err_sys("error reading from socket \n");
        }

        close(sockfd);
        sleep(1);
       
    }
    exit (0);
    return 0;
}
