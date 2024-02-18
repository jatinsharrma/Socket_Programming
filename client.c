/*
filename server_ipaddress portno
argv[0] = filename
argv[1] = server_ipaddress
argv[2] = portno
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h> // this is included for proper functionting of socket and netinet
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc , char *argv[])
{
    if(argc < 3) 
    {
        fprintf(stderr,"Less number of arguments supplied!");
        exit(0);
    }
    int sockfd , port_no ,n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[255];
    port_no = atoi(argv[2]);
    sockfd = socket(AF_INET,SOCK_STREAM,0);

    if (sockfd < 0) error("Error opening socket");

    server = gethostbyname(argv[1]);

    if(server == NULL) error("Error, no such host");

    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    bcopy((char *)server -> h_addr , (char *) &serv_addr.sin_addr.s_addr,server->h_length);

    serv_addr.sin_port = htons(port_no);
    if(connect(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) error("Connection failed");

    while(1)
    {
        bzero(buffer,255);
        fgets(buffer,255,stdin);
        n = write(sockfd,buffer,strlen(buffer));
        if(n < 0) error("Error on writing");
        bzero(buffer,255);
        n = read(sockfd,buffer,255);
        if(n < 0) error("Error on reading");
        printf("Server : %s\n",buffer);
        int i = strncmp("Bye",buffer,3);
        if(i == 0) break;
    }

    close(sockfd);

    return 0;
}