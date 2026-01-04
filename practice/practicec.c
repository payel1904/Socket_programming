#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#define MAX 1024
#define PORT 8080
int main(){
    int client;
    struct sockaddr_in client_addr;
    char buffer[MAX],msg[MAX];
    client =socket(AF_INET,SOCK_STREAM,0);
    if(client<0){perror("socket creation failed");exit(1);}
    client_addr.sin_family=AF_INET;
    client_addr.sin_port=htons(PORT);
    client_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    if(connect(client,(struct sockaddr*)&client_addr,sizeof(client_addr))<0){
        perror("connection failed ");
        exit(1);
    }
    if(!fork()){
        while(1){
            int n=read(client , buffer , MAX);
            if(n>0){buffer[n]='\0';printf("friend:%s\n",buffer);}
        }
    }else{
        fgets(msg,MAX,stdin);
        write(client,msg,strlen(msg));
    }
    close(client);
        return 0;


}
