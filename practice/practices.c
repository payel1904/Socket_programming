#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>
#include<unistd.h>
#define PORT 8080
#define MAX 1024
int main(){
    int sock_fd,client1,client2;
    struct sockaddr_in addr;
    char buffer[MAX];
    sock_fd=socket(AF_INET,SOCK_STREAM,0);
    if(sock_fd<0){perror("socket failed");exit(1);}
    addr.sin_family=AF_INET;
    addr.sin_port=htons(PORT);
    addr.sin_addr.s_addr=INADDR_ANY;
    if(bind(sock_fd,(struct sockaddr*)&addr,sizeof(addr))<0){
        perror("bind failed");
        exit(1);
    }
    listen(sock_fd,2);
    printf("server running ---- waiting for client");
    client1=accept(sock_fd,NULL,NULL);
    printf("client1 connected\n");
    client2=accept(sock_fd,NULL,NULL);
    printf("client2 connected\n");
    if(!fork()){//client1->client2
        while(1){
            int n=read(client1,buffer,MAX);
            if(n<=0)break;
            buffer[n]='\0';
            write(client2,buffer,strlen(buffer));
        }
    }else{//client2->client1;
        while(1){
        int n=read(client2,buffer,MAX);
            if(n<=0){break;}
            buffer[n]='\0';
            write(client1,buffer,strlen(buffer));
        }

    }
    close(client1);
    close(client2);
    close(sock_fd);
    return 0;

}