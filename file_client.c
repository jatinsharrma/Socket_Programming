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
#include <ctype.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

long int findSize(char file_name[]) 
{ 
    // opening the file in read mode 
    FILE* fp = fopen(file_name, "r"); 
  
    // checking if the file exist or not 
    if (fp == NULL) { 
        printf("File Not Found!\n"); 
        return -1; 
    } 
  
    fseek(fp, 0L, SEEK_END); 
  
    // calculating the size of the file 
    long int res = ftell(fp); 
  
    // closing the file 
    rewind(fp);
    fclose(fp); 
  
    return res; 
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

    FILE *file_pointer;

    char file_name[] = { "file.txt" }; 
    long int size = findSize(file_name); 

    file_pointer = fopen("file.txt","rb");

    write(sockfd,&size,sizeof(long int));
    rewind(file_pointer);

    while(size)
    {
        bzero(buffer,255);
        int read = 0;
        if (size > 255) {read = 255; size-=255;}
        else {read = size-1; size = 0;}
        fread(buffer,read,1,file_pointer);
        write(sockfd,&buffer,read);
        
    }

    fclose(file_pointer);
    close(sockfd);

    return 0;
}