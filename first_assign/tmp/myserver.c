/*
*   File:       myserver.c
*
*   Purpose:    This is a skeleton file for a server.
*/

#include "myunp.h"
#include "errno.h"
int main(int argc, char **argv)
{
    int listenfd, connfd, portno,n, clilen, nread, str_is_enter;
    struct sockaddr_in servaddr, cli_addr;
    char buff[MAXLINE];
    char error_string[30] = "Enter valid shell command \n";
    
    if (argc!=2)
        err_quit("\n\nusage: myserver <port_number>\n");
 
    portno = atoi(argv[1]);                                                               //Using asci to integer to get the port number
//    printf("Debug: The port number entered is: %d\n", portno);

    if (n= (listenfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)                             //Creating socket and bailing out if we encounter error
        err_sys("socket error \n");

    bzero(&servaddr, sizeof(servaddr));                                                   //Clearing the sockaddr structure
    
    servaddr.sin_family = AF_INET;                                                        //Intializing the sockaddr structure to get any IP from any client                                       
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(portno);                                                    //htons: host to network conversion


    Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));                                  //Binding the socket with the server address and bailing out if any error


    Listen(listenfd, LISTENQ);                                                            //Listening for client
    
    clilen = sizeof(cli_addr);

    for ( ; ; )                                                                           //Creating infinet loop of accept, read and write`
    {   
        if ( n = (connfd = accept(listenfd, (SA *) &cli_addr , &clilen)) < 0)
            err_sys("accept error \n");   
        
        bzero(buff,MAXLINE);                                                             //Intializing the buffer arry to hold command from the client and setting to zero
            if ( n =( read(connfd,buff,MAXLINE)) <0)
                err_sys("read error \n");
        
//        printf("Received the command: %s from the client \n",buff);                         //Verifying the command received from the client on the server stdout
 
        str_is_enter=0;
        
        if ( str_is_enter == (strcmp("\n",buff)))                                         //Catching newline character without any shell commands
        {
            printf("%s",error_string);
             write(connfd,error_string,strlen(error_string)); 
        }    
        else
        {
            FILE *f = popen(buff ,"r");                                                      //Using popen func to execute the command received from the client
                if (f == NULL)
                    err_sys("popen error");
            for ( ; ; )                                                                    //Loop to handel buffer overflow due to long out put from the server
            {
                bzero(buff,MAXLINE);
                if (fgets(buff, MAXLINE, f) != NULL) 
                { 
                    printf("Number of bytes read %zu \n", strlen(buff));
 
                    if (strlen(buff) > 0)
                    {
                        printf("writing to client\n");
                       if ( (write(connfd,buff,strlen(buff))) < 0 )
                           err_sys("write error");
                    }
            
                }
                else
                {
                    printf("Finished writing to client \n");
                    break;
                } 
             
            }
                pclose (f);                                                                //Handling error from the shell command execution of popen and writing to client
        }
          
        close (connfd);
    }    
    return 0;
}
