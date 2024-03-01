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
    serv_addr.sin_port = htons(port_no); // htons function takes a 16-bit number in host byte order and returns a 16-bit number in network byte order used in TCP/IP networks

    if(bind(sockfd,(struct sockaddr *) &serv_addr , sizeof(serv_addr)) < 0)
        error("Binding error!");

    listen(sockfd , 5);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd,(struct sockaddr *)&cli_addr , &clilen);

    if (newsockfd < 0) error("Error accepting!");

    FILE *fp;

    fp = fopen("receive.txt","wb");

    int size;
    
    read(newsockfd,&size,sizeof(int));
    printf("%d",size);
    while(size>0)
    {
        bzero(buffer,255);

        int write = 0;
        read(newsockfd,buffer,255);
        if(size > 255 ){write=255; size-= 255;}
        else {write = size+3; size=0;}
        fwrite(buffer,write,1,fp);
    }

    close(newsockfd);
    close(sockfd);
    fclose(fp);
    return 0;
}