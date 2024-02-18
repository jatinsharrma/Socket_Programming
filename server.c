#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h> // this is included for proper functionting of socket and netinet

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc , char *argv[])
{
    if(argc < 2) {
        fprintf(stderr,"Port number not defined!");
        exit(0);
    }
    int sockfd , newsockfd, port_no ,n;
    char buffer[255];
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;

    sockfd = socket(AF_INET, SOCK_STREAM,0);

    if (sockfd < 0) error("Error opening socket!");

    bzero((char *) &serv_addr,sizeof(serv_addr)); // clearing the serv address

    port_no = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_no); // host to network short

    if(bind(sockfd,(struct sockaddr *) &serv_addr , sizeof(serv_addr)) < 0)
        error("Binding error!");

    listen(sockfd , 5);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd,(struct sockaddr *)&cli_addr , &clilen);

    if (newsockfd < 0) error("Error accepting!");

    while(1)
    {
        bzero(buffer,255);
        n = read(newsockfd,buffer, 255);
        if (n < 0) error("Error reading!");
        printf("Client : %s\n",buffer);
        bzero(buffer,255);
        fgets(buffer,255,stdin);

        n = write(newsockfd, buffer,strlen(buffer));
        if(n < 0) error("Errro writing!");

        int i = strncmp("Bye",buffer,3);
        if (i==0) break;
    }
    close(newsockfd);
    close(sockfd);

    return 0;
}